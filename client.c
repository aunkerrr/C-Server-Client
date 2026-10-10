#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdbool.h>
#define BUFFER_SIZE 1024
#define RECIEVE_BUFFER_SIZE 2048

int main(int argc, char *argv[]) {
    int client_fd;
    int port = atoi(argv[2]);
    struct sockaddr_in server_addr;

    if (argc < 3) 
    {
        perror("Invalid argument quantity.");
        return 1;
    }

    if (port <= 0 || port > 65535)
    {
        perror("Invalid port. Choose in range of 0 to 65535");
        return 1;
    }

    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Socket creation error");
        return 1;
    }
    
    memset(&server_addr, 0, sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);

    if (inet_pton(AF_INET, argv[1], &server_addr.sin_addr) <= 0)
    {
        perror("Wrong IP address.");
        close(client_fd);
        return 1;
    }

    if (connect(client_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0)
    {
        perror("Connection error.");
        close(client_fd);
        return 1;
    }
    
    printf("Connection with server successfully opened %s:%d.\n", argv[1], port);

    char buffer[BUFFER_SIZE] = {0};
    printf("Insert a string to send it.\n");
    if (fgets(buffer, sizeof(buffer), stdin) == NULL)
    {
        perror("Error reading user input.");
        close(client_fd);
        return 1;
    }
    
    ssize_t bytes_send = send(client_fd, buffer, sizeof(buffer) - 1, 0);
    if (bytes_send < 0)
    {
        perror("Error while sending data");
        close(client_fd);
        return 1;
    }

    char recieve_buffer[RECIEVE_BUFFER_SIZE] = {0};
    ssize_t bytes_recieved = recv(client_fd, recieve_buffer, sizeof(recieve_buffer) - 1, 0);
    if (bytes_recieved < 0)
    {
        perror("Error reciveving server output");
        close(client_fd);
        return 1;
    }

    recieve_buffer [bytes_recieved] = '\0';
    printf("%s", recieve_buffer);

    close(client_fd);
    return 0;
}