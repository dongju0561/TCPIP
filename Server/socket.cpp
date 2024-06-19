#include <vector>
#include <thread>
#include <iostream>
#include "socket.hpp"
#include "thread.hpp"

using namespace std;

ServerSocket::ServerSocket(int port) {
    opt = 1;
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);
}

ServerSocket::~ServerSocket() {
    close(server_fd);
}

void ServerSocket::bindSocket() {
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
}

void ServerSocket::listenSocket(int backlog) {
    if (listen(server_fd, backlog) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
}

void ServerSocket::acceptConnection(vector<int>& client_sockets, vector<thread>& threads) {
    int addrlen = sizeof(address);
    int new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
    if (new_socket < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    int int_buffer = 0;
    client_sockets.push_back(new_socket);
    recv(new_socket,&int_buffer,sizeof(int_buffer),0);
    cout << "Client" << int_buffer << " is connected" << endl;
    threads.push_back(thread(recv_cmd, new_socket));
    threads.push_back(thread(sync_list, new_socket));
}
