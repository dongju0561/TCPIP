#include <iostream>
#include <arpa/inet.h>
#include <list>
#include "socket.hpp"
#include "common.hpp"
#include "thread.hpp"

using namespace std;

extern ClientSocket client;
extern list<Ball *> ballList;
int data_available = false;
char buffer[BUFFER_SIZE];

pthread_cond_t list_cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t list_mutex = PTHREAD_MUTEX_INITIALIZER;

pthread_t input, processor;
ThreadArgs *args[BALL_NUM];

int thread_index = 0;

void *input_CMD(void *arg)
{
    while (true)
    {
        char input[BUFFER_SIZE];
        // 표준 입력에서 문자열을 읽음
        if (fgets(input, BUFFER_SIZE, stdin) == NULL)
        {
            break;
        }

        // 입력 데이터에 대한 잠금 획득
        pthread_mutex_lock(&buffer_mutex);

        // 공유 버퍼에 입력 데이터 복사
        strncpy(buffer, input, BUFFER_SIZE);
        data_available = true;

        // 버퍼가 채워졌음을 출력 스레드에 알림
        pthread_cond_signal(&buffer_cond);

        // 잠금 해제
        pthread_mutex_unlock(&buffer_mutex);
    }
    return NULL;
}
// 입력 받은 값을 처리하는 스레드 함수
void *process_CMD(void *arg)
{
    while (true)
    {
        // 잠금 획득
        pthread_mutex_lock(&buffer_mutex);

        // 데이터가 있을 때까지 대기
        while (!data_available)
        {
            pthread_cond_wait(&buffer_cond, &buffer_mutex);
        }

        switch (buffer[0])
        {
        case 'a':
            send(client.sock, buffer, sizeof(buffer), 0);
            break;
        case 'd':
            send(client.sock, buffer, sizeof(buffer), 0);
            break;
        case 'c':
            send(client.sock, buffer, sizeof(buffer), 0);
            break;
        default:
            break;
        }

        // 데이터가 처리되었음을 표시
        data_available = false;

        // 잠금 해제
        pthread_mutex_unlock(&buffer_mutex);
    }
    return NULL;
}

// 서버 리스트로부터 전달 받은 데이터로 클라이언트 리스트 동기화
void *sync_list(void *arg)
{
    Ball *ball;
    while (true)
    {
        // 클라이언트 리스트와 서버 리스트 동기화
        // mutex lock
        pthread_mutex_lock(&list_mutex);
        recv(client.sock, ball, sizeof(Ball), 0);

        const int bufferSize = 1024; // 필요한 크기로 설정, 예: 최대 1024개의 Ball 객체
        char buffer[bufferSize * sizeof(Ball)];

        // 소켓에서 데이터 수신
        recv(client.sock, buffer, sizeof(buffer), 0);

        pthread_mutex_unlock(&list_mutex);
        // mutex unlock
    }
    return NULL;
}