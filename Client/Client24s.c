#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int client_socket;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];
    char command[BUFFER_SIZE];

    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0) {
        printf("Error in socket creation\n");
        exit(1);
    }
    printf("Socket created successfully\n");

    bzero(&server_addr, sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Server IP

    if (connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) != 0) {
        printf("Connection to Smain failed\n");
        exit(1);
    }
    printf("Connected to Smain\n");

    while (1) {
        bzero(buffer, BUFFER_SIZE);
        bzero(command, BUFFER_SIZE);

        printf("Enter command: ");
        fgets(command, BUFFER_SIZE, stdin);

        write(client_socket, command, sizeof(command));
        read(client_socket, buffer, sizeof(buffer));
        printf("Server: %s", buffer);

        if (strncmp("exit", command, 4) == 0) {
            printf("Exiting...\n");
            break;
        }
    }

    close(client_socket);
    return 0;
}
