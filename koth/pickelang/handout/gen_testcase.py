from random import randint, seed

MAX_BASE = 32767
MAX_EXPONENT = 32767
MAX_MODULUS = 65535

base = randint(2, MAX_BASE)
exponent = randint(1, MAX_EXPONENT)
modulus = randint(MAX_BASE+1, MAX_MODULUS)

print([base, exponent, modulus])
print(pow(base, exponent, modulus))

