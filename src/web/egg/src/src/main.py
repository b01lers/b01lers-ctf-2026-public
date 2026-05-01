import json
import random
import sys
import time
from dataclasses import dataclass
from datetime import datetime
from pathlib import Path
from urllib.parse import parse_qs
from uuid import UUID, uuid4

import db
import uvicorn
from hatchery import Hatchery
from palette import EGG_PALETTES
from starlette.applications import Starlette
from starlette.middleware.sessions import SessionMiddleware
from starlette.requests import Request
from starlette.responses import RedirectResponse
from starlette.routing import Route
from starlette.templating import Jinja2Templates

BASE_DIR = Path(__file__).resolve().parent
PROJECT_DIR = BASE_DIR.parent
TEMPLATE_DIR = PROJECT_DIR / "templates"
SESSION_KEY = "player_id"
SESSION_TIMESTAMP = "session_timestamp"
SESSION_SECRET = "6767676767676767"
MAX_EGGS = 8
HATCHERY_FILENAME = "asciiart"
templates = Jinja2Templates(directory=str(TEMPLATE_DIR))


@dataclass
class PlayerState:
    player_id: str
    username: str
    hatchery: Hatchery
    egg_palettes: dict[str, dict[str, str]]
    flash: str | None = None
    last_hatch: str | None = None


def redirect_home() -> RedirectResponse:
    return RedirectResponse("/", status_code=303)


def cleanup_player_eggs(player_id: str) -> None:
    for row in db.get_player_eggs(player_id):
        egg_path = Path("incubator") / f"{row['egg_id']}.egg"
        if egg_path.exists():
            egg_path.unlink()


def merge(dst: dict, src: dict) -> dict:
    for key, value in src.items():
        if key in dst and isinstance(dst[key], dict) and isinstance(value, dict):
            merge(dst[key], value)
        else:
            dst[key] = value
    return dst


def current_player(request: Request) -> PlayerState | None:
    player_id = request.session.get(SESSION_KEY)
    if not player_id:
        return None

    player_row = db.get_player(player_id)
    if player_row is None:
        return None

    hatchery = Hatchery(UUID(player_id))
    egg_palettes: dict[str, dict[str, str]] = {}
    for row in db.get_player_eggs(player_id):
        egg_id = row["egg_id"]
        created_at = datetime.fromisoformat(row["created_at"])
        hatchery.add_existing_egg(egg_id, row["name"], created_at)
        egg_palettes[row["egg_id"]] = json.loads(row["palette_json"])

    return PlayerState(
        player_id=player_id,
        username=player_row["username"],
        hatchery=hatchery,
        egg_palettes=egg_palettes,
        flash=player_row["flash"],
        last_hatch=player_row["last_hatch"],
    )


def normalize_username(raw_username: str) -> str:
    cleaned = "".join(ch for ch in raw_username if 32 <= ord(ch) < 127).strip()
    return cleaned[:24]


def pop_flash(player: PlayerState) -> str | None:
    message = player.flash
    player.flash = None
    return message


async def parse_form(request: Request) -> dict[str, str]:
    body = await request.body()
    data = parse_qs(body.decode("utf-8"), keep_blank_values=True)
    return {key: values[-1] for key, values in data.items()}


def build_egg_palette() -> dict[str, str]:
    return random.choice(EGG_PALETTES).copy()


def render_login(request: Request, message: str | None = None):
    return templates.TemplateResponse(
        request,
        "login.html",
        {
            "message": message,
        },
    )


def render_dashboard(request: Request, player: PlayerState):
    eggs = sorted(player.hatchery.eggs.items(), key=lambda item: item[1][1])
    now = datetime.now()
    egg_cards: list[dict[str, object]] = []

    for index in range(MAX_EGGS):
        if index < len(eggs):
            egg_id, (egg, created_at) = eggs[index]
            warm_seconds = max(0, int((now - created_at).total_seconds()))
            egg_id_str = str(egg_id)
            palette = player.egg_palettes.setdefault(egg_id_str, build_egg_palette())
            egg_cards.append(
                {
                    "slot": index + 1,
                    "warm_seconds": warm_seconds,
                    "name": egg.name,
                    "egg_id": egg_id_str,
                    "palette": palette,
                    "is_empty": False,
                }
            )
        else:
            egg_cards.append(
                {
                    "slot": index + 1,
                    "warm_seconds": None,
                    "name": None,
                    "egg_id": None,
                    "palette": None,
                    "is_empty": True,
                }
            )

    flash = pop_flash(player)
    if flash is not None:
        db.set_player_flash(player.player_id, None)
    return templates.TemplateResponse(
        request,
        "dashboard.html",
        {
            "username": player.username,
            "egg_count": len(eggs),
            "max_eggs": MAX_EGGS,
            "hatchery_filename": HATCHERY_FILENAME,
            "summon_disabled": len(eggs) >= MAX_EGGS,
            "flash": flash,
            "egg_cards": egg_cards,
            "last_hatch": player.last_hatch,
        },
    )


