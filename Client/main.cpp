#include <cstdlib>
#include <cstring>
#include <unistd.h> 
#include <arpa/inet.h>
#include "common.hpp"
#include "fbDraw.hpp"
#include "thread.hpp"

#define PORT 8080
// #define BUFFER_SIZE 1024

int main() {
    
    struct sockaddr_in serv_addr;
    const char *hello = "Hello from client";
    char buffer[BUFFER_SIZE] = {0};

    fb_init(&fb);

    fb_fillScr(&fb, 0, 0, 0);

    // 소켓 생성(TCP 소캣을 생성)
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);

    // 서버 주소(네트워크 바이트 순서) 변환
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    // 서버에 연결 요청
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }
    pthread_create(&input, NULL, input_CMD, NULL);
    pthread_create(&processor, NULL, process_CMD, NULL);

    pthread_join(input, NULL);
    pthread_join(processor, NULL);
    close(sock);
    return 0;
}

// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <unistd.h>
// #include <arpa/inet.h>
// #include "fbDraw.hpp"

// #define PORT 8080
// #define BUFFER_SIZE 1024

// // typedef struct
// // {
// //     int pos;
// //     int speed;
// // } Ball;

// int main() {
//     int sock = 0;
//     struct sockaddr_in serv_addr;
//     const char *msg = "Hello from client";
//     char buffer[BUFFER_SIZE] = {0};

//     // fb_init(&fb);

//     // fb_fillScr(&fb, 0, 0, 0);


//     // 소켓 생성
//     if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
//         perror("Socket creation error");
//         return -1;
//     }

//     serv_addr.sin_family = AF_INET;
//     serv_addr.sin_port = htons(PORT);

//     // 서버 주소 변환
//     if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
//         perror("Invalid address/ Address not supported");
//         return -1;
//     }

//     // 서버에 연결 요청
//     if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
//         perror("Connection Failed");
//         return -1;
//     }

    

//     // Ball 구조체 전송
//     // if (send(sock, msg, strlen(msg), 0) < 0)
//     // {
//     //     perror("Send failed");
//     //     close(sock);
//     //     return -1;
//     // }

//     // 서버로부터 응답 수신
//     // int valread = read(sock, buffer, BUFFER_SIZE);
//     // if (valread < 0) {
//     //     perror("Read failed");
//     //     close(sock);
//     //     return -1;
//     // }

//     close(sock);
//     fb_close(&fb);
//     return 0;
// }