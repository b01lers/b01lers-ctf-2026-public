a = parse(Int, readline())
b = parse(Int, readline())
c = parse(Int, readline())
r = 1
for i = 1:b
    global r = r * a % c
end
println(r)
