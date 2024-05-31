#ifndef DEQUE_H
#define DEQUE_H

#include "tree.h" // Включаем заголовочный файл с определением типа данных TreeNode

typedef struct DequeNode {
    TreeNode value;
    struct DequeNode *next;
    struct DequeNode *prev;
} DequeNode;

typedef struct Deque {
    int size;
    DequeNode *top;
    DequeNode *bottom;
} Deque;

DequeNode *createDequeNode(TreeNode data);

Deque *createDeque();

void dequePushFront(Deque *deque, TreeNode data);

void dequePushBack(Deque *deque, TreeNode data);

TreeNode dequePopFront(Deque *deque);

TreeNode dequePopBack(Deque *deque);

void dequeDestroyDeque(Deque *deque);


///////////
TreeNode dequeGetBack(Deque *deque);
void dequePrint(const Deque *deque);
void dequePrintPostfix(Deque *deque);

#endif