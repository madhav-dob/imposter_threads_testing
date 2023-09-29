#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <netinet/in.h>
#include <netinet/sctp.h>
#include <string.h>

#define MAX_MESSAGE_SIZE 100

// Function to be executed by Thread 1 (now the client)
void *thread1_func(void *arg) {
    // Create SCTP socket
    int sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_SCTP);
    if (sockfd == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(12345);  // Port number
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");  // IP address of Thread 2 (server)

    // Connect to the server
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Connect failed");
        exit(EXIT_FAILURE);
    }

    printf("Thread 1: Connected to Thread 2 (server).\n");
    while(1)
    {
    // Send a message to Thread 2 (server)
    char message[] = "Hello from Thread 1 (client)!";
    sctp_sendmsg(sockfd, message, sizeof(message), NULL, 0, 0, 0, 0, 0, 0);
    printf("sent a msg \n");
    sleep(3);
    }    


    // Close socket
    close(sockfd);

    return NULL;
}


int main() {
    pthread_t thread1;

    // Create Thread 1 (client)
    if (pthread_create(&thread1, NULL, thread1_func, NULL) != 0) {
        perror("Thread 1 creation failed");
        return EXIT_FAILURE;
    }

    // Wait for threads to finish
    pthread_join(thread1, NULL);

    return EXIT_SUCCESS;
}
