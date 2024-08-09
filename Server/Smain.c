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

void handle_client(int client_socket) {
    char buffer[BUFFER_SIZE];
    int n;

    while (1) {
        bzero(buffer, BUFFER_SIZE);
        n = read(client_socket, buffer, BUFFER_SIZE);
        if (n <= 0) {
            printf("Client disconnected\n");
            break;
        }
        printf("Client: %s", buffer);

        // Process the command
        // If command is upload, download, delete, or other, handle accordingly
        // Transfer files to Spdf or Stext based on file extension (.pdf/.txt)

        bzero(buffer, BUFFER_SIZE);
        strcpy(buffer, "Command processed\n");
        write(client_socket, buffer, sizeof(buffer));
    }

    close(client_socket);
}

int main() {
    int server_socket, client_socket, len;
    struct sockaddr_in server_addr, client_addr;

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        printf("Error in socket creation\n");
        exit(1);
    }
    printf("Socket created successfully\n");

    bzero(&server_addr, sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(PORT);

    if ((bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr))) != 0) {
        printf("Socket bind failed\n");
        exit(1);
    }
    printf("Socket binded successfully\n");

    if ((listen(server_socket, 5)) != 0) {
        printf("Listen failed\n");
        exit(1);
    }
    printf("Server listening...\n");

    len = sizeof(client_addr);

    while (1) {
        client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &len);
        if (client_socket < 0) {
            printf("Server accept failed\n");
            exit(1);
        }
        printf("Server accepted the client\n");

        if (fork() == 0) {
            close(server_socket);
            handle_client(client_socket);
            exit(0);
        } else {
            close(client_socket);
        }
    }

    close(server_socket);
    return 0;
}
