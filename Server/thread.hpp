#ifndef THREAD_HPP
#define THREAD_HPP

#include <iostream>
#include <pthread.h>
#include <cstdlib>
#include <cstring>


#define CMD_BUFFER_SIZE 5
#define BALL_NUM 200

typedef struct {
    int socket;
} ThreadArgs;

extern int sock;
extern int data_available;
extern char buffer[CMD_BUFFER_SIZE];

extern pthread_cond_t buffer_cond;
extern pthread_mutex_t buffer_mutex;

// 공 속도 변수
extern pthread_t receive, move_calculator, sync_t;

void *recv_cmd(void *arg);
void *move_ball(void *arg);
void *sync_list(void *arg);

#endif // THREAD_H
