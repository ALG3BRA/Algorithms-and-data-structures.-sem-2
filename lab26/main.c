#include "stack.h"
#include "textmenu.h"

// C & D
void *creator_stack() {
    Stack *stack = stackCreate(10);
    return (void*)stack;
}

void destroyer_stack(void *obj) {
    Stack *stack = (Stack*)obj;
    stackDestroy(stack);
}

// Handlers
void stack_push(void *obj) {
    Stack *stack = (Stack*)obj;
    Data value;
    scanf("%lf", &value);
    stackPush(stack, value);
    printf("Success\n");
}

void stack_pop(void *obj) {
    Stack *stack = (Stack*)obj;
    Data *value = stackPop(stack);
    if (value)
        printf("Popped element: %.1f\n", *value);
    else
        printf("Stack is empty\n");
}

void stack_get(void *obj) {
    Stack *stack = (Stack*)obj;
    Data *value = stackGetData(stack);
    if (value)
        printf("Top element: %.1f\n", *value);
    else
        printf("Stack is empty\n");
}

void stack_print(void *obj) {
    Stack *stack = (Stack*)obj;
    stackPrint(stack);
}

void stack_sort(void *obj) {
    Stack *stack = (Stack*)obj;
    stackSort(stack);
}


int main() {
    textMenuFastInitialize(5,
                           "print", "print - prints stack.", stack_print,
                           "push", "push <value> - pushes value to stack.", stack_push,
                           "pop", "pop - deletes and returns top value from stack.", stack_pop,
                           "get", "get - gets top value from stack.", stack_get,
                           "sort", "sort - sorts stack.", stack_sort
                            );
    textMenuFastStart(creator_stack, destroyer_stack);
    textMenuFastClose();
}