#ifndef UTILS_H
#define UTILS_H

#include <linux/types.h>

// Structure to hold parsed command and its arguments
typedef struct {
    char *command;  // The main command
    char **args;    // Array of arguments
    int arg_count;  // Number of arguments
} parsed_command_t;

// Function prototypes
parsed_command_t parse_command(const char *input);  // Function to parse the input command
void free_parsed_command(parsed_command_t *parsed); // Function to free allocated memory for parsed command

#endif
