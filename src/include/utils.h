#ifndef UTILS_H
#define UTILS_H

// Structure to hold parsed command and its arguments
typedef struct {
    char *command;    // Command name (e.g., reverse_tcp)
    char *arg1;       // First argument (e.g., IP)
    char *arg2;       // Second argument (e.g., port)
} parsed_command_t;

// Function to parse a command string
parsed_command_t parse_command(const char *input);

#endif
