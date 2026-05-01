raw =. (1!:1) 3
nums =. ". ' ' (I. raw = LF) } raw
a =. 0 { nums
b =. 1 { nums
c =. 2 { nums
r =. (c | a & *)^:b ] 1
echo ": r
exit 0
