#include <iostream>
#include <arpa/inet.h>
#include <list>
#include <chrono>
#include <thread>
#include <vector>
#include <cstring> // for memset
#include <ctime>   // for time_t, time, ctime
#include <unistd.h>
#include <fcntl.h>
#include "common.hpp"
#include "thread.hpp"
#include "socket.hpp"

using namespace std;

extern list<Ball *> ballList;
int data_available = false;
char buffer[CMD_BUFFER_SIZE];
pthread_cond_t list_cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t list_mutex = PTHREAD_MUTEX_INITIALIZER;

struct timespec start_t, end_t;
 
pthread_t receive, move_calculator, sync_t, ball_move_threads[BALL_NUM];
ThreadArgs *args[BALL_NUM];
int ball_index = 0;

// 공 객체 생성 함수
Ball *createBall(int client_num)
{
    Ball *ball = new Ball;
    ball->idx = ball_index++;
    ball->client_num = client_num;
    ball->pos.x = rand() % 1281;
    ball->pos.y = rand() % 801;
    ball->speed.dx = rand() % 2 == 0 ? 1 : -1;
    ball->speed.dy = rand() % 2 == 0 ? 1 : -1;
    return ball;
}

void log_ball_action(int fd, const char *action, int client_num)
{
    time_t now;
    time(&now);
    char *timestamp = ctime(&now);
    timestamp[strlen(timestamp) - 1] = '\0'; // Remove the newline character

    int sizeof_ball = sizeof(Ball);

    char log_buffer[100];
    if (action == "added")
    {
        sprintf(log_buffer, "[%s] A ball is %s {ball_type: Client%d, memory_allocate: %dbytes}\n", timestamp, action, client_num, sizeof_ball);
    }
    else if (action == "deleted")
    {
        sprintf(log_buffer, "[%s] A ball is %s {ball_type: Client%d, memory_deallocate: %dbytes}\n", timestamp, action, client_num, sizeof_ball);
    }
    else if (action == "exit")
    {
        sprintf(log_buffer, "[%s] The program has terminated.\n", timestamp);
    }

    write(fd, log_buffer, strlen(log_buffer));
}

void handle_add_balls(int fd, int client_num, int opt_num)
{
    Ball *newBall;
    ball_args *ball_args;
    if (opt_num == -1)
    {
        pthread_mutex_lock(&list_mutex);
        // 하나만 공 추가
        newBall = createBall(client_num);
        log_ball_action(fd, "added", newBall->client_num);
        // 공 객체를 생성하면서 할당된 메모리 정도를 로그에 남김
        ballList.push_back(newBall);

        pthread_create(&ball_move_threads[newBall->idx], NULL, move_ball, newBall);

        // ballList 요소 전체 출력
        pthread_mutex_unlock(&list_mutex);
    }
    else
    {
        pthread_mutex_lock(&list_mutex);
        for (int i = 0; i < opt_num; ++i)
        {
            newBall = createBall(client_num);
            log_ball_action(fd, "added", newBall->client_num);
            // 공 객체를 생성하면서 할당된 메모리 정도를 로그에 남김

            ballList.push_back(newBall);
            
            pthread_create(&ball_move_threads[newBall->idx], NULL, move_ball, newBall);
        }
        pthread_mutex_unlock(&list_mutex);
    }
}

void handle_delete_balls(int fd, int opt_num)
{

    pthread_mutex_lock(&list_mutex);
    if (ballList.empty())
    {
        pthread_mutex_unlock(&list_mutex);
        return;
    }
    if (opt_num == -1)
    {
        // last thread cancel
        cout << "Ball index: " << ball_index << endl;
        pthread_cancel(ball_move_threads[--ball_index]);
        log_ball_action(fd, "deleted", ballList.back()->client_num);
        delete ballList.back();
        ballList.pop_back();
    }
    else
    {
        for (int i = 0; i < opt_num && !ballList.empty(); ++i)
        {
            // last thread cancel
            cout << "Ball index: " << ball_index << endl;
            pthread_cancel(ball_move_threads[--ball_index]);
            log_ball_action(fd, "deleted", ballList.back()->client_num);
            delete ballList.back();
            ballList.pop_back();
        }
        if (ballList.empty())
        {
            cout << "Ball list is empty" << endl;
        }
    }

    pthread_mutex_unlock(&list_mutex);
}

