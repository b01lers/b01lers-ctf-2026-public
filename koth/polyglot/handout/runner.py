#!/usr/bin/python3

import os
import random
import shutil
import subprocess
import sys
import tomllib

N = 50
RANGES = {"a": (2**8, 2**16-1), "b": (2**6, 2**8-1), "c": (2**10, 2**12-1)}

def gen():
    cases = []
    for _ in range(N):
        a = random.randint(*RANGES["a"])
        b = random.randint(*RANGES["b"])
        c = random.randint(*RANGES["c"])
        cases.append((f"{a}\n{b}\n{c}\n", str(pow(a, b, c))))
    return cases

def main():
    if len(sys.argv) != 3:
        print(f"usage: {sys.argv[0]} <langs> <polyglot.bin>")
        print(f"  langs: comma-separated list of languages (e.g. bash,lua,c)")
        sys.exit(1)

    languages = [l.strip() for l in sys.argv[1].split(",") if l.strip()]
    path = sys.argv[2]

    if not os.path.exists(path):
        print(f"file not found: {path}")
        sys.exit(1)

    with open(path, "rb") as f:
        code = f.read()

    with open("/app/languages.toml", "rb") as f:
        config = tomllib.load(f)

    cases = gen()

    for lang in languages:
        if lang not in config:
            print(f"unknown language: {lang}")
            sys.exit(1)

        print(f"--- {lang} ---")

        lang_config = config[lang]
        runner_conf = lang_config.get("runner", {})
        compile_conf = lang_config.get("compile", {})

        filename = compile_conf.get("filename", runner_conf.get("filename", "script"))
        script_path = os.path.join("/tmp", filename)

        with open(script_path, "wb") as f:
            f.write(code)

        def make_env(env_list):
            env = os.environ.copy()
            for e in env_list:
                if "=" in e:
                    k, v = e.split("=", 1)
                    env[k] = v
            return env

        os.makedirs("/out", exist_ok=True)
        for f in os.listdir("/out"):
            p = os.path.join("/out", f)
            if os.path.isfile(p):
                os.remove(p)

        binary_name = compile_conf.get("binary_name", runner_conf.get("binary_name", "prog"))

        # compile
        if compile_conf:
            cmd = compile_conf["cmd"]
            env = make_env(compile_conf.get("extra_env", []))
            timeout = compile_conf.get("timeout", 30)
            try:
                res = subprocess.run(cmd, capture_output=True, text=True, env=env, timeout=timeout)
            except subprocess.TimeoutExpired:
                print(f"  compilation timed out ({timeout}s)")
                continue
            if res.returncode != 0:
                print(f"  compilation failed:")
                print(f"  {res.stderr.strip()}")
                continue

            out_bin = os.path.join("/out", binary_name)
            tmp_bin = os.path.join("/tmp", binary_name)
            if os.path.exists(out_bin):
                shutil.copy(out_bin, tmp_bin)

        # run
        cmd = runner_conf["cmd"]
        env = make_env(runner_conf.get("extra_env", []))
        timeout = runner_conf.get("timeout", 10)
        passed = 0

        for i, (inp, expected) in enumerate(cases):
            try:
                res = subprocess.run(cmd, input=inp, capture_output=True, text=True, env=env, timeout=timeout)
            except subprocess.TimeoutExpired:
                print(f"  FAIL #{i+1}: timed out ({timeout}s)")
                break

            actual = res.stdout.strip()

            if res.returncode != 0:
                print(f"  FAIL #{i+1}: exit code {res.returncode}")
                print(f"    stderr: {res.stderr.strip()}")
                break

            if actual == expected:
                passed += 1
            else:
                a, b, c = inp.strip().split("\n")
                print(f"  FAIL #{i+1}: pow({a}, {b}, {c})")
                print(f"    expected: {expected}")
                print(f"    got:      {actual!r}")
                break

        if passed == N:
            print(f"  passed all {N} tests")
        else:
            print(f"  {passed}/{N} passed")
        print()

    sys.exit(0)

if __name__ == "__main__":
    main()
