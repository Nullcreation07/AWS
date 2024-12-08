#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};
    char name[BUFFER_SIZE];
    
    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    
    // Bind the socket to the network address and port
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    
    // Listen for incoming connections
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    
    printf("Enter your name: ");
    fgets(name, BUFFER_SIZE, stdin);
    strtok(name, "\n"); // Remove newline character

    printf("Waiting for connection...\n");
    
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    printf("Connected.\n");

    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        read(new_socket, buffer, BUFFER_SIZE);

        if (strcmp(buffer, "OVER69") == 0) {
            printf("Connection terminated.\n");
            break;
        }

        printf("%s\n", buffer);
        
        memset(buffer, 0, BUFFER_SIZE);
        printf("%s: ", name);
        fgets(buffer, BUFFER_SIZE, stdin);
        strtok(buffer, "\n"); // Remove newline character

        send(new_socket, buffer, strlen(buffer), 0);

        if (strcmp(buffer, "OVER69") == 0) {
            printf("Connection terminated.\n");
            break;
        }
    }

    close(new_socket);
    close(server_fd);
    return 0;
}
