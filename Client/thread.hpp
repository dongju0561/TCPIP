#ifndef THREAD_HPP
#define THREAD_HPP

#include <iostream>
#include <pthread.h>
#include <cstdlib>
#include <cstring>
#include "linkedList.hpp"

#define BUFFER_SIZE 5
#define BALL_NUM 200

typedef struct {
    dev_fb *fb;
    Ball *ball;
} ThreadArgs;

extern int data_available;
extern char buffer[BUFFER_SIZE];

extern pthread_cond_t buffer_cond;
extern pthread_mutex_t buffer_mutex;

// 공 속도 변수
extern pthread_t input, processor;
extern int speed;


void *input_CMD(void *arg);
void *process_CMD(void *arg);

#endif // THREAD_H
