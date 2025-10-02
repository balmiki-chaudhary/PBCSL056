#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 8081
#define SERVER_IP "127.0.0.1" // Localhost

// Structure to hold the two variables for transmission (must match the server)
struct Payload {
    int a;
    int b;
};

int main() {
    int sockfd;
    struct sockaddr_in servaddr;
    struct Payload send_data, received_data;
    socklen_t len;

    // The values specified by the problem:
    int a_val = 15;
    int b_val = 20;

    // 1. Create socket file descriptor (AF_INET = IPv4, SOCK_DGRAM = UDP)
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Set server address to zero
    memset(&servaddr, 0, sizeof(servaddr));

    // Configure server address
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    // Convert IP address string to binary form
    if (inet_pton(AF_INET, SERVER_IP, &servaddr.sin_addr) <= 0) {
        printf("Invalid address/ Address not supported \n");
        close(sockfd);
        return -1;
    }

    // Prepare data to send
    send_data.a = a_val;
    send_data.b = b_val;
    printf("Original values to send: a = %d, b = %d\n", send_data.a, send_data.b);

    // 2. Send the data to the server
    len = sizeof(servaddr);
    sendto(sockfd, &send_data, sizeof(struct Payload),
           0, (const struct sockaddr *)&servaddr, len);

    printf("Datagram sent to server.\n");

    // 3. Receive the reply datagram from the server
    int n = recvfrom(sockfd, &received_data, sizeof(struct Payload),
                     0, (struct sockaddr *)&servaddr, &len);

    if (n < 0) {
        perror("Recvfrom failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("\n--- Results Received ---\n");
    printf("Received swapped values: a = %d, b = %d\n", received_data.a, received_data.b);
    printf("------------------------\n");

    // 4. Close the socket
    close(sockfd);
    printf("Client closed.\n");

    return 0;
}

