#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>

#define BUF_SIZE 1024

int main(int argc, char **argv)
{
    char buf[BUF_SIZE];
    struct sockaddr_in server, client;
    int server_fd, client_fd = -1, ret = 0;
    socklen_t client_len = sizeof(client);
    uint16_t port;

    if (argc < 2)
    {
        printf("Usage: %s <PORT>\n", argv[0]);
        return -1;
    }

    port = atoi(argv[1]);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0)
    {
        perror("Failed to create a server socket");
        return -2;
    }

    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = htonl(INADDR_ANY);

    int opt_val = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt_val, sizeof opt_val);

    if (bind(server_fd, (struct sockaddr *)&server, sizeof(server)) < 0)
    {
        perror("Failed to bind the server socket");
        ret = -3;
        goto finish;
    }

    if (listen(server_fd, 128) < 0)
    {
        perror("Failed to listen on the server socket");
        ret = -4;
        goto finish;
    }

    printf("Server is listening on %d\n", port);

    client_fd = accept(server_fd, (struct sockaddr *)&client, &client_len);

    if (client_fd < 0)
    {
        perror("Failed to establish connection with the client\n");
        ret = -5;
        goto finish;
    }

    while (1)
    {
        memset(buf, 0, BUF_SIZE);
        int read_count = recv(client_fd, buf, BUF_SIZE, 0);
        if (read_count == 0)
        {
            break;
        }
        if (read_count < 0)
        {
            perror("Failed to read from the client");
            ret = -6;
            goto finish;
        }

        printf("Received: %s\n", buf);

        if (send(client_fd, buf, read_count, 0) < 0)
        {
            perror("Failed to send data to the client");
            ret = -7;
            goto finish;
        }
    }

finish:
    if (client_fd >= 0)
    {
        close(client_fd);
    }
    close(server_fd);

    return ret;
}
