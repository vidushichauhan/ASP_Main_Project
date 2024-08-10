#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>

#define PORT 9080
#define BUFFER_SIZE 1024

int main() {
    int client_socket;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];
    char command[BUFFER_SIZE];
    FILE *fp;

    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket < 0) {
        printf("Error in socket creation\n");
        exit(1);
    }
    printf("Socket created successfully\n");

    bzero(&server_addr, sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Ensure this matches the server's IP

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
        command[strcspn(command, "\n")] = 0; // Remove newline character
        printf("Sending command: %s\n", command);

        write(client_socket, command, strlen(command));

        // Check if the command is 'ufile' and contains a file name
        if (strncmp("ufile", command, 5) == 0) {
            char *filename = strtok(command + 6, " "); // Get filename after 'ufile '
            fp = fopen(filename, "r");
            if (fp == NULL) {
                perror("Error opening file");
                continue;
            }
            printf("Sending file content of %s\n", filename);

            // Send file content to the server
            while (fgets(buffer, BUFFER_SIZE, fp) != NULL) {
                write(client_socket, buffer, strlen(buffer));
            }
            fclose(fp);
        }

        // Expect a response from the server
        int bytes_received = read(client_socket, buffer, BUFFER_SIZE);
        if (bytes_received > 0) {
            buffer[bytes_received] = '\0'; // Null-terminate the buffer
            printf("Server: %s\n", buffer);
        } else {
            printf("No response from server, connection might be closed.\n");
            break;
        }

        if (strncmp("exit", command, 4) == 0) {
            printf("Exiting...\n");
            break;
        }
    }

    close(client_socket);
    return 0;
}
