#!/usr/local/bin/python3 -u

import base64
import json
import subprocess
import sys
import zstd


BFCI = "/usr/local/bin/bfci"
SCRIPT = "/tmp/code"
TIMEOUT = 10  # seconds per testcase
MAX_STDOUT = 50
# i would add MAX_STDERR but i don't think it's relevant here

def print_json(obj):
    print(json.dumps(obj, separators=(",", ":")))


def truncate(s: str) -> str:
    return s[:MAX_STDOUT] + "..." if len(s) > MAX_STDOUT else s


def main():
    raw = sys.stdin.readline()
    if not raw.strip():
        return print_json({"err": "empty request", "results": None})

    try:
        req = json.loads(raw)
    except json.JSONDecodeError:
        return print_json({"err": "invalid JSON", "results": None})

    code_b64 = req.get("code", "")
    testcases = req.get("testcases")

    if not testcases or not isinstance(testcases, list):
        return print_json({"err": "missing or empty testcases", "results": None})

    try:
        code = zstd.decompress(base64.b64decode(code_b64))
    except Exception:
        return print_json({"err": "invalid base64/zstd compression", "results": None})

    with open(SCRIPT, "wb") as f:
        f.write(code)

    results = []
    for tc_input in testcases:
        try:
            proc = subprocess.run(
                [BFCI, "-x", SCRIPT],
                input=tc_input,
                capture_output=True,
                text=True,
                timeout=TIMEOUT,
            )
            results.append(
                {
                    "stdout": truncate(proc.stdout),
                    "stderr": proc.stderr,
                    "exit_code": proc.returncode,
                }
            )
        except subprocess.TimeoutExpired:
            return print_json({"err": "execution timed out", "results": None})

    print_json({"err": None, "results": results})


if __name__ == "__main__":
    main()
