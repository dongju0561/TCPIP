#include <iostream>
#include <cstdlib>
#include <cstring>
#include <list>
#include <unistd.h>
#include <arpa/inet.h>
#include "common.hpp"

#define PORT 8080
#define BUFFER_SIZE 1024

using namespace std;

Ball *createBall()
{
    // 랜덤으로 x, y, dx, dy 생성
    Ball *ball = new Ball;
    ball->pos.x = rand() % 1281;
    ball->pos.y = rand() % 801;
    ball->speed.dx = rand() % 2 == 0 ? 1 : -1;
    ball->speed.dy = rand() % 2 == 0 ? 1 : -1;

    return ball;
}

int main()
{
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    list<Ball *> ballList;
    Ball *newBall;
    // char buffer[BUFFER_SIZE] = {0};
    char cmd[100];

    const char *hello = "Hello from server";

    // 소켓 파일 디스크립터 생성
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // 소켓 옵션 설정
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // 소켓과 포트를 바인딩
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    // 연결을 대기 상태로 설정
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    // 수신 연결 수락
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    while (1)
    {
        memset(cmd, 0, BUFFER_SIZE); // 버퍼 초기화
        int valread = recv(new_socket, cmd, BUFFER_SIZE, 0);
        if (valread <= 0)
        {
            break; // 연결이 끊어지거나 에러 발생 시 루프 탈출
        }
        switch (cmd[0])
        {
        case 'a':
            // 공 객체 생성
            newBall = createBall();

            // 공 객체를 리스트에 추가
            ballList.push_back(newBall);

            cout << "ballList size: " << ballList.size() << endl;
            for (auto it = ballList.begin(); it != ballList.end(); ++it)
            {
                std::cout << "Ball Position: " << (*it)->pos.x << ", " << (*it)->pos.y
                          << " | Speed: " << (*it)->speed.dx << ", " << (*it)->speed.dy << std::endl;
            }
            break;
        case 'd':
            // 리스트에서 마지막 객체 삭제
            if (!ballList.empty())
            {
                delete ballList.back();
                ballList.pop_back();
            }
            cout << "ballList size: " << ballList.size() << endl;
            for (auto it = ballList.begin(); it != ballList.end(); ++it)
            {
                std::cout << "Ball Position: " << (*it)->pos.x << ", " << (*it)->pos.y
                          << " | Speed: " << (*it)->speed.dx << ", " << (*it)->speed.dy << std::endl;
            }
            break;
        case 'c':
            // 리스트 초기화
            // 리스트 내 모든 객체 할당 해제
            for (auto it = ballList.begin(); it != ballList.end(); ++it)
            {
                delete *it;
            }
            ballList.clear();
            cout << "ballList size: " << ballList.size() << endl;
            break;
        default:
            break;
        }
    }

    close(new_socket);
    close(server_fd);
    return 0;
}