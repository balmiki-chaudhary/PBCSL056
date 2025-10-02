#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define PORT 8081
#define MAX_BUFFER_SIZE 1024

// Structure to hold the two variables for transmission
struct Payload {
    int a;
    int b;
};

int main() {
    int sockfd;
    struct sockaddr_in servaddr, cliaddr;
    struct Payload received_data;
    struct Payload swapped_data;
    socklen_t len = sizeof(cliaddr);

    // 1. Create socket file descriptor (AF_INET = IPv4, SOCK_DGRAM = UDP)
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Set server address to zero
    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));

    // Configure server address
    servaddr.sin_family = AF_INET;        // IPv4
    servaddr.sin_addr.s_addr = INADDR_ANY; // Listen on any interface
    servaddr.sin_port = htons(PORT);     // Port number

    // 2. Bind the socket to the specified IP and PORT
    if (bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    printf("UDP Server is running on port %d. Waiting for data...\n", PORT);

    // 3. Wait to receive the datagram from the client
    // recvfrom is a blocking call, waiting for a packet
    int n = recvfrom(sockfd, &received_data, sizeof(struct Payload),
                     0, (struct sockaddr *)&cliaddr, &len);

    if (n < 0) {
        perror("Recvfrom failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    printf("Received datagram: a = %d, b = %d\n", received_data.a, received_data.b);

    // 4. Swap the values (The core task)
    swapped_data.a = received_data.b; // a gets the value of b (20)
    swapped_data.b = received_data.a; // b gets the value of a (15)

    printf("Swapping values... New values: a = %d, b = %d\n", swapped_data.a, swapped_data.b);

    // 5. Send the swapped data back to the client
    // We use the 'cliaddr' captured by recvfrom() to know where to send the reply
    sendto(sockfd, &swapped_data, sizeof(struct Payload),
           0, (const struct sockaddr *)&cliaddr, len);
    
    printf("Swapped values sent back to client.\n");

    // 6. Close the socket
    close(sockfd);
    printf("Server closed.\n");

    return 0;
}

