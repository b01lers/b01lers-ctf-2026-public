import math
import os
import random
import shutil
import sys
import time
from datetime import datetime
from hashlib import sha256
from pathlib import Path
from uuid import UUID
from zipfile import ZIP_DEFLATED, ZipFile

EGG_TEMPLATE = """
class Creature:

    def __init__(self):
        self.name = "{name}"
        self.attack = 0x{attack:02x}
        self.defense = 0x{defense:02x}
        self.health = 0x{health:02x}
        self.stability = 0x{stability:02x}
        self.art = ""

    def get_ascii_art(self) -> str:
        return self.art

    def gen_ascii_art(self, offset: int) -> None:
        if not all(c.isascii() and c.isalnum() for c in "{filename}"):
            return None
        f = open("{filename}", "r")
        f.seek(offset)
        self.art = f.read(40)
"""

EGG_DIR = "incubator"
ART_LEN = 44


def is_valid_file(s: str) -> bool:
    return all(c.isascii() and c.isalnum() or c == "/" for c in s)


def is_valid_name(s: str) -> bool:
    return all(c.isascii() and c.isalnum() for c in s)


class Egg:
    def __init__(self, name: str, filename: str, player_creation_time: float):
        if not is_valid_name(name):
            return

        if not is_valid_file(filename):
            return

        time_diff = time.time() - player_creation_time
        if time_diff <= 0:
            return

        self.name = name
        self.id = sha256(str(time_diff * random.random()).encode()).hexdigest()

        dir_path = Path(EGG_DIR) / self.id
        egg = dir_path / "creature.py"

        zip_path = Path(EGG_DIR) / f"{self.id}.egg"

        egg.parent.mkdir(exist_ok=True, parents=True)

        fd = open(egg, "w")

        egg_template = EGG_TEMPLATE.format(
            name=name,
            attack=random.randint(0, 100),
            defense=random.randint(0, 100),
            health=random.randint(0, 100),
            stability=random.randint(0, 10),
            filename=filename,
        )

        fd.write(egg_template)
        fd.close()

        # create a .egg file to store the creature module
        with ZipFile(zip_path, "w", compression=ZIP_DEFLATED) as zf:
            for path in dir_path.rglob("*"):
                zf.write(path, arcname=path.relative_to(dir_path))

        shutil.rmtree(dir_path)

    @classmethod
    def from_existing(cls, egg_id: str, name: str):
        egg = cls.__new__(cls)
        egg.id = egg_id
        egg.name = name
        return egg

    def hatch(self, hatch_time: datetime, art_index: int) -> str:
        sys.path.insert(0, str(Path(EGG_DIR) / f"{self.id}.egg"))
        multiplier = self._score_time_diff(hatch_time, datetime.now())
        try:
            hatched = __import__("creature").Creature()
            hatched.attack *= multiplier
            hatched.defense *= multiplier
            hatched.health *= multiplier
            hatched.stability *= multiplier

            hatched.gen_ascii_art(art_index * ART_LEN)
            return f"{hatched.name}\n{hatched.get_ascii_art()}\nAttack: {hatched.attack}\nDefense: {hatched.defense}\nHealth: {hatched.health}\nStability: {hatched.stability}"
        finally:
            sys.path.pop(0)
            os.remove(str(Path(EGG_DIR) / f"{self.id}.egg"))

    def _score_time_diff(self, t1: datetime, t2: datetime, scale_seconds=60) -> float:
        diff = abs((t2 - t1).total_seconds())
        return 100 * (1 - math.exp(-diff / scale_seconds))


class Hatchery:
    def __init__(self, user: UUID):
        self.eggs: dict[str, tuple[Egg, datetime]] = {}
        self.user = user

    def incubate(self, name: str, filename: str, time: float) -> str:
        egg = Egg(name, filename, time)
        self.eggs[egg.id] = (egg, datetime.now())
        return egg.id

    def add_existing_egg(self, egg_id: str, name: str, created_at: datetime) -> None:
        self.eggs[egg_id] = (Egg.from_existing(egg_id, name), created_at)

    def hatch(self, egg_id: str, art_index: int) -> str:
        if egg_id not in self.eggs:
            return ""

        egg, hatch_time = self.eggs[egg_id]
        return egg.hatch(hatch_time, art_index)
