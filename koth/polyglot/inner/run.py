"""Polyglot language runner with nsjail sandboxing.

Modes:
  CLI:    run.py <language> <script-path>
  Server: run.py --serve (reads JSON from stdin per connection via socat)
"""

import base64
import json
import os
import stat
import sys
import tempfile
import shutil
import tomllib
import contextlib
from dataclasses import dataclass, field
from pathlib import Path

from nsjail import MountPt, NsJailConfig, Exe, Runner, IdMap, LogLevel

NOBODY = "65534"
RLIMIT_HARD = 2
MAX_STDOUT_BYTES = int(os.environ.get("MAX_STDOUT_BYTES", 50))
MAX_STDERR_BYTES = int(os.environ.get("MAX_STDERR_BYTES", 1000))


def truncate(s: str, limit: int) -> str:
    return s[:limit] + "..." if len(s) > limit else s


@dataclass
class LangRunner:
    cmd: list[str]
    timeout: int
    bins: list[str] = field(default_factory=list)
    dirs: list[str] = field(default_factory=list)
    env: list[str] = field(default_factory=list)
    filename: str = "script"
    needs_proc: bool = False
    binary_name: str | None = None
    i386: bool = False


@dataclass
class LangCompiler:
    cmd: list[str]
    timeout: int
    bins: list[str] = field(default_factory=list)
    dirs: list[str] = field(default_factory=list)
    env: list[str] = field(default_factory=list)
    filename: str = "script"
    needs_proc: bool = False
    binary_name: str | None = None


@dataclass
class LangDefinition:
    runner: LangRunner
    compiler: LangCompiler | None = None


config_path = os.path.join(os.path.dirname(__file__), "languages.toml")
with open(config_path, "rb") as f:
    _toml_data = tomllib.load(f)

seccomp_path = os.path.join(os.path.dirname(__file__), "seccomp.policy")
with open(seccomp_path) as f:
    SECCOMP_POLICY = f.read()

LANGUAGES: dict[str, LangDefinition] = {}
for name, data in _toml_data.items():
    runner = LangRunner(**data["runner"])
    compiler = LangCompiler(**data["compile"]) if "compile" in data else None
    LANGUAGES[name] = LangDefinition(runner=runner, compiler=compiler)


# Mounts shared by all languages.
BASE_MOUNTS = [
    # noexec would block .so loading via mmap(PROT_EXEC), so it is not used here.
    MountPt(src="/lib", dst="/lib", is_bind=True, rw=False, is_dir=True, nosuid=True, nodev=True),
    MountPt(src="/usr/lib", dst="/usr/lib", is_bind=True, rw=False, is_dir=True, nosuid=True, nodev=True),

    MountPt(src="/dev/null", dst="/dev/null", is_bind=True, rw=True, is_dir=False),
    MountPt(src="/dev/urandom", dst="/dev/urandom", is_bind=True, rw=False, is_dir=False),

    MountPt(dst="/tmp", fstype="tmpfs", rw=True),
]


def read_compiled_artifact(compile_dir: str, binary_name: str) -> bytes | None:
    """Safely read a compiled artifact, refusing symlinks.

    Opens with O_NOFOLLOW to prevent symlink substitution attacks
    (e.g. Template Haskell replacing /out/prog with a symlink to a
    host binary). Returns None if the file is missing, is not a
    regular file, or exceeds the size cap.
    """
    artifact_path = os.path.join(compile_dir, binary_name)
    try:
        fd = os.open(artifact_path, os.O_RDONLY | os.O_NOFOLLOW)
    except (FileNotFoundError, OSError):
        return None
    try:
        st = os.fstat(fd)
        if not stat.S_ISREG(st.st_mode):
            return None
        return os.read(fd, st.st_size)
    finally:
        os.close(fd)


def build_mounts(
    config: LangCompiler | LangRunner, script_content: bytes, out_dir: str = None, compiled_file: str = None
) -> list[MountPt]:
    """Build the mount list for a language sandbox.

    Only the language's own interpreter binaries are bind-mounted as
    individual files; other interpreters are never visible.
    The script is injected via src_content without writing to host disk.

    For compiled artifacts: the caller writes verified (O_NOFOLLOW-checked)
    bytes to a fresh host temp file and passes that path here. This avoids
    symlink substitution from the compile sandbox while preserving execute
    permissions needed by native binaries.
    """
    mounts = BASE_MOUNTS.copy()

    for bin_path in config.bins:
        if os.path.exists(bin_path):
            mounts.append(
                MountPt(
                    src=bin_path,
                    dst=bin_path,
                    is_bind=True,
                    rw=False,
                    is_dir=False,
                )
            )

    for dir_path in config.dirs:
        if os.path.exists(dir_path):
            mounts.append(
                MountPt(
                    src=dir_path,
                    dst=dir_path,
                    is_bind=True,
                    rw=False,
                    is_dir=True,
                )
            )

    if config.needs_proc:
        mounts.append(MountPt(src="/proc", dst="/proc", is_bind=True, rw=False, is_dir=True))

    mounts.append(
        MountPt(
            src_content=script_content,
            dst=f"/tmp/{config.filename}",
            rw=False,
            is_dir=False,
        )
    )

    if out_dir:
        mounts.append(MountPt(src=out_dir, dst="/out", is_bind=True, rw=True, is_dir=True))
    if compiled_file:
        target_name = config.binary_name if config.binary_name else "prog"
        mounts.append(MountPt(src=compiled_file, dst=f"/tmp/{target_name}", is_bind=True, rw=False, is_dir=False))

    return mounts


