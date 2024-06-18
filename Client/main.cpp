#include <iostream>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <list>
#include <pthread.h> // pthread 사용을 위한 헤더
#include "fbDraw.hpp" // fbDraw 클래스 헤더
#include "thread.hpp"
#include "socket.hpp" // socket 관련 함수 정의가 있는 헤더

#define PORT 8080
#define BUFFER_SIZE 1024

using namespace std;

// 전역 변수로 client socket 객체 선언
extern ClientSocket client;

list<Ball *> ballList;

int main() {
    //fb 초기화
    printf("fb init\n");
    int result = fb_init(&fb);
    if (result != 0) 
    {
        fprintf(stderr, "Framebuffer initialization failed with error code %d\n", result);
        exit(1);
    }

    fb_fillScr(&fb, 255, 255, 255);

    // 소켓 생성
    client.connectToServer("127.0.0.1",PORT);

    // 입력 및 처리 쓰레드 생성
    pthread_create(&input, NULL, input_CMD, NULL);
    pthread_create(&processor, NULL, process_CMD, NULL);
    pthread_create(&sync_t, NULL, sync_list, NULL);
    pthread_create(&fb_fill_background_thread, NULL, fb_fill_background, NULL);

    // 쓰레드 종료 대기
    pthread_join(input, NULL);
    pthread_join(processor, NULL);
    pthread_join(sync_t, NULL);
    pthread_join(fb_fill_background_thread, NULL);
    for (int i = 0; i < BALL_NUM; i++)
    {
        pthread_join(ball_thread[i], NULL);
    }

    // 소켓 닫기
    close(client.sock);
    fb_close(&fb);

    return 0;
}
