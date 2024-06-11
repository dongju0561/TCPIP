#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUF_SIZE 1024
#define BALL_COUNT 5

typedef struct {
    int x;
    int y;
} BallPosition;

void error_handling(char *message);

int main() {
    int sock;
    struct sockaddr_in serv_addr;
    BallPosition balls[BALL_COUNT];
    int i, str_len;

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock == -1)
        error_handling("socket() error");

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(PORT);

    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1)
        error_handling("connect() error");

    while (1) {
        // 서버로부터 공 위치 데이터를 읽음
        str_len = read(sock, balls, sizeof(balls));
        if (str_len == -1)
            error_handling("read() error");

        // 공 위치 데이터를 출력
        for (i = 0; i < BALL_COUNT; i++) {
            printf("Ball %d: x=%d, y=%d\n", i, balls[i].x, balls[i].y);
        }
        printf("\n");
        // sleep(1);
    }

    close(sock);
    return 0;
}

void error_handling(char *message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
