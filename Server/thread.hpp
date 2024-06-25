#ifndef THREAD_HPP
#define THREAD_HPP

#include <iostream>
#include <pthread.h>
#include <cstdlib>
#include <cstring>
#include <vector>
#include "socket.hpp"

#define CMD_BUFFER_SIZE 5
#define BALL_NUM 2000

using namespace std;

typedef struct {
    int sockets;
} ThreadArgs;

extern int sock;
extern int data_available;
extern char buffer[CMD_BUFFER_SIZE];

extern pthread_cond_t buffer_cond;
extern pthread_mutex_t buffer_mutex;

// 공 속도 변수
extern pthread_t receive, move_calculator, sync_t, ball_move_threads[BALL_NUM];

extern int ball_move_thread_idx;

void keep_accept(ServerSocket server, vector<int>& client_sockets, vector<thread>& threads);
void recv_cmd(int client_socket);
void *move_ball(void *arg);
void sync_list(int client_socket);
void monitor_list();

#endif // THREAD_H
