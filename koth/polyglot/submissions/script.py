a = input()
b = input()
c = input()
r = 1
i = 0
while i < b:
	r = (r * a) % c
	i = i + 1
print r