def compile_sandbox(lang: str, code: bytes, out_dir: str) -> dict:
    """Run compilation in the sandbox, placing the result in out_dir."""
    compiler = LANGUAGES[lang].compiler
    mounts = build_mounts(compiler, code, out_dir=out_dir)
    env_vars = compiler.env.copy()

    cfg = NsJailConfig(
        exec_bin=Exe(path=compiler.cmd[0], arg=compiler.cmd[1:], exec_fd=False),
        hostname="localhost",
        time_limit=compiler.timeout,
        mount=mounts,
        envar=env_vars,
        log_level=LogLevel.FATAL,
        seccomp_string=[SECCOMP_POLICY],
        uidmap=[IdMap(inside_id=NOBODY, outside_id=NOBODY, count=1)],
        gidmap=[IdMap(inside_id=NOBODY, outside_id=NOBODY, count=1)],
        clone_newcgroup=False,
        cgroup_mem_max=0,
        detect_cgroupv2=False,
        rlimit_nofile=512,
        rlimit_nofile_type=RLIMIT_HARD,
        rlimit_nproc=512,
        rlimit_nproc_type=RLIMIT_HARD,
        rlimit_fsize=64,
        rlimit_fsize_type=RLIMIT_HARD,
    )

    try:
        result = Runner(base_config=cfg).run()
    except Exception as e:
        return {"stdout": "", "stderr": str(e), "exit_code": -1}

    return {
        "stdout": truncate(result.stdout.decode(errors="replace") if result.stdout else "", MAX_STDOUT_BYTES),
        "stderr": truncate(result.stderr.decode(errors="replace") if result.stderr else "", MAX_STDERR_BYTES),
        "exit_code": result.returncode,
    }


@contextlib.contextmanager
def redirect_stdin(data: bytes = b""):
    """Replace stdin with a pipe containing data, restoring it on exit.

    Prevents sandboxed programs from inheriting socat's network socket
    (which would cause stdin-reading languages to hang forever) while
    still allowing them to read provided data and get a clean EOF.
    """
    r_fd, w_fd = os.pipe()
    if data:
        os.write(w_fd, data)
    os.close(w_fd)

    saved = os.dup(0)
    os.dup2(r_fd, 0)
    os.close(r_fd)
    try:
        yield
    finally:
        os.dup2(saved, 0)
        os.close(saved)


def run_sandbox(lang: str, code: bytes, stdin_data: bytes = b"", compiled_file: str = None) -> dict:
    """Run code in the sandbox and return {"stdout": ..., "stderr": ..., "exit_code": ...}."""
    runner = LANGUAGES[lang].runner
    mounts = build_mounts(runner, code, compiled_file=compiled_file)
    env_vars = runner.env.copy()

    # Kafel's BPF checks AUDIT_ARCH_X86_64 and kills other architectures,
    # so seccomp must be skipped for 32-bit (i386) binaries like python0.
    seccomp = [] if runner.i386 else [SECCOMP_POLICY]

    cfg = NsJailConfig(
        exec_bin=Exe(path=runner.cmd[0], arg=runner.cmd[1:], exec_fd=False),
        hostname="localhost",
        time_limit=runner.timeout,
        mount=mounts,
        envar=env_vars,
        log_level=LogLevel.FATAL,
        seccomp_string=seccomp,
        # Run as nobody both inside and outside the namespace.
        uidmap=[IdMap(inside_id=NOBODY, outside_id=NOBODY, count=1)],
        gidmap=[IdMap(inside_id=NOBODY, outside_id=NOBODY, count=1)],
        clone_newcgroup=False,
        cgroup_mem_max=0,
        detect_cgroupv2=False,
        rlimit_nofile=512,
        rlimit_nofile_type=RLIMIT_HARD,
        rlimit_nproc=512,
        rlimit_nproc_type=RLIMIT_HARD,
        rlimit_fsize=64,
        rlimit_fsize_type=RLIMIT_HARD,
    )

    with redirect_stdin(stdin_data):
        result = Runner(base_config=cfg).run()

    return {
        "stdout": truncate(result.stdout.decode(errors="replace") if result.stdout else "", MAX_STDOUT_BYTES),
        "stderr": truncate(result.stderr.decode(errors="replace") if result.stderr else "", MAX_STDERR_BYTES),
        "exit_code": result.returncode,
    }


