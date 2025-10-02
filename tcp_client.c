/*
 * TCP CLIENT PROGRAM: Sends an array of 6 integers to the server and
 * receives the sorted array back.
 *
 * Steps to remember: socket() -> connect() -> write() -> read() -> close()
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8080
#define SERVER_IP "127.0.0.1" // Localhost
#define ARRAY_SIZE 6

int main() {
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    // The numbers to be sent to the server
    int numbers_to_send[ARRAY_SIZE] = {5, 10, 2, 4, 7, 3};
    // Buffer to hold the sorted numbers received from the server
    int sorted_numbers[ARRAY_SIZE] = {0};

    // 1. Create socket file descriptor
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("Socket creation error \n");
        return -1;
    }

    // Set up the server address structure
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 addresses from text to binary form (for SERVER_IP)
    if (inet_pton(AF_INET, SERVER_IP, &serv_addr.sin_addr) <= 0) {
        printf("Invalid address/ Address not supported \n");
        return -1;
    }

    // 2. Connect to the server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection Failed");
        return -1;
    }
    printf("Connected to server.\n");

    // Original numbers to show what was sent
    printf("Original numbers sent: ");
    for (int i = 0; i < ARRAY_SIZE; i++) {
        printf("%d ", numbers_to_send[i]);
    }
    printf("\n");

    // 3. Write (send) the array of numbers to the server
    send(sock, numbers_to_send, sizeof(numbers_to_send), 0);
    printf("Array sent to server for sorting.\n");

    // 4. Read (receive) the sorted array back from the server
    valread = read(sock, sorted_numbers, sizeof(sorted_numbers));

    if (valread == sizeof(sorted_numbers)) {
        printf("\nReceived sorted numbers from server: ");
        for (int i = 0; i < ARRAY_SIZE; i++) {
            printf("%d ", sorted_numbers[i]);
        }
        printf("\n");
    } else {
        printf("Error receiving data or received incorrect size.\n");
    }

    // 5. Close the connected socket
    close(sock);
    printf("Client closed.\n");

    return 0;
}

