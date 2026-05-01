const input: string[] = (await Bun.stdin.text()).trim().split("\n");
const a: number = parseInt(input[0]);
const b: number = parseInt(input[1]);
const c: number = parseInt(input[2]);
let r: number = 1;
for (let i: number = 0; i < b; i++) {
    r = r * a % c;
}
console.log(r);
