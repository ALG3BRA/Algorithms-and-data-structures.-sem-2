#ifndef TREE_STACK_H
#define TREE_STACK_H

#include <stdbool.h>
#include <stdlib.h>
#include "tree.h"

typedef struct {
    TreeNode **nodes;
    size_t size; 
    size_t capacity;
} TNStack;

TNStack *createTreeNodeStack();

bool treeNodeStackIsFull(TNStack *stack);

void treeNodeStackIncrease(TNStack *stack);

void treeNodeStackPush(TNStack *stack, TreeNode *node);

TreeNode *treeNodeStackPop(TNStack *stack);

TreeNode *treeNodeStackTop(TNStack *stack);

bool treeNodeStackIsEmpty(TNStack *stack);

void clearTreeNodeStack(TNStack *stack);

void freeTreeNodeStack(TNStack *stack);

void treeNodeStackPrintPostfix(TNStack *stack);


#endif /* TREE_STACK_H */
