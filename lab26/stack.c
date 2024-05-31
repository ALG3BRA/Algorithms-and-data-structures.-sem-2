#include "stack.h"


// Stack Interface ===

Stack *stackCreate(size_t capacity) {
    Stack *stack = malloc(sizeof(Stack));
    stack->capacity = capacity;
    stack->size = 0;
    stack->elements = malloc(stack->capacity * sizeof(Data));
    return stack;
}

void stackDestroy(Stack *stack) {
    free(stack->elements);
    free(stack);
}

bool stackIsEmpty(Stack *stack) {
    return stack->size == 0;
}

bool stackIsFull(Stack *stack) {
    return stack->capacity == stack->size;
}

void stackIncrease(Stack *stack) {
    stack->capacity *= 2;
    stack->elements = realloc(stack->elements, stack->capacity * sizeof(Data));
}

void stackReduce(Stack *stack) {
    stack->capacity = stack->capacity / 2 + 1;
    stack->elements = realloc(stack->elements, stack->capacity * sizeof(Data));
}

void stackPush(Stack *stack, Data value) {
    if (stackIsFull(stack)) {
        stackIncrease(stack);
    }
    stack->elements[stack->size++] = value;
}

Data *stackPop(Stack *stack) {
    if (stackIsEmpty(stack)) {
        return NULL;
    }
    return &stack->elements[--stack->size];
}

Data *stackGetData(Stack *stack) {
    if (stackIsEmpty(stack)) {
        return NULL;
    }
    return &stack->elements[stack->size - 1];
}

size_t stackSize(Stack *stack) {
    return stack->size;
}

void stackPrint(Stack *stack) {
    if (stackIsEmpty(stack)) {
        printf("Stack is empty.\n");
        return;
    }
    printf("Stack content: ");
    for (size_t i = 0; i < stack->size; i++) {
        printf("%.1f  ", stack->elements[i]);
    }
    printf("\n");
}

// \Stack Interface ===


// Sort ===
void removeFromStackToStack(Stack *from, Stack *to) {
    while (!stackIsEmpty(from)) {
        stackPush(to, *stackPop(from));
    }
}

bool swapElementsInStack(Stack *stack) {
    if (stackSize(stack) < 2) return false;

    Stack *temp_stack = stackCreate(stackSize(stack));
    Data current_value;

    while (!stackIsEmpty(stack)) {   
        current_value = *stackPop(stack);
        stackPush(temp_stack, current_value);
        if (stackGetData(stack) && *stackGetData(stack) > current_value) {
            stackPop(temp_stack);
            break;
        }    
    }

    if (stackIsEmpty(stack)) {
        removeFromStackToStack(temp_stack, stack);
        stackDestroy(temp_stack);
        return false;
    }

    Data next_value;
    while (!stackIsEmpty(stack)) {
        next_value = *stackGetData(stack);
        if (next_value > current_value) {
            stackPop(stack);
            stackPush(temp_stack, next_value);
        } else {
            stackPush(temp_stack, current_value);
            break;
        }
    }

    if (stackIsEmpty(stack)) 
        stackPush(stack, current_value);
    removeFromStackToStack(temp_stack, stack);
    stackDestroy(temp_stack);
    
    return true;
}

void stackSort(Stack *stack) {
    if (stackIsEmpty(stack)) {
        printf("Stack is empty\n");
        return;
    }
    while (swapElementsInStack(stack));
}

// \Sort ===


bool swapElementsInStack2(Stack *stack) {
    if (stackSize(stack) < 2) return false;

    Stack *temp_stack = stackCreate(stackSize(stack));
    Data current_value;

    stackPush(temp_stack, *stackPop(stack));
    bool was_swap = false;

    while (!stackIsEmpty(stack)) {
        current_value = *stackGetData(temp_stack);
        if (stackGetData(stack) && current_value >= *stackGetData(stack)) {
            stackPush(temp_stack, *stackPop(stack));
        } else {
            stackPop(temp_stack);
            stackPush(temp_stack, *stackPop(stack));
            stackPush(temp_stack, current_value);
            was_swap = true;
        }
    }

    removeFromStackToStack(temp_stack, stack);
    stackDestroy(temp_stack);
    
    return was_swap;
}


void stackSort2(Stack *stack) {
    if (stackIsEmpty(stack)) {
        printf("Stack is empty\n");
        return;
    }
    while (swapElementsInStack2(stack));
}


int main() {
    Stack *stack = stackCreate(STACK_INIT_CAPACITY);
    
    // stackPush(stack, 25);
    // stackPush(stack, 15);
    // stackPush(stack, 70);
    // stackPush(stack, 60);
    // stackPush(stack, 1);   
    // stackPush(stack, 80);
    // stackPush(stack, 10);
    // stackPush(stack, 5);
    // stackPush(stack, 80);
    // stackPush(stack, 65);
    // stackPush(stack, 100);
    // stackPush(stack, 90);
    // stackPush(stack, 20);
    // stackPush(stack, 45);
    // stackPush(stack, 47);
    // stackPush(stack, 18);
    // stackPush(stack, 75);

    // stackPush(stack, 25);
    // stackPush(stack, 20);

    stackPrint(stack);
    stackSort2(stack); 
    stackPrint(stack);

    stackDestroy(stack);

    return 0;
}

