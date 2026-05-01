#!/usr/local/bin/python3
import os
import pwd
import random
import select
import stat
import subprocess
import sys

# pwn.red/jail passes the network connection as stdin/stdout
# Use /tmp for writable temp files

TIMEOUT = 5
TMPDIR = "/tmp"
COMPILER = "/app/compiler_wrapper"
CURRENT_COMPILER = "/tmp/compiler"
OG = "/app/calc1"
TARGET_UID = 1337
LINE_ENDING_GRACE = 0.05


class InputClosed(Exception):
    pass


class TokenReader:
    def __init__(self, fd):
        self.fd = fd
        self.pending = bytearray()

    def _read_byte_from_fd(self, timeout=None):
        if timeout is not None:
            readable, _, _ = select.select([self.fd], [], [], timeout)
            if not readable:
                return None

        data = os.read(self.fd, 1)
        if not data:
            return b""
        return data

    def read_byte(self):
        if self.pending:
            data = bytes(self.pending[:1])
            del self.pending[:1]
            return data
        return self._read_byte_from_fd()

    def unread(self, data):
        self.pending[:0] = data

    def read_available_byte(self, timeout=0):
        if self.pending:
            return self.read_byte()
        return self._read_byte_from_fd(timeout=timeout)

    def consume_optional_line_ending(self):
        byte = self.read_available_byte(timeout=LINE_ENDING_GRACE)
        if byte == b"\n":
            return
        if byte == b"\r":
            next_byte = self.read_available_byte(timeout=LINE_ENDING_GRACE)
            if next_byte not in (None, b"", b"\n"):
                self.unread(next_byte)
            return
        if byte not in (None, b""):
            self.unread(byte)

    def read_until_token(self):
        data = bytearray()
        token = b"&&"

        while True:
            byte = self.read_byte()
            if byte == b"":
                if data:
                    return bytes(data), False
                raise InputClosed()

            data += byte
            if data.endswith(token):
                del data[-len(token):]
                self.consume_optional_line_ending()
                return bytes(data), True

def run_compiler(binary_path, source):
    result = subprocess.run(
        [binary_path],
        input=source,
        capture_output=True,
        timeout=TIMEOUT,
        check=False,
    )
    if result.returncode != 0:
        raise Exception(f"{binary_path} failed with return code {result.returncode}")
    return result.stdout

def format_bytes(data, limit=64):
    suffix = "" if len(data) <= limit else "..."
    return data[:limit].hex() + suffix

def hex_push(value):
    return f"${value & 0xff:02x}"

def build_test_cases():
    tests = [
        ("direct hex bytes", b"7f 45 4c 46\n"),
        ("push and output", b"$41 @ $42 @ $43 @\n"),
        ("comments and whitespace", b"# leading comment\n\t$0a @  $20 @\n# trailing comment\n"),
        ("all arithmetic operators", b"$05 $03 + @ $05 $03 - @ $05 $03 * @ $09 $03 / @\n"),
        ("stack order", b"$02 $03 $04 + * @ $20 $03 $02 * - @\n"),
        ("comments inside literals", b"4# direct byte split across a comment\n1 $4# push byte split across a comment\n2 @\n"),
        ("raw getchar byte", b"!Z @\n"),
        ("raw getchar nul byte", b"!\x00 @\n"),
        ("overflow low byte behavior", b"$ff $02 + @ $80 $02 * @ $00 $01 - @\n"),
        ("subtraction and division order", b"$21 $04 / @ $21 $04 - @ $04 $21 - @\n"),
    ]

    for test_num in range(16):
        a = random.randint(0, 255)
        b = random.randint(0, 255)
        c = random.randint(0, 255)
        d = random.randint(0, 255)
        e = random.randint(0, 255)
        f = random.randint(0, 255)
        g = random.randint(0, 255)
        h = random.randint(0, 255)
        divisor = random.randint(1, 31)
        direct1 = random.randint(0, 255)
        direct2 = random.randint(0, 255)

        source = (
            f"# randomized mixed test {test_num}\n"
            f"{hex_push(a)} {hex_push(b)} + @\n"
            f"{hex_push(c)} {hex_push(d)} * {hex_push(e)} + @\n"
            f"{hex_push(f)} {hex_push(g)} - @\n"
            f"{hex_push(h)} {hex_push(divisor)} / @\n"
            f"{direct1:02x} {direct2:02x}\n"
        ).encode("utf-8")
        tests.append((f"randomized mixed expression {test_num}", source))

    for test_num in range(6):
        values = [random.randint(0, 255) for _ in range(5)]
        source = (
            f"# randomized stack depth test {test_num}\n"
            f"{hex_push(values[0])} {hex_push(values[1])} + "
            f"{hex_push(values[2])} + {hex_push(values[3])} + "
            f"{hex_push(values[4])} + @\n"
            f"!{chr(random.randint(33, 126))} @\n"
        ).encode("utf-8")
        tests.append((f"randomized stack depth {test_num}", source))

    return tests

def main():
    global OG
    try:
        subprocess.run(["ln", "-sf", OG, CURRENT_COMPILER], check=True)
        reader = TokenReader(sys.stdin.fileno())

        # print(subprocess.run(["ls", "-al"], check=True))
        # print(subprocess.run(["ls", "-al", "/tmp"], check=True))

        for stage in range(2):
            try:
                data, had_token = reader.read_until_token()
            except InputClosed:
                break

            # 2. Write to temp file
            temp_he = os.path.join(TMPDIR, "temp.he")
            with open(temp_he, "wb") as f:
                f.write(data)

            # Generate a random ID for this build
            build_id = random.randint(100000, 999999)
            new_binary = os.path.join(TMPDIR, f"calc{build_id}")
            #print(f"recieved bin at {new_binary}\n")

            # 3. Compile: wrapper -> current compiler < temp.he > new_binary
            with open(temp_he, "r") as f_in, open(new_binary, "wb") as f_out:
                subprocess.run([COMPILER], stdin=f_in, stdout=f_out, timeout=TIMEOUT, check=True)

            os.chmod(new_binary, os.stat(new_binary).st_mode | stat.S_IEXEC)

            # 4. Run tests
            tests_passed = True
            test_cases = build_test_cases()

            for test_index, (test_name, test_input) in enumerate(test_cases, 1):
                expected = run_compiler(COMPILER, test_input)
                actual = run_compiler(new_binary, test_input)

                if actual != expected:
                    print(
                        f"Test {test_index} ({test_name}) failed. "
                        f"Expected: {format_bytes(expected)}, Got: {format_bytes(actual)}"
                    )
                    tests_passed = False
                    break

            # 5. Update the wrapper target if successful
            if tests_passed:
                pw_record = pwd.getpwuid(TARGET_UID)
                owner_name = pw_record.pw_name
                os.chmod(new_binary, 0o755)
                
                OG = new_binary
                subprocess.run(["ln", "-sf", new_binary, CURRENT_COMPILER], check=True)
                print(
                    f"Success: Compilation and tests passed. Wrapper now launches {owner_name}'s compiler.",
                    flush=True,
                )
            else:
                print("Failure: Tests did not pass.", flush=True)

            if not had_token:
                break

    except subprocess.TimeoutExpired:
        print("Error: Process timed out (possible infinite loop).", flush=True)
    except Exception as e:
        import traceback
        traceback.print_exc()
        print(f"Error: {e}", flush=True)

if __name__ == "__main__":
    main()
