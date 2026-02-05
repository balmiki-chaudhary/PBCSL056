#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>

#define PORT 8080

int isVowel(char ch) {
    return (ch=='a'||ch=='e'||ch=='i'||ch=='o'||ch=='u'||
            ch=='A'||ch=='E'||ch=='I'||ch=='O'||ch=='U');
}

int main() {
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len;
    char buffer[1024], vowels[1024];
    int i, j = 0;

    // 1. Create socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("Socket failed");
        exit(1);
    }

    // 2. Server address setup
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

    // 6. Receive string
    read(client_fd, buffer, sizeof(buffer));
    printf("Received string: %s\n", buffer);

    // 7. Extract vowels
    for (i = 0; buffer[i] != '\0'; i++) {
        if (isVowel(buffer[i])) {
            vowels[j++] = buffer[i];
        }
    }
    vowels[j] = '\0';

    // 8. Send vowels back
    write(client_fd, vowels, strlen(vowels) + 1);

    // 9. Close sockets
    close(client_fd);
    close(server_fd);

    return 0;
}
