#include <iostream>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <list>
#include "thread.hpp"
#include <pthread.h> // pthread 사용을 위한 헤더
#include "socket.hpp" // socket 관련 함수 정의가 있는 헤더

#define PORT 8080
#define BUFFER_SIZE 1024

using namespace std;

// 전역 변수로 client socket 객체 선언
extern ClientSocket client;

list<Ball *> ballList;

int main() {
    // 소켓 생성
    client.connectToServer("127.0.0.1",PORT);

    // 입력 및 처리 쓰레드 생성
    pthread_create(&input, NULL, input_CMD, NULL);
    pthread_create(&processor, NULL, process_CMD, NULL);

    // 쓰레드 종료 대기
    pthread_join(input, NULL);
    pthread_join(processor, NULL);

    // 소켓 닫기
    close(client.sock);

    return 0;
}
