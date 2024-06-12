#include <iostream>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

typedef struct
{
	int pos;
	int speed;
}Ball;

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    // char buffer[BUFFER_SIZE] = {0};
    Ball ball;

    const char *hello = "Hello from server";

    // 소켓 파일 디스크립터 생성
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // 소켓 옵션 설정
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // 소켓과 포트를 바인딩
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    // 연결을 대기 상태로 설정
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    // 수신 연결 수락
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    read(new_socket, &ball, sizeof(Ball));
    printf("Message from client: pos: %d speed: %d\n", ball.pos, ball.speed);
    send(new_socket, hello, strlen(hello), 0);
    printf("Hello message sent\n");

    close(new_socket);
    close(server_fd);
    return 0;
}

// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <unistd.h>
// #include <arpa/inet.h>

// #define PORT 8080
// #define BUF_SIZE 1024
// #define BALL_COUNT 5
// #define WIDTH 1280
// #define HEIGHT 800
// #define RADIUS 4

// typedef struct pixel_t
// {
// 	int x;
// 	int y;
// }pixel;

// typedef struct 
// {
//     int dx; //case: -1, 1
//     int dy; //case: -1, 1
// } Speed;

// typedef struct
// {
// 	pixel pos;
// 	Speed speed;
// }Ball;
// void moveBall(Ball *ball);
// void error_handling(const char *message);

// int main() {
//     int serv_sock, clnt_sock;
//     struct sockaddr_in serv_addr, clnt_addr;
//     socklen_t clnt_addr_size;
//     char message[11];

//     serv_sock = socket(PF_INET, SOCK_STREAM, 0);
//     if (serv_sock == -1)
//         error_handling("socket() error");

//     memset(&serv_addr, 0, sizeof(serv_addr));
//     serv_addr.sin_family = AF_INET;
//     serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
//     serv_addr.sin_port = htons(PORT);

//     if (bind(serv_sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) == -1)
//         error_handling("bind() error");

//     if (listen(serv_sock, 5) == -1)
//         error_handling("listen() error");

//     clnt_addr_size = sizeof(clnt_addr);
//     clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &clnt_addr_size);
//     if (clnt_sock == -1)
//         error_handling("accept() error");
//     read(clnt_sock, message, strlen(message));
//     printf("Message from client: %s\n", message);
    
//     // while (1) {
//     //     // 공의 위치 데이터를 클라이언트로 전송
//     //     // write(clnt_sock, &ball, sizeof(Ball));
//     //     // moveBall(&ball);
//     //     read(clnt_sock, message, strlen(message));
//     //     if (strcmp(message, "공 추가") == 0)
//     //     {
//     //         printf("Message from client: %s\n", message);
//     //         sprintf(message, "");
//     //         break;
//     //     }
//     // }

//     close(clnt_sock);
//     close(serv_sock);
//     return 0;
// }
// void moveBall(Ball *ball){
//     ball->pos.x += ball->speed.dx;
//         ball->pos.y += ball->speed.dy;
//         //ball이 벽에 부딪혔을때
//         if (ball->pos.x <= 0 || ball->pos.x >= WIDTH - RADIUS) ball->speed.dx = -ball->speed.dx;
//         if (ball->pos.y <= 0 || ball->pos.y >= HEIGHT - RADIUS) ball->speed.dy = -ball->speed.dy;
// }

// void error_handling(const char *message) {
//     fputs(message, stderr);
//     fputc('\n', stderr);
//     exit(1);
// }
