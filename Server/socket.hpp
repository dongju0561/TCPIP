#ifndef SOCKET_HPP
#define SOCKET_HPP

#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <iostream>
#include <vector>
#include <thread>

using namespace std;

class ServerSocket {
public:
    int server_fd;
    struct sockaddr_in address;
    int opt;

    ServerSocket(int port);
    ~ServerSocket();
    void bindSocket();
    void listenSocket(int backlog);
    void acceptConnection(vector<int>& client_sockets, vector<thread>& threads);
};

#endif // SOCKET_HPP
