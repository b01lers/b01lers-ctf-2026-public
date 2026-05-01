import asyncio
import base64
import json
import logging
import os
import random
import zstd

import websockets

WORKERS = int(os.environ.get("WORKERS", "1"))
WS_PORT = 5001

N = 50
RANGE_MAPPING = {
    "a": (10, 999),
    "b": (50, 100),
    "c": (100, 999),
}

log = logging.getLogger("brainfuck-outer")

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


async def run_inner(code_b64: str, testcases: list[dict]) -> tuple[dict | None, str | None]:
    """Returns (result_dict, None) on success or (None, error_string) on failure."""
    pkt = (
        json.dumps(
            {
                "code": code_b64,
                "testcases": [t["input"] for t in testcases],
            },
            separators=(",", ":"),
        )
        + "\n"
    )

    try:
        reader, writer = await asyncio.wait_for(
            asyncio.open_connection("inner", 8990, limit=16 * 1024 * 1024),
            timeout=10,
        )
    except asyncio.TimeoutError:
        return None, "Timed out connecting to runner"
    except Exception as e:
        return None, f"Error connecting to runner: {e}"

    try:
        writer.write(pkt.encode())
        await writer.drain()

        stdout = await asyncio.wait_for(reader.readline(), timeout=120)
    except asyncio.TimeoutError:
        return None, "2-minute timeout exceeded"
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

    if team in queued_teams:
        return await send_error(websocket, "team already in queue")

    queued_teams.add(team)
    await queue.put((submission_id, team, payload))

    log.info("Queued: id=%s team=%s", submission_id, team)
    await websocket.send(json.dumps({"type": "queue", "id": submission_id, "team": team}))


async def worker():
    log.info("Worker started")
    while True:
        submission_id, team, payload = await queue.get()
        log.info("Processing: id=%s team=%s", submission_id, team)

        await broadcast({"type": "test", "id": submission_id, "team": team})

        testcases = gen_testcases(N)

        try:
            result, error = await run_inner(payload, testcases)
        except Exception as e:
            log.exception("Unexpected error running inner for id=%s", submission_id)
            result, error = None, f"Internal error: {e}"

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
            if result.get("err"):
                await broadcast(
                    {
                        "type": "completed",
                        "id": submission_id,
                        "team": team,
                        "score": [0, 0],
                        "tests": [],
                        "error": result["err"],
                    }
                )
                queued_teams.discard(team)
                log.info("Completed: id=%s team=%s", submission_id, team)
                continue

            results = result.get("results", [])
            first_failure = None
            passed = 0


            reported_tests = [[t["input"], t["output"]] for t in testcases]

            for i, tr in enumerate(results):
                expected = testcases[i]["output"]
                actual = (tr.get("stdout") or "").strip()

                if tr.get("exit_code", -1) != 0:
                    if not first_failure:
                        stderr = (tr.get("stderr") or "").strip()
                        first_failure = (
                            f"testcase {i + 1}: non-zero exit code "
                            f"({tr.get('exit_code')}): {stderr}"
                        )
                        reported_tests = []
                        for j, tc in enumerate(testcases):
                            tc_actual = ""
                            if j < len(results):
                                tc_actual = (results[j].get("stdout") or "").strip()
                            reported_tests.append([tc["input"], tc_actual])
                    continue

                if actual == expected:
                    passed += 1
                else:
                    if not first_failure:
                        first_failure = (
                            f"testcase {i + 1} failed (expected {expected}, got {actual!r})"
                        )
                        reported_tests = []
                        for j, tc in enumerate(testcases):
                            tc_actual = ""
                            if j < len(results):
                                tc_actual = (results[j].get("stdout") or "").strip()
                            reported_tests.append([tc["input"], tc_actual])

            if first_failure:
                score = [0, 0]
            else:
                score = [-len(zstd.decompress(base64.b64decode(payload))), 0]

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

    async with websockets.serve(handle_connection, "0.0.0.0", WS_PORT):
        log.info("WebSocket server listening on :%d", WS_PORT)
        await asyncio.Future()


if __name__ == "__main__":
    logging.basicConfig(
        level=logging.INFO,
        format="%(asctime)s %(levelname)s %(message)s",
    )
    asyncio.run(main())
