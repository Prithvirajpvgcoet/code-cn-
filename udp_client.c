#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int client_socket;
    char buffer[1024];
    struct sockaddr_in server_addr;
    socklen_t addr_size = sizeof(server_addr);

    client_socket = socket(AF_INET, SOCK_DGRAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    char *msg = "Hello from UDP Client";
    sendto(client_socket, msg, strlen(msg), 0,
           (struct sockaddr*)&server_addr, addr_size);

    recvfrom(client_socket, buffer, sizeof(buffer), 0,
             (struct sockaddr*)&server_addr, &addr_size);

    printf("Server says: %s\n", buffer);

    close(client_socket);
    return 0;
}
