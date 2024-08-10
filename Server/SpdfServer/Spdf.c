#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

#define PORT 8081  // Port for PDF server
#define BUFFER_SIZE 1024

void handle_pdf_request(int smain_socket) {
    char buffer[BUFFER_SIZE];
    int n;

    while (1) {
        bzero(buffer, BUFFER_SIZE);
        n = read(smain_socket, buffer, BUFFER_SIZE);
        if (n <= 0) {
            printf("Smain disconnected\n");
            break;
        }
        printf("Received request from Smain: %s", buffer);

        // Here you would handle the file transfer, storage, or retrieval of PDF files
        // For example, save a PDF file, send a PDF file, or delete a PDF file

        bzero(buffer, BUFFER_SIZE);
        strcpy(buffer, "PDF request processed\n");
        write(smain_socket, buffer, sizeof(buffer));
    }

    close(smain_socket);
}

int main() {
    int server_socket, smain_socket, len;
    struct sockaddr_in server_addr, smain_addr;

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        printf("Error in socket creation\n");
        exit(1);
    }
    printf("PDF Server socket created successfully\n");

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
    printf("PDF Server listening...\n");

    len = sizeof(smain_addr);

    while (1) {
        smain_socket = accept(server_socket, (struct sockaddr*)&smain_addr, &len);
        if (smain_socket < 0) {
            printf("Server accept failed\n");
            exit(1);
        }
        printf("Accepted connection from Smain\n");

        if (fork() == 0) {
            close(server_socket);
            handle_pdf_request(smain_socket);
            exit(0);
        } else {
            close(smain_socket);
        }
    }

    close(server_socket);
    return 0;
}
