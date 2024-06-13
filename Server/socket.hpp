#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <iostream>

class ServerSocket {
public:
    int server_fd;
    struct sockaddr_in address;
    int opt;

    ServerSocket(int port);
    ~ServerSocket();
    void bindSocket();
    void listenSocket(int backlog);
    int acceptConnection();
};

#endif // SOCKET_HPP
