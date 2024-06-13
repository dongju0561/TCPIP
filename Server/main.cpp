#include <iostream>
#include <cstdlib>
#include <cstring>
#include <list>
#include "common.hpp"
#include "socket.hpp"

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
    list<Ball *> ballList;
    Ball *newBall;
    char cmd[100];

    // 서버 소켓 생성
    ServerSocket server(PORT);
    server.bindSocket();
    server.listenSocket(3);
    int new_socket = server.acceptConnection();

    // 클라이언트로부터 명령 수신
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

            //리스트 추가 시에는 클라이어트 전송 mutex 필요
            // 공 객체를 리스트에 추가

            //mutex lock

            ballList.push_back(newBall);

            //mutex unlock

            cout << "ballList size: " << ballList.size() << endl;
            for (auto it = ballList.begin(); it != ballList.end(); ++it)
            {
                cout << "Ball Position: " << (*it)->pos.x << ", " << (*it)->pos.y
                          << " | Speed: " << (*it)->speed.dx << ", " << (*it)->speed.dy << endl;
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
                cout << "Ball Position: " << (*it)->pos.x << ", " << (*it)->pos.y
                          << " | Speed: " << (*it)->speed.dx << ", " << (*it)->speed.dy << endl;
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
    return 0;
}
