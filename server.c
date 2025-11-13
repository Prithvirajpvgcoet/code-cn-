#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 5000
#define BUFFER_SIZE 4096

int main() {
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size;
    char buffer[BUFFER_SIZE];

    // Create a TCP socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    // Configure server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind socket
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        exit(1);
    }

    // Listen for connections
    if (listen(server_fd, 1) < 0) {
        perror("Listen failed");
        exit(1);
    }

    printf("Server running on port %d...\n", PORT);

    addr_size = sizeof(client_addr);
    client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &addr_size);
    if (client_fd < 0) {
        perror("Accept failed");
        exit(1);
    }

    printf("Client connected.\n");

    // 1. Receive Hello message
    recv(client_fd, buffer, sizeof(buffer), 0);
    printf("Client says: %s\n", buffer);

    // 2. Send hello reply
    send(client_fd, "Hello from Server", 18, 0);

    // 3. Receive filename
    char filename[256];
    recv(client_fd, filename, sizeof(filename), 0);
    printf("Receiving file: %s\n", filename);

    // open file to write received contents
    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        perror("File open error");
        exit(1);
    }

    // 4. Receive file contents
    int bytes;
    while ((bytes = recv(client_fd, buffer, BUFFER_SIZE, 0)) > 0) {
        fwrite(buffer, 1, bytes, fp);
    }

    printf("File received successfully.\n");

    fclose(fp);
    close(client_fd);
    close(server_fd);

    return 0;
}
