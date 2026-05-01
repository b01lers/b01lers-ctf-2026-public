#!/usr/bin/env python3
"""Paper-guided solver for the KKI individual-attack challenge.

Attack idea (dishonest Bob with ancilla/probe):
- Use ancilla |0>.
- Apply U = CNOT(A -> C) before forwarding A.
- After public communication:
  - If set is S0, measure B in z and C in z.
  - If set is S1, measure B in x and C in z.
- Infer secret from parity of outcomes.

This follows the delayed-information advantage discussed in the paper.
"""

from __future__ import annotations

import argparse
from concurrent.futures import ProcessPoolExecutor, as_completed
import json
import math
import os
import queue
import random
import re
import subprocess
import sys
import threading
import time
from typing import Dict, List, Optional, Tuple


PAPER_EDGE_A = 0.0
PAPER_EDGE_QBER = 0.25
PAPER_EDGE_MI = 1.0


def binary_entropy(p: float) -> float:
    p = min(max(p, 0.0), 1.0)
    if p <= 1e-15 or p >= 1.0 - 1e-15:
        return 0.0
    return -(p * math.log2(p) + (1.0 - p) * math.log2(1.0 - p))


def mutual_information_from_error(q: float) -> float:
    return 1.0 - binary_entropy(q)


def theory_max_information_from_qber(d: float) -> Tuple[float, float, float]:
    """Paper frontier from Eqs. (10), (12), (14), using a = 4D - 1."""
    d = min(max(d, 0.0), 0.5)
    a = min(max(4.0 * d - 1.0, -1.0), 1.0)
    q_min = 0.5 - 0.5 * math.sqrt(max(0.0, 1.0 - a * a))
    i_max = mutual_information_from_error(q_min)
    return a, q_min, i_max


def build_cnot_a_to_c_user_matrix() -> List[List[str]]:
    """8x8 unitary in user basis |ABC>, applying C <- C xor A."""
    mat = [["0+0j" for _ in range(8)] for _ in range(8)]
    for a in (0, 1):
        for b in (0, 1):
            for c in (0, 1):
                src = (a << 2) | (b << 1) | c
                dst = (a << 2) | (b << 1) | (c ^ a)
                mat[dst][src] = "1+0j"
    return mat


def parse_measurement_outcomes(line: str) -> Dict[str, int]:
    # Example: "measurement_outcomes: b:z=1 c:z=0"
    payload = line.split(":", 1)[1].strip()
    out: Dict[str, int] = {}
    for token in payload.split():
        lhs, rhs = token.split("=")
        qubit_name = lhs.split(":", 1)[0]
        out[qubit_name] = int(rhs)
    return out


