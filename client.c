#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#define PORT 8080

int main(int argc, char *argv[]) {
    int client_fd;

    if (argc < 3) 
    {
        perror("Invalid argument quantity.");
        return 1;
    }

    int port = atoi(argv[2]);
    int (port <= )

    if (client_fd = socket(AF_INET, SOCK_STREAM, 0) < 0)
    {
        perror("Socket creation error");
    }
    

}