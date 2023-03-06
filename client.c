#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <math.h>

#define MAXLINE 1024
#define RETRY_MAX 10
#define BASE_WAIT 500
#define MULTIPLIER 2

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Usage: %s <server-ip> <server-port> <message>\n", argv[0]);
        return 1;
    }

    const char *server_ip = argv[1];
    int server_port = atoi(argv[2]);
    const char *message = argv[3];

    // Create UDP socket
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        printf("Socket creation failed: %s\n", strerror(errno));
        return 1;
    }

    // Set server address
    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(server_ip);
    servaddr.sin_port = htons(server_port);

    // Set timeout for recvfrom()
    struct timeval tv;
    tv.tv_sec = 1;  // timeout value in seconds
    tv.tv_usec = 0;
    if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (const char *)&tv, sizeof(tv)) < 0) {
        printf("Error setting socket options: %s\n", strerror(errno));
        close(sockfd);
        return 1;
    }

    // Send message to server
    int n;
    int retry = 0;
    int wait_time = BASE_WAIT;
    do {
        n = sendto(sockfd, (const char *)message, strlen(message), MSG_CONFIRM, (const struct sockaddr *)&servaddr, sizeof(servaddr));
        if (n < 0) {
            printf("Send failed: %s\n", strerror(errno));
        } else {
            printf("Sent message: %s\n", message);
            char buffer[MAXLINE];
            socklen_t len = sizeof(servaddr);

            // Receive message from server with timeout
            n = recvfrom(sockfd, (char *)buffer, MAXLINE, MSG_WAITALL, (struct sockaddr *)&servaddr, &len);
            if (n < 0 && errno == EAGAIN) {
                printf("Timeout waiting for response. Retrying in %d ms...\n", wait_time);
                usleep(wait_time * 1000);  // wait_time in microseconds
                retry++;
                wait_time = BASE_WAIT * pow(MULTIPLIER, retry);
            } else {
                buffer[n] = '\0';
                printf("Received echo: %s\n", buffer);
                close(sockfd);
                return 0;
            }
        }
    } while (retry < RETRY_MAX);

    printf("Maximum number of retries reached. Exiting...\n");
    close(sockfd);
    return 1;
}