async def homepage(request: Request):
    player = current_player(request)
    if player is None:
        return render_login(request)
    return render_dashboard(request, player)


async def login(request: Request):
    form = await parse_form(request)
    username = normalize_username(form.get("username", ""))
    if not username:
        response = render_login(request, "Pick a username to unlock the hatchery.")
        response.status_code = 400
        return response

    existing_token = request.session.get(SESSION_KEY)
    if existing_token:
        cleanup_player_eggs(existing_token)
        db.delete_player(existing_token)
        request.session.clear()

    session_token = str(uuid4())
    db.create_player(session_token, username, flash="Successfully logged in.")

    request.session[SESSION_KEY] = session_token
    request.session[SESSION_TIMESTAMP] = time.time()
    return redirect_home()


async def logout(request: Request) -> RedirectResponse:
    session_token = request.session.get(SESSION_KEY)
    if session_token:
        cleanup_player_eggs(session_token)
        db.delete_player(session_token)

    request.session.clear()
    return redirect_home()


async def create_egg(request: Request) -> RedirectResponse:
    player = current_player(request)
    if player is None:
        return redirect_home()
    data = merge(
        {
            "filename": HATCHERY_FILENAME,
        },
        await request.json(),
    )
    if not data.get("name"):
        return redirect_home()

    name = str(data["name"])
    filename = str(data["filename"])
    time = request.session[SESSION_TIMESTAMP]

    if len(player.hatchery.eggs) >= MAX_EGGS:
        db.set_player_flash(
            player.player_id,
            "Incubator cap reached. Hatch something before summoning more.",
        )
        return redirect_home()
    try:
        egg_id = player.hatchery.incubate(name, filename, time)
    except Exception as exc:
        db.set_player_flash(player.player_id, f"Failed: {type(exc).__name__}")
    else:
        palette = build_egg_palette()
        db.add_egg(
            player.player_id,
            str(egg_id),
            name,
            player.hatchery.eggs[egg_id][1].isoformat(),
            palette,
        )
        db.set_player_flash(player.player_id, f"{name} incubating...")

    return redirect_home()


async def hatch_egg(request: Request) -> RedirectResponse:
    data = await request.json()
    art_index = int(data.get("art_index", 0))

    player = current_player(request)
    if player is None:
        return redirect_home()

    try:
        egg_id = request.path_params["egg_id"]
    except ValueError:
        db.set_player_flash(player.player_id, "Failed: Not a valid egg to hatch.")
        return redirect_home()

    if egg_id not in player.hatchery.eggs:
        db.set_player_flash(player.player_id, "Failed: That egg already hatched.")
        return redirect_home()
    try:
        sys.modules.pop("creature", None)
        try:
            last_hatch = player.hatchery.hatch(egg_id, art_index=art_index)
        finally:
            sys.modules.pop("creature", None)
            player.hatchery.eggs.pop(egg_id, None)
            player.egg_palettes.pop(str(egg_id), None)
    except Exception as exc:
        db.set_player_last_hatch(player.player_id, None)
        db.set_player_flash(player.player_id, f"Failed: {type(exc).__name__}")
    else:
        db.delete_egg(str(egg_id))
        db.set_player_last_hatch(player.player_id, last_hatch)
        db.set_player_flash(player.player_id, "You hatched an egg!")

    return redirect_home()


app = Starlette(
    routes=[
        Route("/", homepage),
        Route("/login", login, methods=["POST"]),
        Route("/logout", logout, methods=["POST"]),
        Route("/eggs", create_egg, methods=["POST"]),
        Route("/eggs/{egg_id:str}/hatch", hatch_egg, methods=["POST"]),
    ]
)
app.add_middleware(SessionMiddleware, secret_key=SESSION_SECRET, same_site="lax")


if __name__ == "__main__":
    db.init_db()
    FLAG = sys.argv[1]
    uvicorn.run("main:app", host="0.0.0.0", port=8000, reload=False, workers=4)
