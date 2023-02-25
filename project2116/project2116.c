#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

#define DEVICE_NAME "project2116"
#define EXAMPLE_MSG "Hello, World!\n"
#define MSG_BUFFER_LEN 15

MODULE_LICENSE("GPL");

static int major;
static char msg_buffer[MSG_BUFFER_LEN];
static char *msg_ptr;

static int my_open(struct inode *inode, struct file *filp)
{
    printk(KERN_INFO "project2116: Device has been opened\n");
    msg_ptr = msg_buffer;
    return 0;
}

static int my_release(struct inode *inode, struct file *filp)
{
    printk(KERN_INFO "project2116: Device has been released\n");
    return 0;
}

static ssize_t my_read(struct file *filp, char *buf, size_t count, loff_t *f_pos)
{
    int bytes_read = 0;
    while (count && *msg_ptr) {
        put_user(*(msg_ptr++), buf++);
        count--;
        bytes_read++;
    }
    return bytes_read;
}

static ssize_t my_write(struct file *filp, const char *buf, size_t count, loff_t *f_pos)
{
    printk(KERN_INFO "project2116: Sorry, this operation isn't supported\n");
    return -EINVAL;
}

static struct file_operations my_fops = {
    .open = my_open,
    .release = my_release,
    .read = my_read,
    .write = my_write
};

static int __init my_init(void)
{
    printk(KERN_INFO "project2116: Initializing the project2116 LKM\n");
    major = register_chrdev(0, DEVICE_NAME, &my_fops);
    if (major < 0) {
        printk(KERN_ALERT "project2116: Failed to register a major number\n");
        return major;
    }
    printk(KERN_INFO "project2116: Registered correctly with major number %d\n", major);
    strncpy(msg_buffer, EXAMPLE_MSG, MSG_BUFFER_LEN);
    return 0;
}

static void __exit my_exit(void)
{
    unregister_chrdev(major, DEVICE_NAME);
    printk(KERN_INFO "project2116: Goodbye, World!\n");
}

module_init(my_init);
module_exit(my_exit);




























































































