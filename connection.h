#ifndef CHESSGAME_CONNECTION_H
#define CHESSGAME_CONNECTION_H

#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#define PORT 8080

int connection_type; //1 - server; 2 - client

int server_fd, new_socket, valread;
struct sockaddr_in address;
int opt = 1;
int addrlen = sizeof(address);
char buffer[10000] = {0};

int sock = 0, valread;
struct sockaddr_in serv_addr;

int connect_as_server() {
    // Creating socket file descriptor
    connection_type = 1;
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 8080
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                   &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );

    // Forcefully attaching socket to the port 8080
    if (bind(server_fd, (struct sockaddr *)&address,
             sizeof(address))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    printf("Waiting for connection...\n");
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                             (socklen_t*)&addrlen))<0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    printf("Successfully connected!\n");
    return 0;
}

int connect_as_client() {
    connection_type = 2;
    char buffer[1024] = {0};
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }

    memset(&serv_addr, '0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses from text to binary form
    char serv_addr_str[200];
    printf("Enter server adress: ");
    scanf("%s", serv_addr_str);
    if(inet_pton(AF_INET, serv_addr_str, &serv_addr.sin_addr)<=0)
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
        printf("\nConnection Failed \n");
        return -1;
    }
    printf("Successfully connected!");
    return 0;
}

int send_as_client(char* s) {
    send(sock, s, strlen(s), 0);
    return 0;
}

int send_as_server(char* s) {
    send(new_socket, s, strlen(s), 0);
    return 0;
}

int my_send(char* s) {
    if (connection_type == 1)
        return send_as_server(s);
    return send_as_client(s);
}

char* receive_as_server() {
    valread = read( new_socket , buffer, 1024);
    return buffer;
}

char* receive_as_client() {
    valread = read( sock , buffer, 1024);
    return buffer;
}

char* my_receive() {
    if (connection_type == 1)
        return receive_as_server();
    return receive_as_client();
}

int my_connect() {
    char s[200];
    printf("Enter type of connection(c/s): ");
    scanf("%s", s);
    if (s[0] == 's' || s[0] == 'S')
        return connect_as_server();
    if (s[0] == 'c' || s[0] == 'C')
        return connect_as_client();
}



#endif //CHESSGAME_CONNECTION_H
