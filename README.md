# TCP_IP

## Introduction

## Tech stack
- TCP/IP
- 프레임버퍼
- 스레드 : POSIX 스레드(pthread)를 사용하여 다중 스레드 구현

## Feature
서버에서 공에 대한 데이터 저장 및 움직임 연산
클라이언트는 전달 받은 공의 위치 데이터에 따라 프레임버퍼에 출력
스크린에 벽에 맞고 튕기는 공을 생성('a'), 삭제('z')
공의 속도 증가('s')
공의 속도 감소('d')
프로그램 종료('x')

## Installation

### Installing

1. 프로젝트 클론:
    ```sh
    git clone https://github.com/dongju0561/TCPIP.git
    ```
2. 프로젝트 디렉토리로 이동:
    ```sh
    cd TCPIP
    ```
3. client, server프로그램 빌드
    ```sh
    make all
    ```

# Example command to run the project
- ctrl + Shift + F2~F5 입력(가상 콘솔로 이동)
- 먼저 서버를 실행
```sh
make run-server
```
- 이후 클라이언트 실행
```sh
make run-client
```