def error_response(msg: str) -> str:
    return json.dumps({"error": msg}, separators=(",", ":"))


def handle_request(raw: str) -> str:
    """Parse a JSON request, run each language, return JSON results."""
    try:
        req = json.loads(raw)
    except json.JSONDecodeError as e:
        return error_response(f"invalid json: {e}")

    if not isinstance(req, dict):
        return error_response("request must be a json object")

    # Validate required fields
    if "languages" not in req:
        return error_response("missing field: languages")
    if "code" not in req:
        return error_response("missing field: code")
    if "testcases" not in req:
        return error_response("missing field: testcases")

    languages = req["languages"]
    if not isinstance(languages, list) or not languages:
        return error_response("languages must be a non-empty list")

    # Deduplicate (preserving order) to avoid redundant runs
    languages = list(dict.fromkeys(languages))

    # Validate all languages before running any
    invalid = [l for l in languages if l not in LANGUAGES]
    if invalid:
        return error_response(f"unknown languages: {invalid}. valid: {sorted(LANGUAGES)}")

    # Decode base64 code
    code_b64 = req["code"]
    if not isinstance(code_b64, str):
        return error_response("code must be a base64-encoded string")

    try:
        code_bytes = base64.b64decode(code_b64)
    except Exception as e:
        return error_response(f"base64 decode error: {e}")

    results = {}
    for lang in languages:
        config = LANGUAGES[lang]

        compile_fail = False
        compiled_file = None
        compile_err_msg = None

        ctx = (
            tempfile.TemporaryDirectory(prefix=f"polyglot_compile_{lang}_")
            if config.compiler
            else contextlib.nullcontext()
        )

        with ctx as compile_dir:
            if config.compiler:
                # Create the out directory wide-open so `nobody` can write to it cleanly
                os.chmod(compile_dir, 0o777)

                compile_result = compile_sandbox(lang, code_bytes, compile_dir)
                if compile_result["exit_code"] != 0:
                    compile_fail = True
                    compile_err_msg = compile_result.get("stderr", "compilation failed")
                else:
                    # Read artifact with O_NOFOLLOW to prevent symlink substitution,
                    # then write verified bytes to a new host-controlled temp file.
                    binary_name = config.compiler.binary_name if config.compiler.binary_name else "prog"
                    artifact_bytes = read_compiled_artifact(compile_dir, binary_name)
                    if artifact_bytes is None:
                        compile_fail = True
                        compile_err_msg = f"compiler did not produce a valid artifact at /out/{binary_name}"
                    else:
                        # Write to a fresh temp file that user code never touches
                        fd, compiled_file = tempfile.mkstemp(prefix=f"polyglot_run_{lang}_")
                        os.write(fd, artifact_bytes)
                        os.close(fd)
                        os.chmod(compiled_file, 0o555)

                if compile_fail:
                    results[lang] = {"err": compile_err_msg}
                    continue

            try:
                lang_results = []
                for testcase in req["testcases"]:
                    lang_results.append(run_sandbox(lang, code_bytes, testcase.encode(), compiled_file))

                results[lang] = {"err": None, "results": lang_results}
            finally:
                if compiled_file and os.path.exists(compiled_file):
                    os.unlink(compiled_file)

    return json.dumps(results, separators=(",", ":"))


def serve():
    """Handle a single connection from socat (read JSON from stdin, write to stdout)."""
    raw = input()
    response = handle_request(raw)
    print(response)


def cli():
    """CLI mode: run a single language from command-line args."""
    if len(sys.argv) < 3:
        print("Usage: run.py <language> <script-path>")
        print()
        print("Supported languages:")
        langs = sorted(LANGUAGES)
        for i in range(0, len(langs), 6):
            print(f"  {', '.join(langs[i : i + 6])}")
        sys.exit(1)

    lang, script_path = sys.argv[1], sys.argv[2]
    if lang not in LANGUAGES:
        print(f"Unknown language: {lang}", file=sys.stderr)
        sys.exit(1)

    script = Path(script_path).resolve()
    if not script.exists():
        print(f"Script not found: {script_path}", file=sys.stderr)
        sys.exit(1)

    result = run_sandbox(lang, script.read_bytes())
    sys.stdout.write(result["stdout"])
    sys.stderr.write(result["stderr"])
    sys.exit(result["exit_code"])


if __name__ == "__main__":
    if "--serve" in sys.argv:
        serve()
    else:
        cli()
