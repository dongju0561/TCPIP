# TCP_IP

## Introduction

## Tech stack
- TCP/IP
- 프레임버퍼
- 스레드 : POSIX 스레드(pthread)를 사용하여 다중 스레드 구현

## Feature
- 클라이언트 접속 시 서버에서 연결 상태 확인
- 클라이언트는 서버에게 공 추가 혹은 삭제 요청
- 클라이언트 별 다른 색상의 공간 추가
- 서버에서 공에 대한 데이터 저장 및 움직임 연산
- 서버와 클라이언트는 공의 객체가 저장된 각각의 리스트를 동기화 진행
- 클라이언트는 동기화된 리스트에 따라 공의 위치 데이터에 기반하여 프레임버퍼에 출력

## Input
- 공 생성: 'a'
- 공 삭제: 'd'

## Development Environment

### Operating System
- **OS:** Ubuntu 22.04.1 LTS

### Kernel Version
- **Kernel:** 6.5.0-35-generic

### Network
- **Networking:** 요구되는 포트: 서버와 통신하기 위하여 8080번 포트

### Compiler
- **Compiler:** g++ (Ubuntu 11.4.0-1ubuntu1~22.04) 11.4.0

### Build System
- **Build System:** Make
- **Make Version:** 4.3

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
