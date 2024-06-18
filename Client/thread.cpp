#include <iostream>
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

pthread_cond_t list_cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t list_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t buffer_cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t buffer_mutex = PTHREAD_MUTEX_INITIALIZER;

pthread_t input, processor, sync_t, ball_thread[BALL_NUM], fb_fill_background_thread;
int num_of_thread = 0;
int thread_index = 0;
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
            
            ballList.push_back(NULL);
            
            //ballList 출력해줘

            idx = thread_index;
            pthread_create(&ball_thread[idx], NULL, fb_print_ball, (void *)&idx);
            thread_index += 1;
            break;
        case 'd':
            send(client.sock, buffer, sizeof(buffer), 0);
            pthread_cancel(ball_thread[--thread_index]);
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
    // list 요소 하나와 thread를 맵핑하여 fb에 출력
    int idx = *(int*)arg;
    list<Ball *>::iterator it;
    while (true)
    {
        // 공의 위치를 화면에 출력
        it = ballList.begin();
        advance(it, idx);
        Ball *ball = *it;
        // 아직 공 비어있으면 대기
        if (ball == NULL)
        {
            continue;
        }
        // 현재 좌표를 검은색으로 칠함
        pixel cur_pixel = ball->pos;
        if ((cur_pixel.x < 0 || cur_pixel.x > fb.vinfo.xres) && (cur_pixel.y < 0 || cur_pixel.y > fb.vinfo.yres))
        {
            continue;
        }
        fb_drawFilledCircle(&fb, cur_pixel, 0, 0, 0);
    }
    return NULL;
}

void *fb_fill_background(void *arg)
{
    while (true)
    {
        fb_fillScr(&fb, 255, 255, 255);
        usleep(5000);// 5ms 대기
    }
    return NULL;
}
