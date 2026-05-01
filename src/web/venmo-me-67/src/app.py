from __future__ import annotations

import logging
import os
import tempfile
import threading
import uuid
from datetime import datetime, timezone
from typing import Any

from flask import Flask, jsonify, render_template, request
from PIL import Image, UnidentifiedImageError

from utils import DEFAULT_MODEL, ValidationError, run_split_pipeline


def _configure_logging() -> None:
    log_level_name = os.getenv("LOG_LEVEL", "ERROR").upper()
    log_level = getattr(logging, log_level_name, logging.ERROR)
    logging.basicConfig(
        level=log_level,
        format="%(asctime)s %(levelname)s [%(name)s] %(message)s",
    )


def _get_int_env(name: str, default: int) -> int:
    raw_value = os.getenv(name, str(default)).strip()
    try:
        return int(raw_value)
    except ValueError:
        LOGGER.error("Invalid integer for %s=%r. Falling back to %s.", name, raw_value, default)
        return default


def _utc_now_iso() -> str:
    return datetime.now(timezone.utc).isoformat()


_configure_logging()

LOGGER = logging.getLogger(__name__)

app = Flask(__name__)
app.config["MAX_CONTENT_LENGTH"] = 12 * 1024 * 1024

ACTIVE_TOKEN_STORAGE_KEY = "split-the-bill-active-token"
POLL_INTERVAL_MS = 3000
JOB_TTL_SECONDS = _get_int_env("JOB_TTL_SECONDS", 3600)

_JOBS: dict[str, dict[str, Any]] = {}
_JOBS_LOCK = threading.Lock()


def _render_page(template_name: str, title: str, page: str) -> str:
    return render_template(
        template_name,
        title=title,
        page=page,
        active_token_storage_key=ACTIVE_TOKEN_STORAGE_KEY,
        poll_interval_ms=POLL_INTERVAL_MS,
    )


def _get_configured_model() -> str:
    return os.getenv("SPLIT_MODEL", DEFAULT_MODEL).strip() or DEFAULT_MODEL


def _prune_expired_jobs(now_ts: float) -> None:
    expired_tokens = [token for token, entry in _JOBS.items() if entry.get("expires_at", 0.0) <= now_ts]
    for token in expired_tokens:
        _JOBS.pop(token, None)


def _save_job(token: str, payload: dict[str, Any]) -> None:
    now_ts = datetime.now(timezone.utc).timestamp()
    with _JOBS_LOCK:
        _prune_expired_jobs(now_ts)
        _JOBS[token] = {
            "payload": dict(payload),
            "expires_at": now_ts + JOB_TTL_SECONDS,
        }


def _load_job(token: str) -> dict[str, Any] | None:
    now_ts = datetime.now(timezone.utc).timestamp()
    with _JOBS_LOCK:
        _prune_expired_jobs(now_ts)
        entry = _JOBS.get(token)
        if not entry:
            return None
        return dict(entry.get("payload", {}))


def _update_job(token: str, **fields: Any) -> dict[str, Any]:
    now_ts = datetime.now(timezone.utc).timestamp()
    with _JOBS_LOCK:
        _prune_expired_jobs(now_ts)
        payload = dict(_JOBS.get(token, {}).get("payload", {}))
        payload.update(fields)
        _JOBS[token] = {
            "payload": payload,
            "expires_at": now_ts + JOB_TTL_SECONDS,
        }
    return dict(payload)


def _cleanup_temp_files(*file_paths: str) -> None:
    for file_path in file_paths:
        if not file_path:
            continue
        try:
            if os.path.exists(file_path):
                os.remove(file_path)
        except OSError:
            LOGGER.exception("Unable to remove temporary file %r.", file_path)


def _is_strict_valid_image_upload(uploaded_file: Any) -> bool:
    if uploaded_file is None:
        return False

    stream = getattr(uploaded_file, "stream", None)
    if stream is None:
        return False

    try:
        start_pos = stream.tell()
    except (AttributeError, OSError):
        start_pos = None

    try:
        with Image.open(stream) as image_obj:
            image_obj.verify()
            if image_obj.format not in {"JPEG", "PNG"}:
                return False
        return True
    except (UnidentifiedImageError, OSError, ValueError):
        return False
    finally:
        if start_pos is not None:
            try:
                stream.seek(start_pos)
            except OSError:
                pass


