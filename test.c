#include <arpa/inet.h>
#include<stdio.h>

#define PORT 8080

int main(){
    uint16_t little = PORT;
    uint16_t big = htons(PORT);

    printf("big: %x, little %x\n",big, little);
    return 0;
}