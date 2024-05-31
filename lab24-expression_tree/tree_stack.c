#include "tree_stack.h"


#define INITIAL_CAPACITY 10

TNStack *createTreeNodeStack() {
    TNStack *stack = malloc(sizeof(TNStack));
    if (stack == NULL) {
        exit(EXIT_FAILURE);
    }
    stack->nodes = malloc(INITIAL_CAPACITY * sizeof(TreeNode *));
    if (stack->nodes == NULL) {
        free(stack);
        exit(EXIT_FAILURE);
    }
    stack->size = 0;
    stack->capacity = INITIAL_CAPACITY;
    return stack;
}

bool treeNodeStackIsFull(TNStack *stack) {
    return stack->size >= stack->capacity;
}

void treeNodeStackIncrease(TNStack *stack) {
    size_t new_capacity = stack->capacity * 2;
    TreeNode **new_nodes = realloc(stack->nodes, new_capacity * sizeof(TreeNode *));
    if (new_nodes == NULL) {
        exit(EXIT_FAILURE);
    }
    stack->nodes = new_nodes;
    stack->capacity = new_capacity;
}

void treeNodeStackPush(TNStack *stack, TreeNode *node) {
    if (treeNodeStackIsFull(stack)) {
        treeNodeStackIncrease(stack);
    }
    stack->nodes[stack->size++] = node;
}

TreeNode *treeNodeStackPop(TNStack *stack) {
    if (treeNodeStackIsEmpty(stack)) {
        exit(EXIT_FAILURE);
    }
    return stack->nodes[--stack->size];
}

TreeNode *treeNodeStackTop(TNStack *stack) {
    if (treeNodeStackIsEmpty(stack)) {
        exit(EXIT_FAILURE);
    }
    return stack->nodes[stack->size - 1];
}

bool treeNodeStackIsEmpty(TNStack *stack) {
    return stack->size == 0;
}

void clearTreeNodeStack(TNStack *stack) {
    stack->size = 0;
}

void freeTreeNodeStack(TNStack *stack) {
    free(stack->nodes);
    free(stack);
}



void treeNodeStackPrintPostfix(TNStack *stack) {
    if (stack == NULL || treeNodeStackIsEmpty(stack)) {
        printf("Stack is empty\n");
        return;
    }

    for (size_t i = 0; i < stack->size; i++) {
        TreeNode *node = stack->nodes[i];
        switch (node->token.type) {
            case VARIABLE:
                printf("%s", node->token.name);
                break;
            case INTEGER:
                printf("%d", node->token.value);
                break;
            case OPERATION_PLUS:
                printf("+");
                break;
            case OPERATION_MINUS:
                printf("-");
                break;
            case OPERATION_MULT:
                printf("*");
                break;
            case OPERATION_DIVIDE:
                printf("/");
                break;
            default:
                break;
        }
    }
    printf("\n");
}

