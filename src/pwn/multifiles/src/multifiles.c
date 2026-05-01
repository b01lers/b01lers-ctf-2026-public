#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/device/class.h>
#include <linux/fs.h>
#include <linux/ioctl.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/mutex.h>
#include <linux/slab.h>
#include <linux/types.h>
#include <linux/uaccess.h>

#define MODULE_NAME "multifiles"

// only file supported atm
// TODO: add folder socket fifo
#define TYPE_FILE 1

// this should be the only flags needed. should not be leaked to userspace
#define DEFAULT_FLAGS 0x7d333a7b66746362

#define NAME_SIZE 16
#define DATA_COUNT 16
#define USERCOPY_SIZE (sizeof(u64) * DATA_COUNT + sizeof(char) * NAME_SIZE)
#define MAX_RW_SIZE 64

typedef struct {
    u64 type;
    u64 flags;
    char name[NAME_SIZE];
    u64 data[DATA_COUNT];
} MultiFile;

#define NUM_SLOTS 67

typedef struct {
    struct mutex lock;
    MultiFile *files[NUM_SLOTS];
    u32 active_idx;
} MultiFileList;

typedef struct {
    char name[16];
} MultiFileCreateReq;

static dev_t multifiles_devt;
static struct cdev multifiles_cdev;
static struct class *multifiles_class;
static struct kmem_cache *multifiles_cache;

static bool idx_is_valid(u32 idx) {
    return idx < NUM_SLOTS;
}

static MultiFile *get_active_file(MultiFileList *list) {
    if (list == NULL || !idx_is_valid(list->active_idx)) {
        return NULL;
    }

    // may return null
    return list->files[list->active_idx];
}

static long multifiles_create(struct file *self, unsigned long arg) {
    MultiFileList *list = self->private_data;
    long ret = 0;

    if (list == NULL) {
        return -EINVAL;
    }

    mutex_lock(&list->lock);

    for (u32 i = 0; i < NUM_SLOTS; i++) {
        if (list->files[i] == NULL) {
            MultiFile *multi_file = kmem_cache_zalloc(multifiles_cache, GFP_KERNEL);
            if (multi_file == NULL) {
                ret = -ENOMEM;
                goto out_unlock;
            }

            if (kmem_cache_copy_from_user(
                multifiles_cache,
                multi_file->name,
                (void __user *)arg,
                sizeof(MultiFileCreateReq)
            ) != 0) {
                kfree(multi_file);
                ret = -EFAULT;
                goto out_unlock;
            }

            multi_file->type = TYPE_FILE;
            multi_file->flags = DEFAULT_FLAGS;
            list->files[i] = multi_file;
            ret = i;
            goto out_unlock;
        }
    }

    ret = -ENOSPC;

out_unlock:
    mutex_unlock(&list->lock);
    return ret;
}

static long multifiles_delete(struct file *self, unsigned long arg) {
    MultiFileList *list = self->private_data;
    u32 index = (u32) arg;

    if (list == NULL || !idx_is_valid(index)) {
        return -EINVAL;
    }

    mutex_lock(&list->lock);
    MultiFile *multi_file = list->files[index];
    list->files[index] = NULL;
    mutex_unlock(&list->lock);

    if (multi_file == NULL) {
        return -ENOENT;
    }

    kfree(multi_file);
    return 0;
}

static long multifiles_set_active(struct file *self, unsigned long arg) {
    MultiFileList *list = self->private_data;
    u32 index = (u32) arg;

    if (list == NULL || !idx_is_valid(index)) {
        return -EINVAL;
    }

    mutex_lock(&list->lock);
    if (list->files[index] == NULL) {
        mutex_unlock(&list->lock);
        return -ENOENT;
    }

    list->active_idx = index;
    self->f_pos = 0;

    mutex_unlock(&list->lock);
    return 0;
}

static ssize_t multifiles_read(struct file *self, char __user *buf, size_t count, loff_t *offset) {
    MultiFileList *list = self->private_data;
    ssize_t ret = 0;
    if (list == NULL) {
        return -EINVAL;
    }

    mutex_lock(&list->lock);

    // check index is selected
    MultiFile *multi_file = get_active_file(list);
    if (multi_file == NULL) {
        ret = -ENOENT;
        goto out_unlock;
    }

    // check read bounds
    if (
        count > MAX_RW_SIZE
        || (count % sizeof(u64)) != 0
        || *offset >= sizeof(MultiFile)
        || *offset < 0
    ) {
        ret = -EINVAL;
        goto out_unlock;
    }

    loff_t old_offset = *offset;
    *offset += count;

    if (kmem_cache_copy_to_user(
        multifiles_cache,
        buf,
        ((u8 *) &multi_file->data[0]) + old_offset,
        count
    ) != 0) {
        ret = -EFAULT;
        goto out_unlock;
    }

    ret = count;

out_unlock:
    mutex_unlock(&list->lock);
    return ret;
}

