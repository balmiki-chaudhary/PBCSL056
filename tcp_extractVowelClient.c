#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

#define PORT 8080

int main() {
    int sock;
    struct sockaddr_in server_addr;
    char buffer[1024], result[1024];

    // 1. Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Socket failed");
        exit(1);
    }

    // 2. Server address setup
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);

    // 3. Connect to server
    connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr));

    // 4. Input string
    printf("Enter a string: ");
    fgets(buffer, sizeof(buffer), stdin);

    // Remove newline
    buffer[strcspn(buffer, "\n")] = '\0';

    // 5. Send string
    write(sock, buffer, strlen(buffer) + 1);

    // 6. Receive vowels
    read(sock, result, sizeof(result));
    printf("Vowels received from server: %s\n", result);

    // 7. Close socket
    close(sock);

    return 0;
}
