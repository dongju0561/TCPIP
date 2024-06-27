#include <iostream>
#include <fstream>
#include <string>
#include <arpa/inet.h>
#include <list>
#include <vector>
#include <thread>
#include "fbDraw.hpp"
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
pthread_cond_t buffer_cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t buffer_mutex = PTHREAD_MUTEX_INITIALIZER;

pthread_t input, processor, sync_t, print_ball, monitor, erase_all_ball_v, fb_fill_background_thread;
ThreadArgs *args[BALL_NUM];
extern int num_of_list_element;

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
            pkt.opt_num = -1;
            if (buffer[1] == ':')
            {
                // 인덱스 두번째부터 마지막 인덱스의 문자를 인식하여 정수로 변화 후 pkt.opt_num에 저장
                pkt.opt_num = atoi(buffer + 2);
            }
            send(client.sock, &pkt, sizeof(packet), 0);
            break;
        case 'd':
            pkt.cmd[0] = 'd';
            pkt.client_num = CLIENT_NUM;
            pkt.opt_num = -1;
            if (buffer[1] == ':')
            {
                // 인덱스 두번째부터 마지막 인덱스의 문자를 인식하여 정수로 변화 후 pkt.opt_num에 저장
                pkt.opt_num = atoi(buffer + 2);
            }
            send(client.sock, &pkt, sizeof(pkt), 0);
            break;
        case 'x':
            pkt.cmd[0] = 'x';
            send(client.sock, &pkt, sizeof(pkt), 0);
            cout << "프로그램 종료" << endl;
            sleep(1);
            exit(0);
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
    sync_packet pkt;
    int recved_idx;
    list<Ball *>::iterator it;
    while (true)
    {
        // 클라이언트 리스트와 서버 리스트 동기화

        // ball = new Ball;

        recv(client.sock, &pkt, sizeof(sync_packet), 0);
        switch (pkt.pkt_type)
        {
        case 0:
            
            num_of_server_list_element = pkt.list_size;

            pthread_mutex_lock(&list_mutex);
            ballList.clear();
            for (int i = 0; i < num_of_list_element; i++)
            {
                ballList.push_back(NULL);
            }
            pthread_mutex_unlock(&list_mutex);
            break;
        case 1:
            ball = new Ball;
            memcpy(ball, &pkt.ball, sizeof(Ball));
            recved_idx = ball->idx;

            pthread_mutex_lock(&list_mutex);
            it = ballList.begin();
            advance(it, recved_idx);
            *it = ball;
            pthread_mutex_unlock(&list_mutex);
            break;
        default:
            break;
        }
    }
    return NULL;
}

void *fb_print_ball(void *arg)
{
    while (true)
    {
        list<Ball *> ballsToProcess;


        pthread_mutex_lock(&list_mutex);
        if (!ballList.empty())
        {
            ballsToProcess = ballList;
        }
        pthread_mutex_unlock(&list_mutex);


        if (ballsToProcess.empty())
        {
            continue;
        }
        for (list<Ball *>::iterator it = ballsToProcess.begin(); it != ballsToProcess.end(); ++it)
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
            int idx = ball->idx;

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
void *fb_fill_background(void *arg)
{
    while (true)
    {
        fb_fillScr(&fb, 255, 255, 255);
        usleep(80000); // 5ms 대기
    }
    return NULL;
}