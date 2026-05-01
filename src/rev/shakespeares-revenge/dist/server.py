#!/usr/bin/env python3

import random
import subprocess
from pathlib import Path


def read_insults() -> list[str]:
	insults_path = Path(__file__).with_name("insults.txt")
	if not insults_path.exists():
		return []

	lines = [line.strip() for line in insults_path.read_text().splitlines() if line.strip()]
	return lines


def main() -> int:
	script_dir = Path(__file__).resolve().parent
	challenge_bin = script_dir / "shakespeare"
	challenge_file = script_dir / "challenge.spl"

	try:
		exit_code = subprocess.call([str(challenge_bin), str(challenge_file)])
	finally:
		insults = read_insults()
		if insults:
			print(random.choice(insults))
		else:
			print("No insults available.")

	return exit_code


if __name__ == "__main__":
	raise SystemExit(main())

