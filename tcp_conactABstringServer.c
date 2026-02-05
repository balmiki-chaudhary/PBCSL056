#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

#define PORT 8080

int main() {
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len;

    char strA[1024], strB[1024], result[2048];

    // 1. Create socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    // 2. Server address configuration
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // 3. Bind
    bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));

    // 4. Listen
    listen(server_fd, 5);
    printf("Server waiting for client...\n");

    // 5. Accept connection
    addr_len = sizeof(client_addr);
    client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &addr_len);

    // 6. Receive String A
    read(client_fd, strA, sizeof(strA));

    // 7. Receive String B
    read(client_fd, strB, sizeof(strB));

    printf("Received String A: %s\n", strA);
    printf("Received String B: %s\n", strB);

    // 8. Concatenate strings
    strcpy(result, strA);
    strcat(result, strB);

    // 9. Send result
    write(client_fd, result, strlen(result) + 1);

    // 10. Close sockets
    close(client_fd);
    close(server_fd);

    return 0;
}
