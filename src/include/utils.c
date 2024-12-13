#include <linux/string.h>  // For strsep and string functions
#include <linux/slab.h>    // For kmalloc and kfree
#include "utils.h"

// Function to parse the input command into command and arguments
parsed_command_t parse_command(const char *input) {
    parsed_command_t parsed;
    char *buf, *token;
    int arg_index = 0;

    // Allocate buffer and copy the input to modify it
    buf = kmalloc(strlen(input) + 1, GFP_KERNEL);
    if (!buf) {
        parsed.command = NULL;
        parsed.args = NULL;
        parsed.arg_count = 0;
        return parsed;
    }
    strncpy(buf, input, strlen(input) + 1);

    // First token is the command
    parsed.command = strsep(&buf, " ");

    // Count arguments
    parsed.arg_count = 0;
    while ((token = strsep(&buf, " ")) != NULL) {
        parsed.arg_count++;
    }

    // Allocate memory for arguments
    parsed.args = kmalloc(parsed.arg_count * sizeof(char *), GFP_KERNEL);
    if (!parsed.args) {
        parsed.command = NULL;
        parsed.arg_count = 0;
        kfree(buf);
        return parsed;
    }

    // Parse arguments
    buf = kmalloc(strlen(input) + 1, GFP_KERNEL);  // Copy input again for re-parsing
    strncpy(buf, input, strlen(input) + 1);

    // Skip the command and start extracting arguments
    strsep(&buf, " ");
    while ((token = strsep(&buf, " ")) != NULL) {
        parsed.args[arg_index++] = kstrdup(token, GFP_KERNEL);
    }

    // Free temporary buffer
    kfree(buf);
    return parsed;
}

// Function to free the parsed command and arguments
void free_parsed_command(parsed_command_t *parsed) {
    int i;

    if (parsed->command) {
        kfree(parsed->command);
    }

    for (i = 0; i < parsed->arg_count; i++) {
        if (parsed->args[i]) {
            kfree(parsed->args[i]);
        }
    }

    if (parsed->args) {
        kfree(parsed->args);
    }
}