def parse_score_metrics(lines: List[str]) -> Dict[str, object]:
    metrics: Dict[str, object] = {
        "qber": None,
        "max_qber": None,
        "kept_info_accuracy": None,
        "info_error": None,
        "raw_info_accuracy": None,
        "qber_checked": None,
        "min_qber_checked": None,
        "qber_checked_z": None,
        "qber_checked_x": None,
        "min_qber_checked_per_basis": None,
        "info_rounds": None,
        "min_info_rounds": None,
        "mutual_info_empirical_bits": None,
        "min_mutual_info_bits": None,
        "paper_frontier_abs_dev_bits": None,
        "max_frontier_abs_dev_bits": None,
        "announce_balance": None,
        "delayed_advantage_bits": None,
        "raw_mutual_info_empirical_bits": None,
        "flag_line": None,
        "has_flag": False,
    }

    float_keys = {
        "qber",
        "max_qber",
        "kept_info_accuracy",
        "info_error",
        "raw_info_accuracy",
        "mutual_info_empirical_bits",
        "min_mutual_info_bits",
        "paper_frontier_abs_dev_bits",
        "max_frontier_abs_dev_bits",
        "announce_balance",
        "delayed_advantage_bits",
        "raw_mutual_info_empirical_bits",
    }
    int_keys = {
        "qber_checked",
        "min_qber_checked",
        "min_qber_checked_per_basis",
        "info_rounds",
        "min_info_rounds",
    }

    basis_checked_re = re.compile(r"qber_checked_by_basis=z:(\d+)\s+x:(\d+)")

    for line in lines:
        s = line.strip()
        if "bctf{" in s or "flag{" in s or "ctf{" in s:
            metrics["has_flag"] = True
            metrics["flag_line"] = s

        m = basis_checked_re.match(s)
        if m:
            metrics["qber_checked_z"] = int(m.group(1))
            metrics["qber_checked_x"] = int(m.group(2))
            continue

        if "=" not in s:
            continue

        key, value = s.split("=", 1)
        key = key.strip()
        value = value.strip()

        if key in float_keys:
            try:
                metrics[key] = float(value)
            except ValueError:
                pass
        elif key in int_keys:
            try:
                metrics[key] = int(value)
            except ValueError:
                pass

    qber_checked_z = metrics.get("qber_checked_z")
    qber_checked_x = metrics.get("qber_checked_x")
    if isinstance(qber_checked_z, int) and isinstance(qber_checked_x, int):
        metrics["qber_checked"] = qber_checked_z + qber_checked_x

    info_error = metrics.get("info_error")
    if isinstance(info_error, (int, float)):
        metrics["mutual_info_empirical_bits"] = mutual_information_from_error(float(info_error))

    raw_info_accuracy = metrics.get("raw_info_accuracy")
    if isinstance(raw_info_accuracy, (int, float)):
        raw_info_error = 1.0 - float(raw_info_accuracy)
        raw_mi = mutual_information_from_error(raw_info_error)
        metrics["raw_mutual_info_empirical_bits"] = raw_mi
    else:
        raw_mi = None

    mi = metrics.get("mutual_info_empirical_bits")
    qber = metrics.get("qber")
    if isinstance(mi, (int, float)) and isinstance(qber, (int, float)):
        _, _, i_max = theory_max_information_from_qber(float(qber))
        metrics["paper_frontier_abs_dev_bits"] = abs(i_max - float(mi))

    if isinstance(mi, (int, float)) and isinstance(raw_mi, (int, float)):
        metrics["delayed_advantage_bits"] = float(mi) - float(raw_mi)

    return metrics


def is_passing_output(output_text: str, timed_out: bool, exit_code: Optional[int]) -> bool:
    if timed_out:
        return False
    if exit_code is not None and exit_code != 0:
        return False

    lines = [line.strip() for line in output_text.splitlines() if line.strip()]
    if not lines:
        return False

    has_score = any(line == "=== score ===" for line in lines)
    has_explicit_fail = any(
        line.startswith("No flag yet:") or line.startswith("ABORT:") for line in lines
    )

    # Works even when FLAG is redacted and does not contain a CTF token.
    if has_score and not has_explicit_fail:
        return True

    return any(("bctf{" in line) or ("flag{" in line) or ("ctf{" in line) for line in lines)


def metric_value(metrics: Dict[str, object], key: str, default: float) -> float:
    v = metrics.get(key)
    if isinstance(v, (int, float)):
        return float(v)
    return default