@app.get("/")
def index() -> str:
    return _render_page("index.html", "Split The Bill", "index")


@app.get("/status")
def status_page() -> str:
    return _render_page("status.html", "Split The Bill Status", "status")


def _run_job(token: str, receipt_file_path: str, instruction_file_path: str, model: str) -> None:
    try:
        _update_job(token, status="running", started_at=_utc_now_iso())
        result = run_split_pipeline(
            receipt_file_path=receipt_file_path,
            instruction_file_path=instruction_file_path,
            model=model,
            logger=LOGGER,
            job_token=token,
        )
        _update_job(
            token,
            status="completed",
            completed_at=_utc_now_iso(),
            result={
                "split": result.get("split", {}),
                "venmo": result.get("venmo", {}),
            },
        )
    except ValidationError as exc:
        try:
            _update_job(token, status="error", completed_at=_utc_now_iso(), error=str(exc))
        except Exception:
            LOGGER.exception("Unable to persist the validation error state for job %s.", token)
    except Exception as exc:
        LOGGER.exception("Unexpected failure while processing job %s.", token)
        try:
            _update_job(
                token,
                status="error",
                completed_at=_utc_now_iso(),
                error=f"Unexpected error while processing the split: {exc}",
            )
        except Exception:
            LOGGER.exception("Unable to persist the unexpected error state for job %s.", token)
    finally:
        _cleanup_temp_files(receipt_file_path, instruction_file_path)


@app.post("/process")
def process() -> Any:
    receipt = request.files.get("receipt")
    instructions = request.files.get("instructions")
    model = _get_configured_model()

    if receipt is None or instructions is None:
        return jsonify({"error": "Both receipt and instructions files are required."}), 400

    if not _is_strict_valid_image_upload(receipt):
        return jsonify({"error": "Receipt upload must be a valid JPEG or PNG image."}), 400

    token = str(uuid.uuid4())
    receipt_suffix = os.path.splitext(receipt.filename or "receipt.jpg")[1] or ".jpg"
    instruction_suffix = os.path.splitext(instructions.filename or "instructions.mp3")[1] or ".mp3"

    try:
        with tempfile.NamedTemporaryFile(delete=False, suffix=receipt_suffix) as receipt_tmp:
            receipt.save(receipt_tmp)
            receipt_file_path = receipt_tmp.name

        with tempfile.NamedTemporaryFile(delete=False, suffix=instruction_suffix) as instruction_tmp:
            instructions.save(instruction_tmp)
            instruction_file_path = instruction_tmp.name

        _save_job(
            token,
            {
                "status": "queued",
                "model": model,
                "created_at": _utc_now_iso(),
            },
        )
    except Exception as exc:
        _cleanup_temp_files(
            locals().get("receipt_file_path", ""),
            locals().get("instruction_file_path", ""),
        )
        LOGGER.exception("Unable to persist uploaded files for job %s.", token)
        return jsonify({"error": f"Unable to save the uploaded files: {exc}"}), 500

    worker = threading.Thread(
        target=_run_job,
        args=(token, receipt_file_path, instruction_file_path, model),
        daemon=True,
        name=f"split-job-{token}",
    )
    worker.start()
    return jsonify({"token": token, "status": "queued"}), 202


@app.get("/check-result")
def check_result() -> Any:
    token = request.args.get("token", "").strip()
    if not token:
        return jsonify({"error": "Missing token query parameter."}), 400

    data = _load_job(token)
    if data is None:
        return jsonify({"error": "No saved job payload was found for this token. It may have expired or been lost after restart."}), 404

    return jsonify({"token": token, **data}), 200


if __name__ == "__main__":
    port = int(os.getenv("PORT", "5000"))
    app.run(host="0.0.0.0", port=port, debug=False)
