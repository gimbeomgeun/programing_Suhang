#include "Queue.h"
#include <stdio.h>

// ť �ʱ�ȭ
void initQueue(Queue* queue) {
    queue->front = -1;
    queue->rear = -1;
}

// ť�� ����ִ��� Ȯ��
int isEmpty(Queue* queue) {
    return (queue->front == -1 && queue->rear == -1);
}

// ť�� ���� ���ִ��� Ȯ��
int isFull(Queue* queue) {
    return ((queue->rear + 1) % MAX_SIZE == queue->front);
}

// ť�� ���� �߰�
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

// ť���� ���� ����
int dequeue(Queue* queue) {
    if (isEmpty(queue)) {
        printf("Queue is empty. Cannot dequeue.\n");
        return -1; // ���� ó��: ť�� ������� ���� -1�� ��ȯ�ϰų� �ٸ� ������� ó���� �� �ֽ��ϴ�.
    }

    int item = queue->array[queue->front];

    if (queue->front == queue->rear) {
        // ť�� ������ ���Ұ� �����ִ� ���
        initQueue(queue);
    }
    else {
        queue->front = (queue->front + 1) % MAX_SIZE;
    }

    return item;
}

// ť�� front�� �ִ� ���� ��ȯ
int front(Queue* queue) {
    if (isEmpty(queue)) {
        printf("Queue is empty.\n");
        return -1; // ���� ó��: ť�� ������� ���� -1�� ��ȯ�ϰų� �ٸ� ������� ó���� �� �ֽ��ϴ�.
    }

    return queue->array[queue->front];
}

// ť�� rear�� �ִ� ���� ��ȯ
int rear(Queue* queue) {
    if (isEmpty(queue)) {
        printf("Queue is empty.\n");
        return -1; // ���� ó��: ť�� ������� ���� -1�� ��ȯ�ϰų� �ٸ� ������� ó���� �� �ֽ��ϴ�.
    }

    return queue->array[queue->rear];
}

// ť�� ��� ���� ���
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