def rank_attempt(metrics: Dict[str, object]) -> float:
    """Lower is better; 0 means all parsed constraints satisfied."""
    qber = metric_value(metrics, "qber", 1.0)
    max_qber = metric_value(metrics, "max_qber", 0.25)
    qber_checked = metric_value(metrics, "qber_checked", 0.0)
    min_qber_checked = metric_value(metrics, "min_qber_checked", 8.0)
    qber_checked_z = metric_value(metrics, "qber_checked_z", 0.0)
    qber_checked_x = metric_value(metrics, "qber_checked_x", 0.0)
    min_per_basis = metric_value(metrics, "min_qber_checked_per_basis", 4.0)
    info_rounds = metric_value(metrics, "info_rounds", 0.0)
    min_info_rounds = metric_value(metrics, "min_info_rounds", 40.0)
    mi = metric_value(metrics, "mutual_info_empirical_bits", 0.0)
    min_mi = metric_value(metrics, "min_mutual_info_bits", 0.75)
    frontier_dev = metric_value(metrics, "paper_frontier_abs_dev_bits", 1.0)
    max_frontier_dev = metric_value(metrics, "max_frontier_abs_dev_bits", 0.1)
    announce_balance = metric_value(metrics, "announce_balance", 1.0)
    delayed_advantage = metric_value(metrics, "delayed_advantage_bits", -1.0)

    penalty = 0.0
    penalty += max(0.0, qber - max_qber) * 10.0
    penalty += max(0.0, min_qber_checked - qber_checked) * 0.5
    penalty += max(0.0, min_per_basis - qber_checked_z) * 0.8
    penalty += max(0.0, min_per_basis - qber_checked_x) * 0.8
    penalty += max(0.0, min_info_rounds - info_rounds) * 0.1
    penalty += max(0.0, min_mi - mi) * 8.0
    penalty += max(0.0, frontier_dev - max_frontier_dev) * 8.0
    penalty += max(0.0, announce_balance - 0.35) * 4.0
    penalty += max(0.0, -delayed_advantage) * 6.0
    return penalty


def run_one_attempt_from_cmd(
    cmd: List[str],
    announce_seed: int,
    io_timeout: float,
    debug: bool,
    debug_tag: str,
) -> Dict[str, object]:
    proc = subprocess.Popen(
        cmd,
        stdin=subprocess.PIPE,
        stdout=subprocess.PIPE,
        stderr=subprocess.STDOUT,
        text=True,
        bufsize=1,
    )

    assert proc.stdin is not None
    assert proc.stdout is not None

    # Read stdout lines in a dedicated thread to avoid select/TextIO buffering issues.
    line_queue: "queue.Queue[Optional[str]]" = queue.Queue()

    def _reader() -> None:
        try:
            for out_line in proc.stdout:
                line_queue.put(out_line)
        finally:
            line_queue.put(None)

    reader_thread = threading.Thread(target=_reader, daemon=True)
    reader_thread.start()

    ancilla = ["1+0j", "0+0j"]
    unitary = build_cnot_a_to_c_user_matrix()

    current_set = ""
    latest_outcomes: Dict[str, int] = {}
    rng = random.Random(announce_seed)
    announce_round = 0
    announce_counts = {"z": 0, "x": 0}
    lines: List[str] = []
    start = time.monotonic()
    timed_out = False
    saw_terminal_verdict = False

    def send_line(s: str) -> None:
        if debug:
            print(f"[debug {debug_tag}] >>> {s}", file=sys.stderr, flush=True)
        proc.stdin.write(s + "\n")
        proc.stdin.flush()

    while True:
        if io_timeout > 0 and (time.monotonic() - start > io_timeout):
            timed_out = True
            lines.append(f"[solver] timeout after {io_timeout:.1f}s\n")
            break

        try:
            line = line_queue.get(timeout=0.25)
        except queue.Empty:
            if proc.poll() is not None and line_queue.empty():
                break
            continue

        if line is None:
            break

        lines.append(line)
        stripped = line.strip()
        if debug:
            print(f"[debug {debug_tag}] <<< {stripped}", file=sys.stderr, flush=True)

        if (
            stripped.startswith("No flag yet:")
            or stripped.startswith("ABORT:")
            or ("bctf{" in stripped)
            or ("flag{" in stripped)
            or ("ctf{" in stripped)
        ):
            saw_terminal_verdict = True
            break

        if stripped == "ancilla_statevector_json>":
            send_line(json.dumps(ancilla))
            continue

        if stripped == "unitary_8x8_json>":
            send_line(json.dumps(unitary))
            continue

        if stripped.startswith("public_set:"):
            if "S0" in stripped:
                current_set = "S0"
            elif "S1" in stripped:
                current_set = "S1"
            continue

        if stripped == "pre_public_announce_json>":
            announce_round += 1
            announce_basis = "z" if (announce_round % 2 == 0) else "x"
            announce_counts[announce_basis] += 1
            announce = {
                # Paper model uses random test-basis choices; deterministic alternation
                # keeps challenge-side announce balance safely within threshold.
                "basis": announce_basis,
                "outcome": rng.randint(0, 1),
            }
            send_line(json.dumps(announce))
            continue

        if stripped == "measurement_plan_json>":
            if current_set == "S0":
                plan = [["b", "z"], ["c", "z"]]
            elif current_set == "S1":
                plan = [["b", "x"], ["c", "z"]]
            else:
                raise RuntimeError("Set info missing before measurement prompt")
            send_line(json.dumps(plan))
            continue

        if stripped.startswith("measurement_outcomes:"):
            latest_outcomes = parse_measurement_outcomes(stripped)
            continue

        if stripped == "secret_guess_bit>":
            b_bit = latest_outcomes["b"]
            c_bit = latest_outcomes["c"]
            parity = b_bit ^ c_bit

            if current_set == "S0":
                guess = parity
            elif current_set == "S1":
                guess = 1 ^ parity
            else:
                raise RuntimeError("Set info missing before guess prompt")

            send_line(str(guess))
            continue

    if proc.poll() is None:
        if timed_out:
            proc.kill()
        elif saw_terminal_verdict:
            proc.terminate()

    try:
        proc.wait(timeout=1.0 if saw_terminal_verdict else None)
    except subprocess.TimeoutExpired:
        proc.kill()
        proc.wait()
    reader_thread.join(timeout=0.5)
    metrics = parse_score_metrics(lines)
    announce_total = announce_counts["z"] + announce_counts["x"]
    if announce_total:
        metrics["announce_balance"] = (
            abs(announce_counts["z"] - announce_counts["x"]) / announce_total
        )
    else:
        metrics["announce_balance"] = 1.0
    return {
        "exit_code": proc.returncode,
        "timed_out": timed_out,
        "output": "".join(lines),
        "announce_z": announce_counts["z"],
        "announce_x": announce_counts["x"],
        **metrics,
    }


