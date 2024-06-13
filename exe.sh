#!/bin/bash
# 이 쉘 스크립트는 Client와 Server를 동시에 빌드하는 스크립트입니다.

# 스크립트 이름을 저장
SCRIPT_NAME=$(basename "$0")

# 인자가 전달되지 않았을 경우 사용법 출력
if [ $# -ne 1 ]; then
    echo "Usage: $SCRIPT_NAME <m|c|s>"
    exit 1
fi

# 첫 번째 인자를 변수에 저장
command=$1

# 명령어에 따라 분기
case $command in
    m)
        echo "Running make to build the project..."

        # Client 빌드
        cd Client
        make

        # Server 빌드
        cd ../Server
        make
        ;;
    c)
        cd Client
        if [ -f "./client" ]; then
            echo "Executing the client program..."
            ./client
        else
            echo "client executable not found! Please build the project first."
            exit 1
        fi
        ;;
    s)
        cd Server
        if [ -f "./server" ]; then
            echo "Executing the server program..."
            ./server
        else
            echo "server executable not found! Please build the project first."
            exit 1
        fi
        ;;
    
esac
