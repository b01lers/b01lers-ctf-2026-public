package main

import "core:fmt"
import "core:os"
import "core:strconv"
import "core:strings"

main :: proc() {
    buf: [256]byte
    n, _ := os.read(os.stdin, buf[:])
    parts := strings.split(strings.trim_right(string(buf[:n]), "\n"), "\n")
    a, _ := strconv.parse_int(parts[0])
    b, _ := strconv.parse_int(parts[1])
    c, _ := strconv.parse_int(parts[2])
    r := 1
    for i := 0; i < b; i += 1 {
        r = r * a % c
    }
    fmt.println(r)
}
