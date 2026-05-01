import json
import sqlite3
from pathlib import Path

DB_PATH = Path("/tmp/egg_hatcher.sqlite3")


def connect() -> sqlite3.Connection:
    conn = sqlite3.connect(DB_PATH, timeout=30)
    conn.row_factory = sqlite3.Row
    return conn


def init_db() -> None:
    with connect() as conn:
        conn.execute("PRAGMA journal_mode=WAL")
        conn.execute(
            """
            CREATE TABLE IF NOT EXISTS players (
                player_id TEXT PRIMARY KEY,
                username TEXT NOT NULL,
                flash TEXT,
                last_hatch TEXT
            )
            """
        )
        conn.execute(
            """
            CREATE TABLE IF NOT EXISTS eggs (
                egg_id TEXT PRIMARY KEY,
                player_id TEXT NOT NULL,
                name TEXT NOT NULL,
                created_at TEXT NOT NULL,
                palette_json TEXT NOT NULL,
                FOREIGN KEY (player_id) REFERENCES players(player_id) ON DELETE CASCADE
            )
            """
        )


def create_player(player_id: str, username: str, flash: str | None = None) -> None:
    with connect() as conn:
        conn.execute(
            "INSERT OR REPLACE INTO players (player_id, username, flash, last_hatch) VALUES (?, ?, ?, NULL)",
            (player_id, username, flash),
        )


def get_player(player_id: str) -> sqlite3.Row | None:
    with connect() as conn:
        return conn.execute(
            "SELECT player_id, username, flash, last_hatch FROM players WHERE player_id = ?",
            (player_id,),
        ).fetchone()


def delete_player(player_id: str) -> None:
    with connect() as conn:
        conn.execute("DELETE FROM eggs WHERE player_id = ?", (player_id,))
        conn.execute("DELETE FROM players WHERE player_id = ?", (player_id,))


def set_player_flash(player_id: str, flash: str | None) -> None:
    with connect() as conn:
        conn.execute(
            "UPDATE players SET flash = ? WHERE player_id = ?", (flash, player_id)
        )


def set_player_last_hatch(player_id: str, last_hatch: str | None) -> None:
    with connect() as conn:
        conn.execute(
            "UPDATE players SET last_hatch = ? WHERE player_id = ?",
            (last_hatch, player_id),
        )


def add_egg(
    player_id: str,
    egg_id: str,
    name: str,
    created_at: str,
    palette: dict[str, str],
) -> None:
    with connect() as conn:
        conn.execute(
            "INSERT OR REPLACE INTO eggs (egg_id, player_id, name, created_at, palette_json) VALUES (?, ?, ?, ?, ?)",
            (egg_id, player_id, name, created_at, json.dumps(palette)),
        )


def delete_egg(egg_id: str) -> None:
    with connect() as conn:
        conn.execute("DELETE FROM eggs WHERE egg_id = ?", (egg_id,))


def get_player_eggs(player_id: str) -> list[sqlite3.Row]:
    with connect() as conn:
        return conn.execute(
            """
            SELECT egg_id, name, created_at, palette_json
            FROM eggs
            WHERE player_id = ?
            ORDER BY created_at ASC
            """,
            (player_id,),
        ).fetchall()
