#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <netinet/in.h>
#include <netinet/sctp.h>
#include <string.h>

#define MAX_MESSAGE_SIZE 5000


// Function to be executed by Thread 2 (now the server)
void *thread2_func(void *arg) {
    // Create SCTP socket
    int sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_SCTP);
    if (sockfd == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Define the server address
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(12345);  // Port number
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket
    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    listen(sockfd, 5);

    printf("Thread 2: Listening for incoming SCTP connections...\n");

    // Accept a new SCTP connection
    struct sockaddr_in client_addr;
    socklen_t addr_len = sizeof(client_addr);
    int client_sockfd = accept(sockfd, (struct sockaddr *)&client_addr, &addr_len);
    if (client_sockfd == -1) {
        perror("Accept failed");
        exit(EXIT_FAILURE);
    }

    printf("Thread 2: SCTP connection established with Thread 1 (client).\n");


    while (1)
    {

    
        // Receive message from Thread 1 (client)
        char received_message[MAX_MESSAGE_SIZE];
        struct sctp_sndrcvinfo sndrcvinfo;
        socklen_t len = sizeof(struct sctp_sndrcvinfo);
        ssize_t bytes_received = sctp_recvmsg(client_sockfd, received_message, sizeof(received_message), NULL, 0, &sndrcvinfo, &len);
        if (bytes_received > 0) {
            received_message[bytes_received] = '\0';
            printf("Thread 2: Received message from Thread 1 (client): %s\n", received_message);
        }
    }

    // Close sockets
    close(client_sockfd);
    close(sockfd);

    return NULL;
}

int main() {
    pthread_t thread2;


    // Create Thread 2 (server)
    if (pthread_create(&thread2, NULL, thread2_func, NULL) != 0) {
        perror("Thread 2 creation failed");
        return EXIT_FAILURE;
    }

    // Wait for threads to finish
    pthread_join(thread2, NULL);

    return EXIT_SUCCESS;
}
