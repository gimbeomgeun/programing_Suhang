#include "Queue.h"
#include <stdio.h>

// 큐 초기화
void initQueue(Queue* queue) {
    queue->front = -1;
    queue->rear = -1;
}

// 큐가 비어있는지 확인
int isEmpty(Queue* queue) {
    return (queue->front == -1 && queue->rear == -1);
}

// 큐가 가득 차있는지 확인
int isFull(Queue* queue) {
    return ((queue->rear + 1) % MAX_SIZE == queue->front);
}

// 큐에 원소 추가
void enqueue(Queue* queue, int item) {
    if (isFull(queue)) {
        printf("Queue is full. Cannot enqueue.\n");
        return;
    }

    if (isEmpty(queue)) {
        queue->front = 0;
        queue->rear = 0;
    }
    else {
        queue->rear = (queue->rear + 1) % MAX_SIZE;
    }

    queue->array[queue->rear] = item;
}

// 큐에서 원소 제거
int dequeue(Queue* queue) {
    if (isEmpty(queue)) {
        printf("Queue is empty. Cannot dequeue.\n");
        return -1; // 예외 처리: 큐가 비어있을 때는 -1을 반환하거나 다른 방식으로 처리할 수 있습니다.
    }

    int item = queue->array[queue->front];

    if (queue->front == queue->rear) {
        // 큐에 마지막 원소가 남아있는 경우
        initQueue(queue);
    }
    else {
        queue->front = (queue->front + 1) % MAX_SIZE;
    }

    return item;
}

// 큐의 front에 있는 원소 반환
int front(Queue* queue) {
    if (isEmpty(queue)) {
        printf("Queue is empty.\n");
        return -1; // 예외 처리: 큐가 비어있을 때는 -1을 반환하거나 다른 방식으로 처리할 수 있습니다.
    }

    return queue->array[queue->front];
}

// 큐의 rear에 있는 원소 반환
int rear(Queue* queue) {
    if (isEmpty(queue)) {
        printf("Queue is empty.\n");
        return -1; // 예외 처리: 큐가 비어있을 때는 -1을 반환하거나 다른 방식으로 처리할 수 있습니다.
    }

    return queue->array[queue->rear];
}

// 큐의 모든 원소 출력
void printQueue(Queue* queue) {
    if (isEmpty(queue)) {
        printf("Queue is empty.\n");
        return;
    }

    printf("Queue elements: ");
    int i = queue->front;
    while (i != queue->rear) {
        printf("%d, ", queue->array[i]);
        i = (i + 1) % MAX_SIZE;
    }
    printf("%d\n", queue->array[queue->rear]);
}
