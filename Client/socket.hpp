#ifndef SOCKET_H
#define SOCKET_H

#include <netinet/in.h>

class ClientSocket {
public:
    int sock;
    struct sockaddr_in serv_addr;

    ClientSocket();
    ~ClientSocket();
    void connectToServer(const char *ip_address, int port);
};

extern ClientSocket client;

#endif // SOCKET_H
