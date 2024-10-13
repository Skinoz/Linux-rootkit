#include <linux/string.h> // For strsep and string functions
#include <linux/slab.h>   // For kmalloc and kfree
#include "utils.h"

parsed_command_t parse_command(const char *input) {
    parsed_command_t parsed;
    char *buf, *command, *arg1, *arg2;

    // Allocate buffer and copy the input to modify it
    buf = kmalloc(strlen(input) + 1, GFP_KERNEL);
    if (!buf) {
        parsed.command = NULL;
        return parsed;
    }
    strncpy(buf, input, strlen(input) + 1);

    // Parse the command and arguments
    command = strsep(&buf, " ");
    arg1 = strsep(&buf, " ");
    arg2 = strsep(&buf, " ");

    // Allocate memory for parsed components
    parsed.command = command ? kstrdup(command, GFP_KERNEL) : NULL;
    parsed.arg1 = arg1 ? kstrdup(arg1, GFP_KERNEL) : NULL;
    parsed.arg2 = arg2 ? kstrdup(arg2, GFP_KERNEL) : NULL;

    // Free temporary buffer
    kfree(buf);
    return parsed;
}
