#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

#define PORT 8080

int main() {
    int sock;
    struct sockaddr_in server_addr;
    char strA[1024], strB[1024], result[2048];

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

    // 4. Input String A
    printf("Enter String A: ");
    fgets(strA, sizeof(strA), stdin);
    strA[strcspn(strA, "\n")] = '\0';

    // 5. Input String B
    printf("Enter String B: ");
    fgets(strB, sizeof(strB), stdin);
    strB[strcspn(strB, "\n")] = '\0';

    // 6. Send strings
    write(sock, strA, strlen(strA) + 1);
    write(sock, strB, strlen(strB) + 1);

    // 7. Receive concatenated result
    read(sock, result, sizeof(result));
    printf("Concatenated String: %s\n", result);

    // 8. Close socket
    close(sock);

    return 0;
}
