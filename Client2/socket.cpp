#include "socket.hpp"
#include "common.hpp"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstdio>
#include <fcntl.h>

ClientSocket client = ClientSocket();

// Create a TCP socket
ClientSocket::ClientSocket() {
    this->sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Socket creation error");
    }
    // set_nonblocking(this->sock);
}
ClientSocket::~ClientSocket() {
    close(sock);
}
void ClientSocket::connectToServer(const char *ip_address, int port){
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);

    if (inet_pton(AF_INET, ip_address, &serv_addr.sin_addr) <= 0) {
        perror("Invalid address/ Address not supported");
    }

    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("Connection failed");
    }
    int client_num = CLIENT_NUM;
    send(sock, &client_num, sizeof(client_num), 0);
}
// 소켓 파일 디스크립터를 논블로킹 모드로 설정
int set_nonblocking(int sockfd) {
    int flags = fcntl(sockfd, F_GETFL, 0);
    if (flags == -1) {
        perror("fcntl F_GETFL");
        return -1;
    }
    flags |= O_NONBLOCK;
    if (fcntl(sockfd, F_SETFL, flags) == -1) {
        perror("fcntl F_SETFL");
        return -1;
    }
    return 0;
}


