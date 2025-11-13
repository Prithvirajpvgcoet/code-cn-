#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int server_socket, new_socket;
    char buffer[1024] = {0};
    struct sockaddr_in server_addr;
    int addr_len = sizeof(server_addr);

    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr));
    listen(server_socket, 3);

    printf("Server waiting for connection...\n");
    new_socket = accept(server_socket, (struct sockaddr*)&server_addr, (socklen_t*)&addr_len);

    recv(new_socket, buffer, sizeof(buffer), 0);
    printf("Client says: %s\n", buffer);

    char *msg = "Hello from Server";
    send(new_socket, msg, strlen(msg), 0);

    close(new_socket);
    close(server_socket);

    return 0;
}
