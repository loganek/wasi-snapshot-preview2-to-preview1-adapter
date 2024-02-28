#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>

#define BUF_SIZE 1024
#define TEST_STRING "Hello, world!"

int main(int argc, char**argv)
{
    char buf[BUF_SIZE] = TEST_STRING;
    int sockfd, ret = 0;
    struct sockaddr_in servaddr;
    uint16_t port;

    if (argc < 3)
    {
        printf("Usage: %s <IP> <PORT>\n", argv[0]);
        return -1;
    }

    port = atoi(argv[2]);

    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);

    if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) != 1)
    {
        printf("Failed to parse IP address %s\n", argv[1]);
        return -2;
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd == -1)
    {
        perror("Failed to create a socket");
        return -3;
    }

    if (connect(sockfd, (struct sockaddr *)&servaddr, sizeof(servaddr)) != 0)
    {
        perror("Failed to connect to a server");
        ret = -3;
        goto finish;
    }

    printf("Connected to the server\n");

    if (send(sockfd, buf, strlen(buf), 0) < 0)
    {
        perror("Failed to write to socket");
        ret = -4;
        goto finish;
    }
    memset(buf, 0, sizeof(buf));
    if (recv(sockfd, buf, sizeof(buf), 0) <  0) {
        perror("Failed to receive data from server");
        ret = -5;
        goto finish;
    }

    printf("Response from server: %s", buf);

    if (strncmp(buf, TEST_STRING, strlen(TEST_STRING)) != 0)
    {
        ret = 1;
        printf("Expected %s from the echo server but have %s", TEST_STRING, buf);
    }

finish:
    close(sockfd);
    return ret;
}
