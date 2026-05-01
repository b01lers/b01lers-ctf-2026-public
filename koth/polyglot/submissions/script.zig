const std = @import("std");

fn readNum(f: std.fs.File) u64 {
    var n: u64 = 0;
    while (true) {
        var buf: [1]u8 = undefined;
        const len = f.read(&buf) catch return n;
        if (len == 0) return n;
        const ch = buf[0];
        if (ch >= 48 and ch <= 57) {
            n = n * 10 + ch - 48;
        } else if (n > 0) return n;
    }
}

pub fn main() !void {
    const stdin = std.fs.File.stdin();
    const a = readNum(stdin);
    const b = readNum(stdin);
    const c = readNum(stdin);
    var r: u64 = 1;
    for (0..b) |_| r = r * a % c;
    var buf: [64]u8 = undefined;
    const out = std.fmt.bufPrint(&buf, "{d}\n", .{r}) catch "";
    _ = try std.fs.File.stdout().write(out);
}
