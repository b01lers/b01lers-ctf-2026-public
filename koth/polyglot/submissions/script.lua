local a = tonumber(io.read())
local b = tonumber(io.read())
local c = tonumber(io.read())
local r = 1
for i = 1, b do
    r = r * a % c
end
print(r)