def run_one_local_attempt_with_id(
    challenge_path: str, attempt: int, io_timeout: float
) -> Dict[str, object]:
    announce_seed = attempt * 7919
    if io_timeout > 0:
        res = run_one_attempt_from_cmd(
            cmd=[sys.executable, challenge_path],
            announce_seed=announce_seed,
            io_timeout=io_timeout,
            debug=False,
            debug_tag=f"local-{attempt}",
        )
    else:
        # Match local.py behavior for best stability in local optimization phase.
        cmd = [sys.executable, challenge_path]
        proc = subprocess.Popen(
            cmd,
            stdin=subprocess.PIPE,
            stdout=subprocess.PIPE,
            stderr=subprocess.STDOUT,
            text=True,
            bufsize=1,
        )

        assert proc.stdin is not None
        assert proc.stdout is not None

        ancilla = ["1+0j", "0+0j"]
        unitary = build_cnot_a_to_c_user_matrix()

        current_set = ""
        latest_outcomes: Dict[str, int] = {}
        rng = random.Random(announce_seed)
        announce_round = 0
        announce_counts = {"z": 0, "x": 0}
        lines: List[str] = []

        def send_line(s: str) -> None:
            proc.stdin.write(s + "\n")
            proc.stdin.flush()

        while True:
            line = proc.stdout.readline()
            if line == "":
                break

            lines.append(line)
            stripped = line.strip()

            if stripped == "ancilla_statevector_json>":
                send_line(json.dumps(ancilla))
                continue

            if stripped == "unitary_8x8_json>":
                send_line(json.dumps(unitary))
                continue

            if stripped.startswith("public_set:"):
                if "S0" in stripped:
                    current_set = "S0"
                elif "S1" in stripped:
                    current_set = "S1"
                continue

            if stripped == "pre_public_announce_json>":
                announce_round += 1
                announce_basis = "z" if (announce_round % 2 == 0) else "x"
                announce_counts[announce_basis] += 1
                announce = {
                    "basis": announce_basis,
                    "outcome": rng.randint(0, 1),
                }
                send_line(json.dumps(announce))
                continue

            if stripped == "measurement_plan_json>":
                if current_set == "S0":
                    plan = [["b", "z"], ["c", "z"]]
                elif current_set == "S1":
                    plan = [["b", "x"], ["c", "z"]]
                else:
                    raise RuntimeError("Set info missing before measurement prompt")
                send_line(json.dumps(plan))
                continue

            if stripped.startswith("measurement_outcomes:"):
                latest_outcomes = parse_measurement_outcomes(stripped)
                continue

            if stripped == "secret_guess_bit>":
                b_bit = latest_outcomes["b"]
                c_bit = latest_outcomes["c"]
                parity = b_bit ^ c_bit

                if current_set == "S0":
                    guess = parity
                elif current_set == "S1":
                    guess = 1 ^ parity
                else:
                    raise RuntimeError("Set info missing before guess prompt")

                send_line(str(guess))
                continue

        proc.wait()
        metrics = parse_score_metrics(lines)
        announce_total = announce_counts["z"] + announce_counts["x"]
        if announce_total:
            metrics["announce_balance"] = (
                abs(announce_counts["z"] - announce_counts["x"]) / announce_total
            )
        else:
            metrics["announce_balance"] = 1.0

        res = {
            "exit_code": proc.returncode,
            "timed_out": False,
            "output": "".join(lines),
            "announce_z": announce_counts["z"],
            "announce_x": announce_counts["x"],
            **metrics,
        }

    res["attempt"] = attempt
    res["announce_seed"] = announce_seed
    return res


