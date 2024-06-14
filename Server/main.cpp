#include <iostream>
#include <cstdlib>
#include <cstring>
#include <list>
#include "common.hpp"
#include "socket.hpp"
#include "thread.hpp"

#define PORT 8080
#define BUFFER_SIZE 1024

using namespace std;

list<Ball *> ballList;

int main()
{
    // 서버 소켓 생성
    ServerSocket server(PORT);
    server.bindSocket();
    server.listenSocket(3);
    int new_socket = server.acceptConnection();

    // 클라이언트로부터 명령 수신 스레드 생성
    ThreadArgs *args = new ThreadArgs;
    args->socket = new_socket;

    pthread_create(&receive, NULL, recv_cmd, args);
    pthread_create(&move_calculator, NULL, move_ball, NULL);

    pthread_join(receive, NULL);
    pthread_join(move_calculator, NULL);

    close(new_socket);
    return 0;
}
