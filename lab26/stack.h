#ifndef STACK_H
#define STACK_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Stack Interface ===

typedef double Data;

typedef struct Stack {
    Data *elements;
    size_t capacity, size;
} Stack;

Stack *stackCreate(size_t capacity);
void stackDestroy(Stack *stack);
bool stackIsEmpty(Stack *stack);
bool stackIsFull(Stack *stack);
void stackIncrease(Stack *stack);
void stackReduce(Stack *stack);
void stackPush(Stack *stack, Data value);
Data *stackPop(Stack *stack);
Data *stackGetData(Stack *stack);
size_t stackSize(Stack *stack);
void stackPrint(Stack *stack);

// \Stack Interface ===


// Sort ===

bool swapElementsInStack(Stack *stack);
void stackSort(Stack *stack);

// \Sort ===


#define STACK_INIT_CAPACITY 10

#endif // STACK_H
