#include <iostream>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <list>
#include "common.hpp"
#include "socket.hpp"
#include "thread.hpp"
#include <thread>

#define PORT 8080
#define BUFFER_SIZE 1024

using namespace std;

list<Ball *> ballList;
vector<int> client_sockets;
vector<thread> threads;
extern pthread_t ball_move_threads[BALL_NUM];
int main()
{
    // 서버 소켓 생성
    ServerSocket server(PORT);
    server.bindSocket();
    server.listenSocket(3);

    threads.push_back(thread(keep_accept, ref(server), ref(client_sockets), ref(threads)));
    threads.push_back(thread(monitor_list));
    
    //스레드 종료 대기
    for(auto it = threads.begin(); it != threads.end(); ++it)
    {
        it->join();
    }

    //client_sockets 파일디스트립터 닫기
    for(auto it = client_sockets.begin(); it != client_sockets.end(); ++it)
    {
        close(*it);
    }
    for(int i = 0; i < BALL_NUM; i++)
    {
        pthread_join(ball_move_threads[i], NULL);
    }
    return 0;
}
