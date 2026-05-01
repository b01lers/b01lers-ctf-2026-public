import asyncio
import base64
import json
import logging
import os
import random

import websockets

WORKERS = int(os.environ.get("WORKERS", "1"))

N = 50
RANGE_MAPPING = {"a": (2**8, 2**16-1), "b": (2**6, 2**8-1), "c": (2**10, 2**12-1)}

log = logging.getLogger("polyglot-outer")

connected_clients: set = set()
queue: asyncio.Queue = None
queued_teams: set = set()


def gen_testcases(num_cases: int) -> list[dict]:
    cases = []
    for _ in range(num_cases):
        a = random.randint(*RANGE_MAPPING["a"])
        b = random.randint(*RANGE_MAPPING["b"])
        c = random.randint(*RANGE_MAPPING["c"])
        cases.append({"input": f"{a}\n{b}\n{c}\n", "output": str(pow(a, b, c))})
    return cases


async def broadcast(message: dict):
    data = json.dumps(message)
    for ws in connected_clients.copy():
        try:
            await ws.send(data)
        except Exception:
            connected_clients.discard(ws)


async def run_inner(languages: list[str], code_b64: str, testcases: list[dict]) -> tuple[dict | None, str | None]:
    """Returns (result_dict, None) on success or (None, error_string) on failure."""
    # great coordination
    if "python" in languages:
        languages.remove("python")
        if "python0" not in languages:
            languages.append("python0")

    pkt = (
        json.dumps(
            {
                "languages": languages,
                "code": code_b64,
                "testcases": [t["input"] for t in testcases],
            },
            separators=(",", ":"),
        )
        + "\n"
    )

    try:
        reader, writer = await asyncio.wait_for(
            asyncio.open_connection("inner", 8989, limit=16 * 1024 * 1024),
            timeout=10,
        )
    except asyncio.TimeoutError:
        return None, "Timed out connecting to runner"
    except Exception as e:
        return None, f"Error connecting to runner: {e}"

    try:
        writer.write(pkt.encode())
        await writer.drain()

        stdout = await asyncio.wait_for(reader.readline(), timeout=5*60)
    except asyncio.TimeoutError:
        return None, "5-minute timeout exceeded"
    except Exception as e:
        return None, f"Error communicating with runner: {e}"
    finally:
        writer.close()
        await writer.wait_closed()

    try:
        return json.loads(stdout.decode()), None
    except json.JSONDecodeError:
        return None, "Invalid response from runner"


async def handle_connection(websocket):
    connected_clients.add(websocket)
    log.info("Client connected (%d total)", len(connected_clients))
    try:
        async for message in websocket:
            await handle_message(websocket, message)
    except websockets.ConnectionClosed:
        pass
    finally:
        connected_clients.discard(websocket)
        log.info("Client disconnected (%d total)", len(connected_clients))


async def send_error(websocket, msg: str):
    await websocket.send(json.dumps({"type": "completed", "error": msg}))


async def handle_message(websocket, raw: str):
    try:
        req = json.loads(raw)
    except json.JSONDecodeError:
        return await send_error(websocket, "invalid json")

    submission_id = req.get("id")
    if not submission_id or not isinstance(submission_id, str):
        return await send_error(websocket, "missing or invalid 'id' field")

    team = req.get("team")
    if not team or not isinstance(team, str):
        return await send_error(websocket, "missing or invalid 'team' field")

    payload = req.get("payload")
    if not payload or not isinstance(payload, str):
        return await send_error(websocket, "missing or invalid 'payload' field")

    try:
        base64.b64decode(payload)
    except Exception:
        return await send_error(websocket, "payload is not valid base64")

    languages = req.get("languages")
    if not languages or not isinstance(languages, list) or len(languages) == 0:
        return await send_error(websocket, "null or empty languages")

    # Deduplicate languages (preserving order) to prevent score inflation
    languages = list(dict.fromkeys(languages))

    if team in queued_teams:
        return await send_error(websocket, "team already in queue")

    queued_teams.add(team)
    await queue.put((submission_id, team, payload, languages))

    log.info("Queued: id=%s team=%s langs=%s", submission_id, team, languages)
    await websocket.send(json.dumps({"type": "queue", "id": submission_id, "team": team}))


async def worker():
    log.info("Worker started")
    while True:
        submission_id, team, payload, languages = await queue.get()
        log.info("Processing: id=%s team=%s", submission_id, team)

        await broadcast({"type": "test", "id": submission_id, "team": team})

        testcases = gen_testcases(N)

        try:
            result, error = await run_inner(languages, payload, testcases)
        except Exception as e:
            log.exception("Unexpected error running solution for id=%s", submission_id)
            result, error = None, f"Internal error: {e}"

        # Inner returns {"error": "..."} for validation failures (unknown
        # languages, bad JSON, etc.) -- detect and treat as a run error.
        if not error and isinstance(result, dict) and "error" in result:
            error = result["error"]

        if error:
            await broadcast(
                {
                    "type": "completed",
                    "id": submission_id,
                    "team": team,
                    "score": [0, 0],
                    "tests": [],
                    "error": error,
                }
            )
        else:
            code_length = len(base64.b64decode(payload))
            first_failure = None
            num_passed = 0
            
            # Default to expected outputs (which match program outputs if all pass)
            reported_tests = [[t["input"], t["output"]] for t in testcases]

            for lang in languages:
                lang_data = result.get(lang, {})

                if lang_data.get("err"):
                    if not first_failure:
                        first_failure = f"{lang}: {lang_data['err']}"
                        reported_tests = []
                    continue

                lang_results = lang_data.get("results") or []
                all_correct = len(lang_results) == len(testcases)
                for i, tr in enumerate(lang_results):
                    expected = testcases[i]["output"]
                    actual = (tr.get("stdout") or "").strip()
                    if actual != expected:
                        all_correct = False
                        if not first_failure:
                            first_failure = (
                                f"{lang}: testcase {i + 1} failed (expected {expected}, got {actual!r})"
                            )
                            reported_tests = []
                            for j, tc in enumerate(testcases):
                                tc_actual = ""
                                if j < len(lang_results):
                                    tc_actual = (lang_results[j].get("stdout") or "").strip()
                                reported_tests.append([tc["input"], tc_actual])
                        break

                if all_correct:
                    num_passed += 1

            if first_failure:
                score = [0, 0]
            else:
                score = [num_passed, -code_length]

            response = {
                "type": "completed",
                "id": submission_id,
                "team": team,
                "score": score,
                "tests": reported_tests,
            }
            if first_failure:
                response["error"] = first_failure

            await broadcast(response)

        queued_teams.discard(team)
        log.info("Completed: id=%s team=%s", submission_id, team)


async def main():
    global queue
    queue = asyncio.Queue()
    for i in range(WORKERS):
        asyncio.create_task(worker())
    log.info("Spawned %d worker(s)", WORKERS)

    async with websockets.serve(handle_connection, "0.0.0.0", 5000):
        log.info("WebSocket server listening on :5000")
        await asyncio.Future()


if __name__ == "__main__":
    logging.basicConfig(
        level=logging.INFO,
        format="%(asctime)s %(levelname)s %(message)s",
    )
    asyncio.run(main())
