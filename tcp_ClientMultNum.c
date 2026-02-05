#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

#define PORT 8080

int main() {
    int sock;
    struct sockaddr_in server_addr;
    int num1, num2, result;

    // 1. Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    // 2. Server address configuration
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);

    // 3. Connect to server
    connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr));

    // 4. Input numbers
    printf("Enter two numbers: ");
    scanf("%d %d", &num1, &num2);

    // 5. Send numbers
    write(sock, &num1, sizeof(num1));
    write(sock, &num2, sizeof(num2));

    // 6. Receive result
    read(sock, &result, sizeof(result));
    printf("Multiplication result = %d\n", result);

    // 7. Close socket
    close(sock);

    return 0;
}
