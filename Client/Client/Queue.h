#ifndef QUEUE_H
#define QUEUE_H

#define MAX_SIZE 100

typedef struct {
    int array[MAX_SIZE];
    int front, rear;
} Queue;

// 큐 초기화
void initQueue(Queue* queue);

// 큐가 비어있는지 확인
int isEmpty(Queue* queue);

// 큐가 가득 차있는지 확인
int isFull(Queue* queue);

// 큐에 원소 추가
void enqueue(Queue* queue, int item);

// 큐에서 원소 제거
int dequeue(Queue* queue);

// 큐의 front에 있는 원소 반환
int front(Queue* queue);

// 큐의 rear에 있는 원소 반환
int rear(Queue* queue);

// 큐의 모든 원소 출력
void printQueue(Queue* queue);

#endif
