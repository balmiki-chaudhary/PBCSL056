
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define PORT 8080
#define ARRAY_SIZE 6

// Function to sort the array in DESCENDING order
void sortDescending(int arr[], int n) {
    int i, j, temp;
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - i - 1; j++) {
            // Compare j and j+1. If j is smaller, swap them (for descending order).
            if (arr[j] < arr[j + 1]) {
                temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

int main() {
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    // Buffer to hold the 6 incoming integers
    int numbers[ARRAY_SIZE] = {0};

    // 1. Create socket file descriptor (AF_INET = IPv4, SOCK_STREAM = TCP)
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Set up the server address structure
    address.sin_family = AF_INET;
    // INADDR_ANY means listen on all available network interfaces
    address.sin_addr.s_addr = INADDR_ANY;
    // Convert port number to network byte order
    address.sin_port = htons(PORT);

    // 2. Bind the socket to the specified IP and PORT
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // 3. Start listening for client connections (up to 3 pending connections)
    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }
    printf("Server listening on port %d. Waiting for client...\n", PORT);

    // 4. Accept the first connection from the queue. This is a blocking call.
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }
    printf("Connection established with client.\n");

    // 5. Read the numbers from the client. Read exactly the size of the array.
    valread = read(new_socket, numbers, sizeof(numbers));
    if (valread == sizeof(numbers)) {
        printf("Received %d bytes. Numbers received: ", valread);
        for (int i = 0; i < ARRAY_SIZE; i++) {
            printf("%d ", numbers[i]);
        }
        printf("\n");
    } else {
        printf("Error reading data or received incorrect size.\n");
    }

    // 6. Sort the numbers in descending order
    sortDescending(numbers, ARRAY_SIZE);

    printf("Numbers sorted in descending order: ");
    for (int i = 0; i < ARRAY_SIZE; i++) {
        printf("%d ", numbers[i]);
    }
    printf("\n");

    // 7. Write (send) the sorted array back to the client
    send(new_socket, numbers, sizeof(numbers), 0);
    printf("Sent sorted array back to client.\n");

    // 8. Close the connection socket and the main server socket
    close(new_socket);
    close(server_fd);
    printf("Server closed.\n");

    return 0;
}