def run_one_remote_attempt(
    host: str,
    port: int,
    use_ssl: bool,
    announce_seed: int,
    io_timeout: float,
    debug: bool,
    remote_attempt: int,
) -> Dict[str, object]:
    cmd = ["ncat"]
    if use_ssl:
        cmd.append("--ssl")
    if io_timeout > 0:
        timeout_secs = max(2, int(math.ceil(io_timeout)))
        cmd.extend(["-w", f"{timeout_secs}s"])
    cmd.extend([host, str(port)])
    res = run_one_attempt_from_cmd(
        cmd=cmd,
        announce_seed=announce_seed,
        io_timeout=io_timeout,
        debug=debug,
        debug_tag=f"remote-{remote_attempt}",
    )
    res["remote_attempt"] = remote_attempt
    res["announce_seed"] = announce_seed
    return res


def paper_optimality_distance(metrics: Dict[str, object]) -> float:
    """Lower is better; 0 means ideal paper edge point was reached.

    Target point under challenge constraints is the paper edge:
    - QBER close to MAX_QBER from below (D -> 1/4)
    - Mutual information close to 1 bit
    - Small deviation from paper frontier relation
    """
    qber = metric_value(metrics, "qber", 1.0)
    max_qber = metric_value(metrics, "max_qber", 0.25)
    mi = metric_value(metrics, "mutual_info_empirical_bits", 0.0)
    frontier_dev = metric_value(metrics, "paper_frontier_abs_dev_bits", 1.0)
    delayed_advantage = metric_value(metrics, "delayed_advantage_bits", -1.0)

    # For passing runs qber <= max_qber; paper edge is D = 1/4 and I = 1.
    qber_edge_gap = max(0.0, min(max_qber, PAPER_EDGE_QBER) - qber)
    mi_gap = abs(PAPER_EDGE_MI - mi)
    advantage_gap = max(0.0, -delayed_advantage)

    # Weight edge-closeness highest, then MI saturation, then frontier tightness.
    return 6.0 * qber_edge_gap + 3.0 * mi_gap + 1.5 * frontier_dev + 2.0 * advantage_gap


