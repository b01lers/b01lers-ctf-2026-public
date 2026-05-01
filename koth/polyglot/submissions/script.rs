use std::io;
fn main() {
    let mut s = String::new();
    io::stdin().read_line(&mut s).unwrap();
    let a: i64 = s.trim().parse().unwrap();
    s.clear();
    io::stdin().read_line(&mut s).unwrap();
    let b: i64 = s.trim().parse().unwrap();
    s.clear();
    io::stdin().read_line(&mut s).unwrap();
    let c: i64 = s.trim().parse().unwrap();
    let mut r: i64 = 1;
    for _ in 0..b {
        r = r * a % c;
    }
    println!("{}", r);
}
