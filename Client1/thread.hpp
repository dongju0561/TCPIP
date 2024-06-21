#ifndef THREAD_HPP
#define THREAD_HPP

#include <iostream>
#include <pthread.h>
#include <cstdlib>
#include <cstring>
#include "common.hpp"

#define BUFFER_SIZE 5
#define BALL_NUM 100

typedef struct {
    dev_fb *fb;
    Ball *ball;
} ThreadArgs;

extern int data_available;
extern char buffer[BUFFER_SIZE];

extern pthread_cond_t list_cond;
extern pthread_mutex_t list_mutex;
extern pthread_cond_t buffer_cond;
extern pthread_mutex_t buffer_mutex;

// 공 속도 변수
extern pthread_t input, processor,sync_t, print_ball, monitor, erase_all_ball_v;

void *input_CMD(void *arg);
void *process_CMD(void *arg);
void *sync_list(void *arg);
void *fb_print_ball(void *arg);
void *monitor_list(void *arg);
void *erase_all_ball(void *arg);

#endif // THREAD_H
