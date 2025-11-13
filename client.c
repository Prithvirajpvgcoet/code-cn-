#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 5000
#define BUFFER_SIZE 4096

int main() {
    int client_fd;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];
    char filename[256];

    // Create socket
    client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (client_fd < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    // Set server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1"); // Change to server IP if needed

    // Connect to server
    if (connect(client_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        exit(1);
    }

    printf("Connected to server.\n");

    // 1. Send Hello to server
    send(client_fd, "Hello from Client", 18, 0);

    // 2. Receive server reply
    recv(client_fd, buffer, sizeof(buffer), 0);
    printf("Server says: %s\n", buffer);

    // 3. Ask user for filename
    printf("Enter file to send: ");
    scanf("%s", filename);

    // send filename
    send(client_fd, filename, sizeof(filename), 0);

    // open file
    FILE *fp = fopen(filename, "rb");
    if (!fp) {
        perror("File open error");
        exit(1);
    }

    // 4. Send file content
    int n;
    while ((n = fread(buffer, 1, BUFFER_SIZE, fp)) > 0) {
        send(client_fd, buffer, n, 0);
    }

    printf("File sent.\n");

    fclose(fp);
    close(client_fd);

    return 0;
}
