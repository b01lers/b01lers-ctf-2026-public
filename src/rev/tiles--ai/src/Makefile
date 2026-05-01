.PHONY: build

build:
	uv run --with numpy gen.py
	podman build . --platform linux/amd64 -t gridlock
	podman run --platform linux/amd64 --rm -v ./:/src:ro -v ./../dist:/build gridlock

