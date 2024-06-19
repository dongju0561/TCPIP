#include <iostream>
#include <arpa/inet.h>
#include <list>
#include <chrono>
#include <thread>
#include "common.hpp"
#include "thread.hpp"
#include "socket.hpp"

using namespace std;

extern list<Ball *> ballList;
int data_available = false;
char buffer[CMD_BUFFER_SIZE];
pthread_cond_t list_cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t list_mutex = PTHREAD_MUTEX_INITIALIZER;

pthread_t receive, move_calculator, sync_t;
ThreadArgs *args[BALL_NUM];
int thread_index = 0;
int ball_index = 0;

// 공 객체 생성 함수
Ball *createBall(int client_num)
{
    // 랜덤으로 x, y, dx, dy 생성
    Ball *ball = new Ball;
    ball->idx = ball_index++;
    ball->client_num = client_num;
    ball->pos.x = rand() % 1281;
    ball->pos.y = rand() % 801;
    ball->speed.dx = rand() % 2 == 0 ? 1 : -1;
    ball->speed.dy = rand() % 2 == 0 ? 1 : -1;

    return ball;
}

// 공 객체의 위치를 업데이트하는 스레드 함수
void move_ball()
{
    while (true)
    {
        // 리스트에 있는 모든 공 객체의 위치를 업데이트
        // mutex lock
        pthread_mutex_lock(&list_mutex);
        
        for (auto it = ballList.begin(); it != ballList.end(); ++it)
        {
            (*it)->pos.x += (*it)->speed.dx;
            (*it)->pos.y += (*it)->speed.dy;

            // 경계를 넘어가면 방향 전환
            if ((*it)->pos.x <= 0 || (*it)->pos.x >= 1280)
            {
                (*it)->speed.dx *= -1;
            }
            if ((*it)->pos.y <= 0 || (*it)->pos.y >= 800)
            {
                (*it)->speed.dy *= -1;
            }
        }
        pthread_cond_signal(&list_cond);
        pthread_mutex_unlock(&list_mutex);
        usleep(400); // 90ms 대기
        // mutex unlock
        
    }
}

// 입력 받은 값을 처리하는 스레드 함수
void recv_cmd(int client_socket)
{
    int new_socket = client_socket;

    Ball *newBall;
    packet pkt;

    // 클라이언트로부터 명령 수신
    while (true)
    {
        memset(&pkt, 0, sizeof(packet)); // 버퍼 초기화
        int valread = recv(new_socket, &pkt, sizeof(packet), 0);
        if (valread <= 0)
        {
            break; // 연결이 끊어지거나 에러 발생 시 루프 탈출
        }
        switch (pkt.cmd[0])
        {
        case 'a':
            // 공 객체 생성
            //전달 받은 클라이언트 번호를 인자로 전달
            newBall = createBall(pkt.client_num);

            // 리스트 추가 시에는 클라이어트 전송 mutex 필요
            // mutex lock
            pthread_mutex_lock(&list_mutex);
            ballList.push_back(newBall);
            pthread_mutex_unlock(&list_mutex);
            // mutex unlock

            // 리스트 현황 출력
            cout << "ballList size: " << ballList.size() << endl;
            for (auto it = ballList.begin(); it != ballList.end(); ++it)
            {
                cout << "Ball Position: " << (*it)->pos.x << ", " << (*it)->pos.y
                     << " | Speed: " << (*it)->speed.dx << ", " << (*it)->speed.dy
                     << "Ball Index: " << (*it)->idx << "Ball type" << (int)(*it)->client_num << endl;
            }
            break;
        case 'd':
            // 리스트에서 마지막 객체 삭제
            if (!ballList.empty())
            {
                // 리스트 삭제 시에는 클라이어트 전송 mutex 필요
                // mutex lock
                pthread_mutex_lock(&list_mutex);
                ball_index--;
                delete ballList.back();
                ballList.pop_back();
                pthread_mutex_unlock(&list_mutex);
                // mutex unlock
            }

            // 리스트 현황 출력
            cout << "ballList size: " << ballList.size() << endl;
            for (auto it = ballList.begin(); it != ballList.end(); ++it)
            {
                cout << "Ball Position: " << (*it)->pos.x << ", " << (*it)->pos.y
                     << " | Speed: " << (*it)->speed.dx << ", " << (*it)->speed.dy
                     << "Ball Index: " << (*it)->idx << endl;
            }
            break;
        case 'c':
            // 리스트 초기화
            // 리스트 내 모든 객체 할당 해제
            // 리스트 초기화 시에는 클라이어트 전송 mutex 필요
            // mutex lock
            pthread_mutex_lock(&list_mutex);
            for (auto it = ballList.begin(); it != ballList.end(); ++it)
            {
                delete *it;
            }
            ballList.clear();
            pthread_mutex_unlock(&list_mutex);
            // mutex unlock

            // 리스트 현황 출력
            cout << "ballList size: " << ballList.size() << endl;
            break;
        default:
            break;
        }
    }
}

// 클라이언트 리스트와 서버 리스트 동기화
void sync_list(int client_socket)
{
    // 클라이언트 리스트와 서버 리스트 동기화
    int sock = client_socket;

    while (true)
    {
        pthread_mutex_lock(&list_mutex);
        pthread_cond_wait(&list_cond, &list_mutex);
        if (!ballList.empty())
        {
            // 리스트 전체를 한번에 send
            for (auto it = ballList.begin(); it != ballList.end(); ++it)
            {
                this_thread::sleep_for(std::chrono::nanoseconds(1000));
                // 클라이언트로 전송
                send(sock, *it, sizeof(Ball), 0);
            }
        }
        pthread_mutex_unlock(&list_mutex);
    }
}

void keep_accept(ServerSocket server, vector<int>& client_sockets, vector<thread>& threads){
    //thread 처리 필요!
    while (true)
    {
        //클라이언트 소켓 벡터, 스레드 벡터에 추가
        server.acceptConnection(client_sockets, threads);
    }
}