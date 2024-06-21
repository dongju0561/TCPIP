#include <iostream>
#include <fstream>
#include <string>
#include <arpa/inet.h>
#include <list>
#include "fbDraw.hpp"
#include "socket.hpp"
#include "common.hpp"
#include "thread.hpp"

using namespace std;

extern ClientSocket client;
extern list<Ball *> ballList;
int data_available = false;
char buffer[BUFFER_SIZE];

ofstream log_file("log.txt", ios::app);

pthread_cond_t list_cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t list_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t buffer_cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t buffer_mutex = PTHREAD_MUTEX_INITIALIZER;

pthread_t input, processor, sync_t, ball_thread[BALL_NUM];
ThreadArgs *args[BALL_NUM];

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
    int idx;
    packet pkt;

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
            pkt.cmd[0] = 'a';
            pkt.client_num = CLIENT_NUM;
            send(client.sock, &pkt, sizeof(packet), 0);
            
            ballList.push_back(NULL);
            pthread_create(&ball_thread[idx], NULL, fb_print_ball, (void *)&idx);
            break;
        case 'd':
            send(client.sock, buffer, sizeof(buffer), 0);
            ballList.back() = NULL;
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
        ball = new Ball;

        recv(client.sock, ball, sizeof(Ball), 0);

        //idx로 advace를 이용하여 ballList에 추가
        int idx = ball->idx;
        list<Ball *>::iterator it = ballList.begin();
        advance(it, idx);
        *it = ball;
        // 수신된 데이터 처리
        pthread_mutex_unlock(&list_mutex);
        // mutex unlock
    }
    return NULL;
}
void *fb_print_ball(void *arg)
{
    while(true){
        list<Ball *>::iterator it;
        for (it = ballList.begin(); it != ballList.end(); it++)
        {
            Ball *ball = *it;
            if (ball == NULL)
            {
                continue;
            }
            if (ball->pos.x == -1 || ball->pos.y == -1 || ball->pos.x == 1 || ball->pos.y == 1)
            {
                continue;
            }
            pixel cur_pixel = ball->pos;
            switch (ball->client_num)
            {
            case 1:
                fb_drawFilledCircle(&fb, cur_pixel, 255, 0, 0);
                break;
            case 2:
                fb_drawFilledCircle(&fb, cur_pixel, 0, 255, 0);
                break;
            case 3:
                fb_drawFilledCircle(&fb, cur_pixel, 0, 0, 255);
                break;
            default:
                break;
            }
        }
    }
    return NULL;
}
