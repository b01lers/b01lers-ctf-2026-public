import java.util.Scanner;

void main() {
    Scanner sc = new Scanner(System.in);
    long a = sc.nextLong();
    long b = sc.nextLong();
    long c = sc.nextLong();
    long r = 1;
    for (long i = 0; i < b; i++) {
        r = r * a % c;
    }
    System.out.println(r);
}
