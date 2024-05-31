#include "deque.h"


DequeNode *createDequeNode(TreeNode data) {
    DequeNode *newNode = (DequeNode *)malloc(sizeof(DequeNode));
    newNode->value = data;
    newNode->next = NULL;
    newNode->prev = NULL;
    return newNode;
}

Deque *createDeque() {
    Deque *deque = (Deque *)malloc(sizeof(Deque));
    deque->size = 0;
    deque->top = NULL;
    deque->bottom = NULL;
    return deque;
}

void dequePushFront(Deque *deque, TreeNode data) {
    DequeNode *newNode = createDequeNode(data);
    if (deque->size == 0) {
        deque->top = newNode;
        deque->bottom = newNode;
    } else {
        newNode->next = deque->top;
        deque->top->prev = newNode;
        deque->top = newNode;
    }
    deque->size++;
}

void dequePushBack(Deque *deque, TreeNode data) {
    DequeNode *newNode = createDequeNode(data);
    if (deque->size == 0) {
        deque->top = newNode;
        deque->bottom = newNode;
    } else {
        newNode->prev = deque->bottom;
        deque->bottom->next = newNode;
        deque->bottom = newNode;
    }
    deque->size++;
}

TreeNode dequePopFront(Deque *deque) {
    if (deque->size == 0) {
        printf("Deque is empty!\n");
        exit(EXIT_FAILURE);
    }
    DequeNode *temp = deque->top;
    TreeNode value = temp->value;
    deque->top = deque->top->next;
    if (deque->top != NULL)
        deque->top->prev = NULL;
    free(temp);
    deque->size--;
    return value;
}

TreeNode dequePopBack(Deque *deque) {
    if (deque->size == 0) {
        printf("Deque is empty!\n");
        exit(EXIT_FAILURE);
    }
    DequeNode *temp = deque->bottom;
    TreeNode value = temp->value;
    deque->bottom = deque->bottom->prev;
    if (deque->bottom != NULL)
        deque->bottom->next = NULL;
    free(temp);
    deque->size--;
    return value;
}

void dequeDestroyDeque(Deque *deque) {
    while (deque->size > 0)
        dequePopFront(deque);
    free(deque);
}

TreeNode dequeGetBack(Deque *deque) {
    if (deque == NULL || deque->size == 0) {
        printf("Deque is empty!\n");
        exit(EXIT_FAILURE);
    }
    return deque->bottom->value;
}




void dequePrint(const Deque *deque) {
    if (deque == NULL || deque->size == 0) {
        printf("Deque is empty!\n");
        return;
    }

    int count = 1;
    DequeNode *current = deque->top;
    while (current != NULL) {
        printf("%d. ", count);
        switch (current->value.token.type) {
            case INTEGER:
                printf("type: INTEGER, value: %d\n", current->value.token.value);
                break;
            case VARIABLE:
                printf("type: VARIABLE, name: %s\n", current->value.token.name);
                break;
            case OPERATION_PLUS:
                printf("type: OPERATION_PLUS\n");
                break;
            case OPERATION_MINUS:
                printf("type: OPERATION_MINUS\n");
                break;
            case OPERATION_MULT:
                printf("type: OPERATION_MULT\n");
                break;
            case OPERATION_DIVIDE:
                printf("type: OPERATION_DIVIDE\n");
                break;
            case BRACKETS_OPEN:
                printf("type: BRACKETS_OPEN\n");
                break;
            case BRACKETS_CLOSE:
                printf("type: BRACKETS_CLOSE\n");
                break;
            default:
                printf("type: NONE\n");
                break;
        }
        count++;
        current = current->next;
    }
}


void dequePrintPostfix(Deque *deque) {
    if (deque == NULL || deque->size == 0) {
        printf("Deque is empty\n");
        return;
    }
    DequeNode *current = deque->top;
    while (current != NULL) {
        switch (current->value.token.type) {
            case VARIABLE:
                printf("%s", current->value.token.name);
                break;
            case INTEGER:
                printf("%d", current->value.token.value);
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
                // Handle other types or do nothing
                break;
        }
        current = current->next;
    }
    printf("\n");
}


