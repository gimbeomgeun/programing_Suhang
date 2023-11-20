#ifndef QUEUE_H
#define QUEUE_H

#define MAX_SIZE 100

typedef struct {
    int array[MAX_SIZE];
    int front, rear;
} Queue;

// ť �ʱ�ȭ
void initQueue(Queue* queue);

// ť�� ����ִ��� Ȯ��
int isEmpty(Queue* queue);

// ť�� ���� ���ִ��� Ȯ��
int isFull(Queue* queue);

// ť�� ���� �߰�
void enqueue(Queue* queue, int item);

// ť���� ���� ����
int dequeue(Queue* queue);

// ť�� front�� �ִ� ���� ��ȯ
int front(Queue* queue);

// ť�� rear�� �ִ� ���� ��ȯ
int rear(Queue* queue);

// ť�� ��� ���� ���
void printQueue(Queue* queue);

#endif
