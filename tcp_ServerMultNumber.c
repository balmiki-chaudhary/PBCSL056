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
    int num1, num2, result;

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
    printf("Server is waiting for client...\n");

    // 5. Accept connection
    addr_len = sizeof(client_addr);
    client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &addr_len);

    // 6. Receive numbers
    read(client_fd, &num1, sizeof(num1));
    read(client_fd, &num2, sizeof(num2));

    printf("Received numbers: %d and %d\n", num1, num2);

    // 7. Multiply
    result = num1 * num2;

    // 8. Send result
    write(client_fd, &result, sizeof(result));

    // 9. Close sockets
    close(client_fd);
    close(server_fd);

    return 0;
}
