#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define SERVER_PORT 6567      // Remote UDP server port
#define CLIENT_PORT 12345     // Client UDP server port
#define BUFFER_SIZE 100       // Size of buffer

int main() {
    int sockfd;
    struct sockaddr_in server_addr, client_addr;
    char buffer[BUFFER_SIZE];
    socklen_t addr_len = sizeof(server_addr);

    // Create UDP socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
        perror("Error creating socket");
        return -1;
    }

    // Client configuration
    memset(&client_addr, 0, sizeof(client_addr));
    client_addr.sin_family = AF_INET;
    client_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    client_addr.sin_port = htons(CLIENT_PORT);

    // Bind client address with socket
    if (bind(sockfd, (struct sockaddr*)&client_addr, sizeof(client_addr)) < 0) {
        perror("Error binding client");
        close(sockfd);
        return -1;
    }

    // Server configuration
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK); // Server address (localhost)

    // Interactive message sending
    printf("Enter messages to send to the server (type 'exit' to quit):\n");
    while (1) {
        printf("> ");
        // Clear the buffer before getting new input
        memset(buffer, 0, BUFFER_SIZE);
        
        // Read user input
        if (fgets(buffer, BUFFER_SIZE, stdin) == NULL) {
            perror("Error reading input");
            break;
        }

        // Remove the newline character from the input
        buffer[strcspn(buffer, "\n")] = 0;

        // Check if the user wants to exit
        if (strcmp(buffer, "exit") == 0) {
            break;
        }

        // Send the message
        if (sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr*)&server_addr, addr_len) < 0) {
            perror("Error sending message");
            break;
        }

        printf("Sent to server: %s\n", buffer);

        // Receive response from the server
        memset(buffer, 0, BUFFER_SIZE); // Clear buffer before receive
        if (recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr*)&server_addr, &addr_len) < 0) {
            perror("Error receiving response");
            break;
        }

        printf("Response from server: %s\n", buffer);
    }

    // Close socket
    close(sockfd);
    return 0;
}
