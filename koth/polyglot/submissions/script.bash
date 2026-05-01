read a
read b
read c

r=1
for ((i=0; i<b; i++)); do
    r=$((r * a % c))
done

echo $r
