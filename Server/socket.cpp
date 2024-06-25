#include <vector>
#include <thread>
#include <iostream>
#include "socket.hpp"
#include "thread.hpp"
#include <fstream>
#include <fcntl.h>

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
    // Get the current time
    time_t now = time(0);
    char* timestamp;

    //파일 디스크립터를 담을 변수
    int new_fd = open("log.txt", O_WRONLY | O_APPEND | O_CREAT, 0644);
    if (new_fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    int addrlen = sizeof(address);
    int new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen);
    if (new_socket < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    int int_buffer = 0;
    client_sockets.push_back(new_socket);
    recv(new_socket,&int_buffer,sizeof(int_buffer),0);

    //클라이으트와 연결되었을때, 파일에 로그를 남김(날짜, 시간, 클라이언트 번호)
    timestamp = ctime(&now);
    timestamp[strlen(timestamp) - 1] = '\0'; // 줄바꿈 문자 제거
    char log_buffer[100];
    sprintf(log_buffer, "[%s] Client%d is connected\n", timestamp, int_buffer);
    write(new_fd, log_buffer, strlen(log_buffer));

    cout << "Client" << int_buffer << " is connected" << endl;

    threads.push_back(thread(recv_cmd, new_socket));
    threads.push_back(thread(sync_list, new_socket));
    
    close(new_fd);
}
