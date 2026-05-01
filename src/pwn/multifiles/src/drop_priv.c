#include <grp.h>
#include <unistd.h>

int main(void) {
    gid_t gid = 1000;

    if (setgroups(1, &gid) != 0) {
        return 1;
    }
    if (setgid(gid) != 0) {
        return 1;
    }
    if (setuid(1000) != 0) {
        return 1;
    }
    if (chdir("/home/ctf") != 0) {
        return 1;
    }

    execl("/bin/sh", "sh", NULL);
    return 1;
}
