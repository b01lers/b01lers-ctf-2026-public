import claripy
import random

INPUTLEN = 128

ops = ['AND', 'OR', 'NAND', 'NOR', 'XOR', 'IMPL', 'RIMPL']

def mkop(op, l, r):
    match op:
        case 'AND': return l & r
        case 'OR': return l | r
        case 'NAND': return ~(l & r)
        case 'NOR': return ~(l | r)
        case 'XOR': return l != r
        case 'IMPL': return ~l | r
        case 'RIMPL': return l | ~r
        case _: raise Exception

def try_gen():
    vars = [claripy.BoolS(f"i{i}") for i in range(INPUTLEN)]

    def add_var():
        j = len(vars)
        vars.append(claripy.BoolS(f"b{j - INPUTLEN}"))
        return j

    constrs = []
    for i in range(INPUTLEN):
        op = random.choice(ops)
        constrs.append((op, add_var(), i, random.randint(0, INPUTLEN - 1)))
    constrs.append((random.choice(ops), add_var(), INPUTLEN, INPUTLEN + 1))
    for i in range(2, INPUTLEN):
        j = add_var()
        constrs.append((random.choice(ops), j, j - 1, INPUTLEN + i))

    while True:
        s = claripy.Solver()
        for (op, c, a, b) in constrs:
            s.add(vars[c] == mkop(op, vars[a], vars[b]))

        s.add(vars[-1] == True)

        if not s.satisfiable():
            return None

        sol = s.batch_eval(vars[:INPUTLEN], 2)
        if len(sol) == 1:
            break
        for i in range(INPUTLEN):
            if sol[0][i] != sol[1][i]:
                if sol[1][i]:
                    constrs.append(('NAND', add_var(), i, i))
                    j = add_var()
                    constrs.append(('AND', j, j - 1, j - 2))
                else:
                    j = add_var()
                    constrs.append(('AND', j, j - 1, i))

    return (constrs, sol[0])


while True:
    res = try_gen()
    if res != None:
        (constrs, sols) = res
        break


perm = list(range(INPUTLEN))
flagchars = "absSRLUDrl"

while True:
    random.shuffle(perm)
    if not sols[perm[INPUTLEN - 1]]:
        continue
    flag = ""
    i = 0
    while i < INPUTLEN:
        j = 0
        while i + j < INPUTLEN and not sols[perm[i + j]]:
            j += 1
        if j >= 10:
            break
        i += j + 1
        flag += flagchars[j]
    else:
        break

flag = f"flag{{{flag}}}"

constrs.sort(key=lambda c: c[0])

done = set(range(INPUTLEN))
constrs2 = []
while len(constrs2) < len(constrs):
    for c in constrs:
        if c[1] in done:
            continue
        if c[2] in done and c[3] in done:
            constrs2.append(c)
            done.add(c[1])

constrs = constrs2

names = [f"inputs[{perm.index(i)}]" for i in range(
    INPUTLEN)] + [f"b{i}" for i in range(len(constrs))]

for (i, (o, c, a, b)) in enumerate(constrs):
    if i == 0 or constrs[i - 1][0] != o:
        print(f"SETOP({o});")
    print(f"u16 {names[c]} = do_op({names[a]}, {names[b]});")

print(f"u16 res = {names[len(constrs) + INPUTLEN - 1]};")

print(f"// {flag}")