void handle_delete_all_balls(int fd, int opt_num)
{

    pthread_mutex_lock(&list_mutex);
    if (ballList.empty())
    {
        pthread_mutex_unlock(&list_mutex);
        return;
    }
    for (int i = 0; i < opt_num && !ballList.empty(); ++i)
    {
        // last thread cancel
        cout << "Ball index: " << ball_index << endl;
        pthread_cancel(ball_move_threads[--ball_index]);
        log_ball_action(fd, "deleted", ballList.back()->client_num);
        delete ballList.back();
        ballList.pop_back();
    }
    if (ballList.empty())
    {
        cout << "Ball list is empty" << endl;
    }
    log_ball_action(fd, "exit", 0);
    pthread_mutex_unlock(&list_mutex);
}

void *move_ball(void *arg)
{
    // get ballList's address from arg

    Ball *ball = (Ball *)arg;
    while (true)
    {

        ball->pos.x += ball->speed.dx;
        ball->pos.y += ball->speed.dy;

        if (ball->pos.x <= 0 || ball->pos.x >= 1280)
        {
            ball->speed.dx *= -1;
        }
        if (ball->pos.y <= 0 || ball->pos.y >= 800)
        {
            ball->speed.dy *= -1;
        }

        // pthread_mutex_lock(&list_mutex);

        // access to ballList with ball->idx by using advance
        auto it = ballList.begin();
        advance(it, ball->idx);
        *it = ball;

        // pthread_mutex_unlock(&list_mutex);
        usleep(9000); //
    }
    return NULL;
}

void recv_cmd(int client_socket)
{
    int new_fd = open("log.txt", O_WRONLY | O_APPEND | O_CREAT, 0644);
    if (new_fd == -1)
    {
        perror("open");
        exit(EXIT_FAILURE);
    }

    packet pkt;
    while (true)
    {
        memset(&pkt, 0, sizeof(packet));
        int valread = recv(client_socket, &pkt, sizeof(packet), 0);
        if (valread <= 0)
        {
            break;
        }
        switch (pkt.cmd[0])
        {
        case 'a':
            handle_add_balls(new_fd, pkt.client_num, pkt.opt_num);
            break;
        case 'd':
            handle_delete_balls(new_fd, pkt.opt_num);
            break;
        case 'x':
            handle_delete_all_balls(new_fd, ballList.size());
            sleep(2);
            exit(0);
            break;
        default:
            break;
        }
    }
    close(new_fd);
}

void sync_list(int client_socket)
{
    sync_packet pkt;

    while (true)
    {
        pthread_mutex_lock(&list_mutex);

        // pthread_cond_wait(&list_cond, &list_mutex);
        if (!ballList.empty())
        {
            pkt.pkt_type = 0;
            pkt.list_size = ballList.size();
            send(client_socket, &pkt, sizeof(sync_packet), 0);

            for (const auto &ball : ballList)
            {
                // usleep(3500);
                pkt.pkt_type = 1;
                pkt.ball = *ball;
                send(client_socket, &pkt, sizeof(sync_packet), 0);
            }
        }
        else
        {
            pkt.pkt_type = 0;
            pkt.list_size = 0;
            send(client_socket, &pkt, sizeof(sync_packet), 0);
        }

        // calculate time
        pthread_mutex_unlock(&list_mutex);
    }
}

void keep_accept(ServerSocket server, vector<int> &client_sockets, vector<thread> &threads)
{
    while (true)
    {
        server.acceptConnection(client_sockets, threads);
    }
}