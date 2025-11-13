#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int server_socket;
    char buffer[1024];
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size = sizeof(client_addr);

    server_socket = socket(AF_INET, SOCK_DGRAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr));

    recvfrom(server_socket, buffer, sizeof(buffer), 0,
             (struct sockaddr*)&client_addr, &addr_size);

    printf("Client says: %s\n", buffer);

    char *msg = "Hello from UDP Server";
    sendto(server_socket, msg, strlen(msg), 0,
           (struct sockaddr*)&client_addr, addr_size);

    close(server_socket);
    return 0;
}