static ssize_t multifiles_write(struct file *self, const char __user *buf, size_t count, loff_t *offset) {
    MultiFileList *list = self->private_data;
    ssize_t ret = 0;
    if (list == NULL) {
        return -EINVAL;
    }

    mutex_lock(&list->lock);

    // check index is selected
    MultiFile *multi_file = get_active_file(list);
    if (multi_file == NULL) {
        ret = -ENOENT;
        goto out_unlock;
    }

    // check write bounds
    if (
        count > MAX_RW_SIZE
        || (count % sizeof(u64)) != 0
        || *offset >= sizeof(MultiFile)
        || *offset < 0
    ) {
        ret = -EINVAL;
        goto out_unlock;
    }

    loff_t old_offset = *offset;
    *offset += count;

    if (kmem_cache_copy_from_user(
        multifiles_cache,
        ((u8 *) &multi_file->data[0]) + old_offset,
        buf,
        count
    ) != 0) {
        ret = -EFAULT;
        goto out_unlock;
    }

    ret = count;

out_unlock:
    mutex_unlock(&list->lock);
    return ret;
}

loff_t multifiles_llseek(struct file *self, loff_t offset, int whence) {
    MultiFileList *list = self->private_data;
    if (
        list == NULL
        // too lazy to support other types
        || whence != SEEK_SET
        || offset >= sizeof(MultiFile)
        || offset < 0
    ) {
        return -EINVAL;
    }

    mutex_lock(&list->lock);
    self->f_pos = offset;
    mutex_unlock(&list->lock);

    return offset;
}

#define CHAL_IOC_MAGIC 0xC7
#define CHAL_IOC_CREATE _IOW(CHAL_IOC_MAGIC, 0x01, MultiFileCreateReq)
#define CHAL_IOC_DELETE _IOW(CHAL_IOC_MAGIC, 0x02, u32)
#define CHAL_IOC_SET_ACTIVE _IOW(CHAL_IOC_MAGIC, 0x03, u32)

static long multifiles_ioctl(struct file *self, unsigned int cmd, unsigned long arg) {
    switch (cmd) {
    case CHAL_IOC_CREATE:
        return multifiles_create(self, arg);
    case CHAL_IOC_DELETE:
        return multifiles_delete(self, arg);
    case CHAL_IOC_SET_ACTIVE:
        return multifiles_set_active(self, arg);
    default:
        return -EINVAL;
    }
}

static int multifiles_open(struct inode *inode, struct file *self) {
    MultiFileList *list = kzalloc(sizeof(*list), GFP_KERNEL);
    if (list == NULL) {
        return -ENOMEM;
    }

    mutex_init(&list->lock);
    list->active_idx = 0;
    self->private_data = list;
    return 0;
}

static int multifiles_release(struct inode *inode, struct file *self) {
    MultiFileList *list = self->private_data;

    if (list == NULL) {
        return 0;
    }

    for (u32 i = 0; i < NUM_SLOTS; i++) {
        if (list->files[i] != NULL) {
            kmem_cache_free(multifiles_cache, list->files[i]);
        }
    }

    kfree(list);
    self->private_data = NULL;
    return 0;
}

static const struct file_operations multifiles_fops = {
    .owner = THIS_MODULE,
    .open = multifiles_open,
    .release = multifiles_release,
    .read = multifiles_read,
    .write = multifiles_write,
    .llseek = multifiles_llseek,
    .unlocked_ioctl = multifiles_ioctl,
    #ifdef CONFIG_COMPAT
    .compat_ioctl = multifiles_ioctl,
    #endif
};

static int __init multifiles_init(void) {
    int ret;

    multifiles_cache = kmem_cache_create_usercopy(
        MODULE_NAME "_cache",
        sizeof(MultiFile),
        0,
        SLAB_NO_MERGE,
        offsetof(MultiFile, name),
        USERCOPY_SIZE,
        NULL
    );
    if (multifiles_cache == NULL) {
        return -ENOMEM;
    }

    // 0 is base minor number, 1 is count
    ret = alloc_chrdev_region(&multifiles_devt, 0, 1, MODULE_NAME);
    if (ret) {
        goto err_cache;
    }

    cdev_init(&multifiles_cdev, &multifiles_fops);
    multifiles_cdev.owner = THIS_MODULE;

    // 1 is number of devices
    ret = cdev_add(&multifiles_cdev, multifiles_devt, 1);
    if (ret) {
        goto err_chrdev;
    }

    multifiles_class = class_create(MODULE_NAME);
    if (IS_ERR(multifiles_class)) {
        ret = PTR_ERR(multifiles_class);
        goto err_cdev;
    }

    if (IS_ERR(device_create(multifiles_class, NULL, multifiles_devt, NULL, MODULE_NAME))) {
        ret = -EINVAL;
        goto err_class;
    }

    pr_info(MODULE_NAME ": loaded\n");
    return 0;

err_class:
    class_destroy(multifiles_class);
err_cdev:
    cdev_del(&multifiles_cdev);
err_chrdev:
    unregister_chrdev_region(multifiles_devt, 1);
err_cache:
    kmem_cache_destroy(multifiles_cache);
    return ret;
}

static void __exit multifiles_exit(void) {
    device_destroy(multifiles_class, multifiles_devt);
    class_destroy(multifiles_class);
    cdev_del(&multifiles_cdev);
    unregister_chrdev_region(multifiles_devt, 1);
    kmem_cache_destroy(multifiles_cache);
    pr_info(MODULE_NAME ": unloaded\n");
}

module_init(multifiles_init);
module_exit(multifiles_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Athryx");
MODULE_DESCRIPTION("simple slop in kernel, wouldn't want to spoil it");
