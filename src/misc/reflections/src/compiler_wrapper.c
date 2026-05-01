#define _GNU_SOURCE
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(void) {
    const char *target = "/tmp/compiler";
    execl(target, target, (char *)NULL);
    fprintf(stderr, "exec %s failed: %s\n", target, strerror(errno));
    return 111;
}
