#include<iostream>
#include <arpa/inet.h>
#include "common.hpp"
#include "thread.hpp"

int sock = 0;
int data_available = false;
char buffer[BUFFER_SIZE];

pthread_cond_t buffer_cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t buffer_mutex = PTHREAD_MUTEX_INITIALIZER;

pthread_t input, processor;
ThreadArgs *args[BALL_NUM];

int thread_index = 0;

void *input_CMD(void *arg)
{
    while (true) 
    {
        char input[BUFFER_SIZE];
        // 표준 입력에서 문자열을 읽음
        if (fgets(input, BUFFER_SIZE, stdin) == NULL) {
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
//입력 받은 값을 처리하는 스레드 함수 
void *process_CMD(void *arg)
{
    const char *hello = "Hello from client";
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
            send(sock, hello, strlen(hello), 0);
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