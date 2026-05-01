#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <linux/uaccess.h>
#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/inet.h>

#define MAX_RULES 256

struct firewall_rule {
    uint32_t src_ip;
    uint32_t dst_ip;
    uint16_t port;
    uint16_t action;  // ALLOW = 1, DENY = 0
    char desc[1012];
};

_Static_assert(sizeof(struct firewall_rule) == 1024,
               "firewall_rule must be 1024 bytes");

struct fw_add_req {
    char rule_str[256];
};

struct fw_edit_req {
    int idx;
    size_t offset;
    size_t len;
    char data[1024];
};

struct fw_show_req {
    int idx;
    size_t offset;
    size_t len;
    char data[1024];
};

#define FW_ADD_RULE    _IOW('F', 1, struct fw_add_req)
#define FW_DEL_RULE    _IOW('F', 2, int)
#define FW_EDIT_RULE   _IOW('F', 3, struct fw_edit_req)
#define FW_SHOW_RULE   _IOR('F', 4, struct fw_show_req)

static struct firewall_rule *rules[MAX_RULES];

/* Parse IP address string to uint32_t */
static int parse_ip(const char *str, uint32_t *ip)
{
    if (in4_pton(str, -1, (u8 *)ip, -1, NULL) == 0)
        return -EINVAL;
    
    *ip = ntohl(*ip);
    return 0;
}

/* Parse rule string: "SRC_IP DST_IP PORT ACTION DESCRIPTION" */
static int parse_rule(const char *input, struct firewall_rule *rule)
{
    char *token, *cursor;
    char buf[256];
    int field = 0;
    int ret;

    strncpy(buf, input, sizeof(buf) - 1);
    buf[sizeof(buf) - 1] = '\0';
    cursor = buf;

    while ((token = strsep(&cursor, " \t")) != NULL) {
        if (*token == '\0')
            continue;

        switch (field) {
        case 0:  /* src_ip */
            ret = parse_ip(token, &rule->src_ip);
            if (ret)
                return ret;
            break;

        case 1:  /* dst_ip */
            ret = parse_ip(token, &rule->dst_ip);
            if (ret)
                return ret;
            break;

        case 2:  /* port */
            ret = kstrtou16(token, 10, &rule->port);
            if (ret)
                return ret;
            break;

        case 3:  /* action (0=DENY, 1=ALLOW) */
            ret = kstrtou16(token, 10, &rule->action);
            if (ret)
                return ret;
            if (rule->action > 1)
                return -EINVAL;
            break;

        case 4:  /* description (rest of string) */
            if (cursor) {
                snprintf(rule->desc, sizeof(rule->desc), "%s %s", 
                         token, cursor);
            } else {
                snprintf(rule->desc, sizeof(rule->desc), "%s", token);
            }
            return 0;

        default:
            return -EINVAL;
        }

        field++;
    }

    /* Need at least the first 4 fields */
    if (field < 4)
        return -EINVAL;

    return 0;
}

static long fw_add_rule(struct fw_add_req __user *user_req)
{
    struct fw_add_req req;
    struct firewall_rule *rule;
    int idx, ret;

    if (copy_from_user(&req, user_req, sizeof(req)))
        return -EFAULT;

    /* Find empty slot */
    for (idx = 0; idx < MAX_RULES; idx++) {
        if (!rules[idx])
            break;
    }
    if (idx >= MAX_RULES)
        return -ENOMEM;

    /* Allocate rule */
    rule = kmalloc(sizeof(*rule), GFP_KERNEL_ACCOUNT);
    if (!rule)
        return -ENOMEM;

    memset(rule, 0, sizeof(*rule));

    /* Parse rule string */
    ret = parse_rule(req.rule_str, rule);
    if (ret) {
        kfree(rule);
        pr_err("firewall: failed to parse rule: '%s'\n", req.rule_str);
        return ret;
    }

    rules[idx] = rule;
    
    pr_info("firewall: added rule %d: %pI4 -> %pI4:%u %s\n",
            idx, &rule->src_ip, &rule->dst_ip, rule->port,
            rule->action ? "ALLOW" : "DENY");
    
    return idx;
}

static long fw_del_rule(int idx)
{
    if (idx < 0 || idx >= MAX_RULES)
        return -EINVAL;
    if (!rules[idx])
        return -ENOENT;

    kfree(rules[idx]);
    
    pr_info("firewall: deleted rule %d\n", idx);
    return 0;
}

static long fw_edit_rule(struct fw_edit_req __user *user_req)
{
    struct fw_edit_req req;

    if (copy_from_user(&req, user_req, sizeof(req)))
        return -EFAULT;

    if (req.idx < 0 || req.idx >= MAX_RULES)
        return -EINVAL;
    if (!rules[req.idx])
        return -ENOENT;

    if (req.offset + req.len > sizeof(struct firewall_rule))
        return -EINVAL;

    /* UAF write */
    memcpy((char *)rules[req.idx] + req.offset, req.data, req.len);

    return 0;
}

static long fw_show_rule(struct fw_show_req __user *user_req)
{
    struct fw_show_req req;

    if (copy_from_user(&req, user_req, sizeof(req)))
        return -EFAULT;

    if (req.idx < 0 || req.idx >= MAX_RULES)
        return -EINVAL;
    pr_info("fw_show_rule: rules[%d]=%px\n", req.idx, rules[req.idx]);
    if (!rules[req.idx])
        return -ENOENT;

    if (req.offset + req.len > sizeof(struct firewall_rule))
        return -EINVAL;

    /* UAF read */
    if (copy_to_user(user_req->data, (char *)rules[req.idx] + req.offset, 
                     req.len))
        return -EFAULT;

    return 0;
}

static long firewall_ioctl(struct file *filp, unsigned int cmd,
                           unsigned long arg)
{
    switch (cmd) {
    case FW_ADD_RULE:
        return fw_add_rule((struct fw_add_req __user *)arg);
    case FW_DEL_RULE:
        return fw_del_rule((int)arg);
    case FW_EDIT_RULE:
        return fw_edit_rule((struct fw_edit_req __user *)arg);
    case FW_SHOW_RULE:
        return fw_show_rule((struct fw_show_req __user *)arg);
    default:
        return -ENOTTY;
    }
}

static const struct file_operations firewall_fops = {
    .owner = THIS_MODULE,
    .unlocked_ioctl = firewall_ioctl,
};

static struct miscdevice firewall_dev = {
    .minor = MISC_DYNAMIC_MINOR,
    .name = "firewall",
    .fops = &firewall_fops,
};

static int __init firewall_init(void)
{
    int ret;

    pr_info("firewall: initializing (rule size = %lu bytes)\n",
            sizeof(struct firewall_rule));

    ret = misc_register(&firewall_dev);
    if (ret) {
        pr_err("firewall: failed to register device\n");
        return ret;
    }

    pr_info("firewall: device registered as /dev/firewall\n");
    return 0;
}

static void __exit firewall_exit(void)
{
    int i;

    misc_deregister(&firewall_dev);

    for (i = 0; i < MAX_RULES; i++) {
        kfree(rules[i]);
    }

    pr_info("firewall: module unloaded\n");
}

module_init(firewall_init);
module_exit(firewall_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("b01lers");
MODULE_DESCRIPTION("A Fake Firewall Module");
MODULE_VERSION("1.0");