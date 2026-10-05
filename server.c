#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdbool.h>
#include <ctype.h>

#define PORT 8080
#define BUFFER_SIZE 1024
#define RESPONSE_SIZE 2048

int main(int argc, char const* argv[]) {
    int server_socket_fd, new_socket;
    struct sockaddr_in address, client_addr;
    socklen_t addrlen = sizeof(client_addr); 
    char buffer[BUFFER_SIZE] = {0};
    char response_buffer[RESPONSE_SIZE];
    int opt = 1;
    
    server_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    
    if (server_socket_fd < 0)
    {
        perror("Socket Error. Exiting...");
        return 1;
    }

    if (setsockopt(server_socket_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
    {
        perror("Socket opt setting error. Exiting...");
        close(server_socket_fd);
        return 1; 
    }
    
    
    memset(&address, 0 , sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_port = htons(PORT);

    if (bind(server_socket_fd, (struct sockaddr*)&address, sizeof(address)) < 0)
    {
        perror("Socket binding error. Exiting...");
        close(server_socket_fd);
        return 1; 
    }

    if (listen(server_socket_fd, 5) < 0 )
    {
        perror("Socket listening error. Exiting...");
        close(server_socket_fd);
        return 1; 
    }
    
    printf("Server started %d and listening port: %d... \n", server_socket_fd, PORT);
    


    while(true) {
        new_socket = accept(server_socket_fd, (struct sockaddr*)&client_addr, &addrlen);
        
        if (new_socket < 0)
        {
            perror("Socket accept error");
            close(server_socket_fd);
            return 1;
        }

        printf("Client successfully connected.\n");
        
        ssize_t bytes_read = recv(new_socket, buffer, sizeof(buffer) - 1, 0);

        if (bytes_read < 0)
        {
            perror("Reading error");
        } 
        else if (bytes_read == 0) 
        {
            perror("Client closed connection");
        } 
        else 
        {
            buffer[bytes_read] = '\0';
            buffer[strcspn(buffer, "\n\r")] = '\0';
            printf("Recieved from client: %s\n", buffer);

            for (size_t i = 0; i < (size_t)bytes_read; i++)
            {
                buffer[i] = (char)toupper((unsigned char)buffer[i]);
            }
            
            snprintf(response_buffer, sizeof(response_buffer), "%s Sended from server.\n", buffer);
            
            ssize_t bytes_sent = send(new_socket, response_buffer, strlen(response_buffer), 0);
            
            if (bytes_sent < 0)
            {
                perror("Server send error");
            }
        }
        
        close(new_socket);
    }

    close(server_socket_fd);
    return 0;
}