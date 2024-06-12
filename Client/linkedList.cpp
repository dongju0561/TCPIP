#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "linkedList.hpp"
#include "fbDraw.hpp"

node *head = (node *)malloc(sizeof(node));

void append_node(node* head, Ball* new_data) 
{
    // 노드 메모리 할당
    node* new_node = (node*)malloc(sizeof(node));
    if (new_node == NULL) {
        printf("Memory allocation failed\n");
        return;
    }

    // 노드 내 데이터 메모리 할당
    new_node->data = (Ball*)malloc(sizeof(Ball));
    if (new_node->data == NULL) {
        printf("Memory allocation failed\n");
        free(new_node);
        return;
    }

    // 데이터 초기화
    new_node->data->pos = new_data->pos;
    new_node->data->speed = new_data->speed;
    new_node->next = NULL;

    // 리스트 끝에 노드 추가
    node* last = head;
    while (last->next != NULL)
        last = last->next;
    
    last->next = new_node;
}

void delete_last_node(node* head) {
    if (head == NULL || head->next == NULL) {
        return;
    }

    node* prev = head;
    node* curr = head->next;

    // 마지막 노드 탐색
    while (curr->next != NULL) {
        prev = curr;
        curr = curr->next;
    }

    prev->next = NULL;
    free(curr->data);
    free(curr);
}

void print_info(node* head)
{
    if (head == NULL || head->next == NULL) {
        printf("list is empty\n");
        return;
    }

    int cnt = 0;
    node* curr = head->next;

    while (curr != NULL) {
        cnt++;
        int dx = curr->data->speed.dx;
        int dy = curr->data->speed.dy;

        printf("노드번호: #%d\n", cnt);        
        printf("x: %d, y: %d\n", curr->data->pos.x, curr->data->pos.y);
        printf("speed: dx = %d dy = %d\n", dx, dy);

        curr = curr->next;
    }
}

void close_list(node* head)
{
     node* curr = head;
    while (curr != NULL) {
        node* next = curr->next;
        if (curr->data != NULL) {
            free(curr->data);
        }
        free(curr);
        curr = next;
    }
    exit(1);
}