import asyncio
import base64
import binascii
from json import dumps, loads, JSONDecodeError
from websockets.asyncio.server import serve, ServerConnection
from websockets.exceptions import WebSocketException
import traceback
from time import time
from random import randint
import sys

if sys.version_info >= (3, 14):
    from compression import zstd
else:
    from backports import zstd


async def do_runner_request(conn, data, payload):
    await conn.send(dumps({"type": "test", "id": data.get("id"), "team": data.get("team")}))

    max_base = 32767  # also min modulus
    max_exponent = 32767
    max_modulus = 65535
    tests_results = []
    timings = []

    for i in range(5):
        base = randint(2, max_base)
        exponent = randint(1, max_exponent)
        modulus = randint(max_base+1, max_modulus)
        test_case = str([base, exponent, modulus])
        print(f'test #{i+1}', base, exponent, modulus)
        before_time = time()

        try:
            proc = await asyncio.create_subprocess_exec(
                "nc",
                "jail",
                "6969",
                stdin=asyncio.subprocess.PIPE,
                stdout=asyncio.subprocess.PIPE,
                stderr=asyncio.subprocess.PIPE,
            )
            stdout, stderr = await asyncio.wait_for(
                proc.communicate(payload.hex().encode('utf8') + b'\n' + test_case.encode("utf8") + b'\n'),
                timeout=100,
            )
        except asyncio.TimeoutError:
            print(f"timeout of 100s exceeded (id={data.get('id')!r})")
            await conn.send(dumps({
                "type": "completed", "id": data.get("id"), "team": data.get("team"),
                "score": [0, 0],
                "tests": tests_results,
                "error": "timeout of 100s exceeded"
            }))
            return
        except Exception:
            print(f"error in communication somehow (id={data.get('id')!r})")
            print(traceback.format_exc())
            await conn.send(dumps({
                "type": "completed", "id": data.get("id"), "team": data.get("team"),
                "score": [0, 0],
                "tests": tests_results,
                "error": f"error in communication somehow: {traceback.format_exc()}"
            }))
            return

        if proc.returncode != 0:
            print(f"evaluation error (returncode={proc.returncode}, stderr={stderr}) (id={data.get('id')})")
            await conn.send(dumps({
                "type": "completed", "id": data.get("id"), "team": data.get("team"),
                "score": [0, 0],
                "tests": tests_results,
                "error": f"evaluation error (returncode={proc.returncode}, stdout={stdout}, stderr={stderr})"
            }))
            return

        try:
            response = stdout.decode("l1")
            result_int = int(response.removeprefix("pickelang > "))
        except ValueError:
            print(f"output not an integer (id={data.get('id')})")
            await conn.send(dumps({
                "type": "completed", "id": data.get("id"), "team": data.get("team"),
                "score": [0, 0],
                "tests": tests_results,
                "error": f"evaluation error (returncode={proc.returncode}, stdout={stdout}, stderr={stderr})"
            }))
            return

        timings.append(time() - before_time)
        tests_results.append([
            test_case,
            (f"== duration ==\n{time() - before_time}\n== correct result ==\n"
            + f"{pow(base, exponent, modulus)}\n== your result ==\n{result_int}\n")
        ])

        if result_int != pow(base, exponent, modulus):
            print(f"incorrect result #{i+1} (id={data.get('id')}) ({time() - before_time:.03}s)")
            await conn.send(dumps({
                "type": "completed", "id": data.get("id"), "team": data.get("team"),
                "score": [0, 0],
                "tests": tests_results,
                "error": f"incorrect result (yours={result_int}, correct={pow(base, exponent, modulus)})"
            }))
            return
        print(f"correct result #{i+1} (id={data.get('id')}) ({time() - before_time:.03}s)")

    await conn.send(dumps({
        "type": "completed", "id": data.get("id"), "team": data.get("team"),
        "score": [-len(payload), -(round(sorted(timings)[len(timings)//2]))],
        "tests": tests_results,
    }))


async def runner_req(conn, msg):
    try:
        data = loads(msg)
    except JSONDecodeError:
        print("json decode error")
        return
    if not isinstance(data.get("id"), str):
        print("id not str")
        return
    if not isinstance(data.get("team"), str):
        print("team not str")
        return
    if not isinstance(data.get("payload"), str):
        print("payload not str")
        return
    print(f"runner request received (team={data.get('team')!r}, id={data.get('id')!r})")
    try:
        decoded = base64.b64decode(data.get("payload"))
    except binascii.Error:
        print("base64 error")
        return
    try:
        payload = zstd.decompress(decoded)
    except Exception as e:
        print('zstd error')
        return
    # useless ass queue message apparently its ignored on backend :(
    await conn.send(dumps({"type": "queue", "id": data.get("id"), "team": data.get("team")}))
    await do_runner_request(conn, data, payload)


async def handle_conn(conn: ServerConnection):
    print("websocket connection initiated with", conn.remote_address)
    semaphore = asyncio.Semaphore(4)

    async def semaphore_runner_req(msg):
        async with semaphore:
            await runner_req(conn, msg)

    while True:
        try:
            msg = await conn.recv(True)
            asyncio.create_task(semaphore_runner_req(msg))
        except WebSocketException as e:
            print("websocket exception:", e)
            exit(1)


async def main():
    print("server started")
    async with serve(handle_conn, "0.0.0.0", 5000) as server:
        await server.serve_forever()

if __name__ == "__main__":
    asyncio.run(main())

