#ifndef COMMON_H
#define COMMON_H

#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

using namespace std;

typedef struct dev_fb_t
{
	int fbfd;
	struct fb_var_screeninfo vinfo;
	struct fb_fix_screeninfo finfo;
	long int screensize;
	char *fbp;
}dev_fb;

typedef struct pixel_t
{
	int x;
	int y;
}pixel;

typedef struct 
{
    int dx; //case: -1, 1
    int dy; //case: -1, 1
} Speed;

typedef struct
{
	int idx;
	int client_num;
	pixel pos;
	Speed speed;
}Ball;

typedef struct
{
	char cmd[100];
	int opt_num;
	int client_num;
}packet;

typedef struct{
	int pkt_type;//0: size, 1: data
	int list_size;
	Ball ball;
}sync_packet;

typedef struct
{
	Ball *ball;
	iterator<Ball *, list<Ball *>> it;
}ball_args;

#endif// COMMON_H