def main() -> None:
    parser = argparse.ArgumentParser(description="KKI solver with local optimization + remote replay")
    parser.add_argument("--max-attempts", type=int, default=500)
    parser.add_argument("--workers", type=int, default=max(1, (os.cpu_count() or 1)))
    parser.add_argument("--remote-host", default="localhost")
    parser.add_argument("--remote-port", type=int, default=1337)
    parser.add_argument("--remote-attempts", type=int, default=200)
    parser.add_argument("--remote-ssl", action="store_true")
    parser.add_argument(
        "--local-io-timeout",
        type=float,
        default=0.0,
        help="Per-attempt timeout for local optimization. 0 disables timeout (local.py behavior).",
    )
    parser.add_argument(
        "--remote-io-timeout",
        type=float,
        default=600.0,
        help="Per-attempt timeout for remote ncat connections.",
    )
    parser.add_argument("--progress-every", type=int, default=1)
    parser.add_argument("--debug", action="store_true")
    parser.add_argument(
        "--paper-stop-distance",
        type=float,
        default=0.05,
        help="Stop early if a passing run is this close to the paper-optimal edge",
    )
    args = parser.parse_args()

    if args.remote_ssl and args.remote_host in {"localhost", "127.0.0.1", "::1"}:
        print(
            "[solver] warning: --remote-ssl with localhost usually fails; "
            "disabling SSL for local target",
            flush=True,
        )
        args.remote_ssl = False

    script_dir = os.path.dirname(os.path.abspath(__file__))
    challenge_path = os.path.join(script_dir, "challenge.py")

    print(
        f"[solver] starting local phase: attempts={args.max_attempts} workers={args.workers} "
        f"timeout={'none' if args.local_io_timeout <= 0 else f'{args.local_io_timeout}s'}",
        flush=True,
    )

    best_candidate = None
    best_penalty = float("inf")
    selected = None
    best_passing = None
    best_paper_distance = float("inf")
    completed = 0

    futures = {}
    with ProcessPoolExecutor(max_workers=args.workers) as executor:
        for attempt in range(1, args.max_attempts + 1):
            fut = executor.submit(
                run_one_local_attempt_with_id,
                challenge_path,
                attempt,
                args.local_io_timeout,
            )
            futures[fut] = attempt

        for fut in as_completed(futures):
            res = fut.result()
            attempt = int(res["attempt"])
            completed += 1

            if completed % max(1, args.progress_every) == 0:
                print(
                    f"[solver] local progress={completed}/{args.max_attempts} "
                    f"attempt={attempt} exit={res.get('exit_code')} "
                    f"timed_out={bool(res.get('timed_out'))}",
                    flush=True,
                )

            # New challenge prints the flag only when all paper-style conditions pass.
            output_text = str(res.get("output", ""))
            passed = is_passing_output(
                output_text,
                timed_out=bool(res.get("timed_out")),
                exit_code=res.get("exit_code") if isinstance(res.get("exit_code"), int) else None,
            )
            if passed:
                paper_distance = paper_optimality_distance(res)
                if paper_distance < best_paper_distance:
                    best_paper_distance = paper_distance
                    best_passing = res

                # Keep searching for a more paper-optimal run unless already near the edge.
                if paper_distance <= args.paper_stop_distance:
                    selected = res
                    break
                continue

            penalty = rank_attempt(res)
            if penalty < best_penalty:
                best_penalty = penalty
                best_candidate = res

            if completed % 10 == 0:
                if best_candidate is None:
                    print(f"[solver] completed {completed}: no viable run yet")
                else:
                    bq = metric_value(best_candidate, "qber", 1.0)
                    bmi = metric_value(best_candidate, "mutual_info_empirical_bits", 0.0)
                    bfd = metric_value(best_candidate, "paper_frontier_abs_dev_bits", 1.0)
                    bz = metric_value(best_candidate, "qber_checked_z", 0.0)
                    bx = metric_value(best_candidate, "qber_checked_x", 0.0)
                    print(
                        f"[solver] completed {completed}: best so far qber={bq:.4f} "
                        f"mi={bmi:.4f} frontier_dev={bfd:.4f} checks(z/x)={int(bz)}/{int(bx)} "
                        f"penalty={best_penalty:.4f}"
                    )

                if best_passing is not None:
                    pq = metric_value(best_passing, "qber", 1.0)
                    pmi = metric_value(best_passing, "mutual_info_empirical_bits", 0.0)
                    pfd = metric_value(best_passing, "paper_frontier_abs_dev_bits", 1.0)
                    print(
                        f"[solver] completed {completed}: best passing qber={pq:.4f} "
                        f"mi={pmi:.4f} frontier_dev={pfd:.4f} "
                        f"paper_distance={best_paper_distance:.4f}"
                    )

        if selected is not None:
            # Best-effort cancellation of work not yet started.
            for pending in futures:
                pending.cancel()

    chosen = selected if selected is not None else best_passing
    if chosen is None:
        print(
            f"[solver] no locally passing run within {args.max_attempts} attempts; "
            f"increase --max-attempts",
            flush=True,
        )
        return

    local_seed = int(chosen.get("announce_seed", 0))
    local_attempt = int(chosen.get("attempt", -1))
    local_qber = metric_value(chosen, "qber", -1.0)
    local_mi = metric_value(chosen, "mutual_info_empirical_bits", -1.0)
    local_dev = metric_value(chosen, "paper_frontier_abs_dev_bits", -1.0)
    print(
        f"[solver] local-selected attempt={local_attempt} seed={local_seed} "
        f"qber={local_qber:.4f} mi={local_mi:.4f} frontier_dev={local_dev:.4f}"
        ,
        flush=True,
    )

    print(
        f"[solver] starting remote phase: host={args.remote_host}:{args.remote_port} "
        f"ssl={args.remote_ssl} attempts={args.remote_attempts}",
        flush=True,
    )

    best_remote: Optional[Dict[str, object]] = None
    best_remote_penalty = float("inf")

    for remote_i in range(1, args.remote_attempts + 1):
        if remote_i == 1 or remote_i % max(1, args.progress_every) == 0:
            print(
                f"[solver] remote attempt={remote_i}/{args.remote_attempts} "
                f"seed={local_seed}",
                flush=True,
            )

        res = run_one_remote_attempt(
            host=args.remote_host,
            port=args.remote_port,
            use_ssl=args.remote_ssl,
            announce_seed=local_seed,
            io_timeout=args.remote_io_timeout,
            debug=args.debug,
            remote_attempt=remote_i,
        )

        output_text = str(res.get("output", ""))
        passed = is_passing_output(
            output_text,
            timed_out=bool(res.get("timed_out")),
            exit_code=res.get("exit_code") if isinstance(res.get("exit_code"), int) else None,
        )
        if passed:
            print(output_text, end="")
            print(
                f"[solver] remote success on attempt={remote_i} using seed={local_seed} "
                f"host={args.remote_host}:{args.remote_port}"
                ,
                flush=True,
            )
            return

        penalty = rank_attempt(res)
        if penalty < best_remote_penalty:
            best_remote_penalty = penalty
            best_remote = res

        if remote_i % 5 == 0:
            rq = metric_value(res, "qber", 1.0)
            rmi = metric_value(res, "mutual_info_empirical_bits", 0.0)
            rfd = metric_value(res, "paper_frontier_abs_dev_bits", 1.0)
            print(
                f"[solver] remote tries={remote_i}/{args.remote_attempts} "
                f"last(qber={rq:.4f}, mi={rmi:.4f}, frontier_dev={rfd:.4f}) "
                f"best_penalty={best_remote_penalty:.4f}"
                ,
                flush=True,
            )

    print(
        f"[solver] remote phase ended without flag after {args.remote_attempts} attempts "
        f"using local seed={local_seed}"
        ,
        flush=True,
    )
    if best_remote is not None:
        print(best_remote["output"], end="")


if __name__ == "__main__":
    main()
