# Project Overview

This project consists of a Linux Kernel Module (LKM) designed to execute various system commands, with communication being handled via a UDP server. The architecture allows for modular command handling, making it extensible for future functionalities such as custom command execution, new communication protocols, and advanced kernel-level hooks.

## Directory Structure

### 1. `com/` Directory: Communication Layer

All communication logic resides in the `com` directory. This directory currently houses the UDP server implementation, responsible for receiving and sending messages to trigger and control the execution of commands.

- **udp_server.c**: Implements the logic for setting up the UDP socket, listening for incoming commands, and handling command execution. It also sends responses to the client after executing the commands.

- **udp_server.h**: Declares function prototypes and structures used by the UDP server for easy inclusion in other parts of the project.

### 2. `commands/` Directory: Command Handling

The `commands` directory contains all the system commands that can be executed by the kernel module. This includes commands like starting a reverse shell or activating a keylogger. The commands are organized and executed based on string-based input from the UDP server.

- **bash.c**: Contains the implementation for the `reverse_tcp` command, which starts a reverse shell. This command can accept an optional IP address and port as arguments.

- **keylogger.c**: Implements the `keylogger` command, which enables logging of keystrokes on the system.

- **headers/commands.h**: A header file that lists all the available commands that the module can execute. This file maps string commands to the respective function that handles the command execution.

### 3. `include/` Directory: Utility Functions

The `include` directory contains helper functions to facilitate command processing. These functions perform tasks such as parsing input arguments, handling command strings, and validating data passed to the commands.

- **utils.h**: A utility file that includes helpful functions for parsing commands and handling optional arguments. This simplifies command handling and ensures that arguments are correctly passed to the respective functions.

## Future Evolution of the Project

### TODO:

- Add New Communication Methods
- Expand Command Set
- Hide different processes by hooking functions (Add new directory hooks)

## How It Works

### Step 1: Compile the Kernel Module

1. **Navigate to the Project Directory**: Open a terminal and navigate to the root of your project directory, where the `Makefile` is located.

   ```bash
   cd /path/to/your/project

2.** Compile **: Install and execute 
   
   ```bash
   make

### Step 2: Insert the Kernel Module

1. **Load the Module**: Use sudo insmod to insert the compiled kernel module into the kernel. This requires root privileges, hence the use of sudo.
    
    ```bash
    sudo insmod main.ko

2. **Check Kernel Messages**: Use dmesg to view kernel messages, which will help you verify that the module has initialized correctly.
    
    ```bash
    dmesg | tail

### Step 3: Custom and Use the UDP Client to Send Commands

1. **Compile the UDP Client**: Ensure you have a client program (e.g., client.c) that sends commands to your UDP server. Compile it using gcc.
    
    ```bash
    gcc client.c -o client

2. **Run the client**
    
    ```bash
    ./a.out
