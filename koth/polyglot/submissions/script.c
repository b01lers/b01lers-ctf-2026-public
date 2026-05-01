#include <stdio.h>
int main() {
    long a, b, c;
    scanf("%ld\n%ld\n%ld\n", &a, &b, &c);
    long r = 1;
    for (long i = 0; i < b; i++) {
        r = r * a % c;
    }
    printf("%ld\n", r);
    return 0;
}
