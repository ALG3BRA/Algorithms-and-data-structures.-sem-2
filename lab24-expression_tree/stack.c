#include "stack.h"


StationStack *createStationStack() {
    StationStack *stack = malloc(sizeof(StationStack));
    stack->capacity = 1;
    stack->size = 0;
    stack->tokens = malloc(sizeof(Token));
    return stack;
}

bool stationStackIsEmpty(StationStack *stack) {
    return stack->size == 0;
}

bool stationStackIsFull(StationStack *stack) {
    return stack->capacity == stack->size;
}

void stationStackIncrease(StationStack *stack) {
    stack->capacity *= 2;
    stack->tokens = realloc(stack->tokens, stack->capacity * sizeof(Token));
}

void stationStackPush(StationStack *stack, Token token) {
    if (stationStackIsFull(stack)) {
        stationStackIncrease(stack);
    }
    stack->tokens[stack->size] = token;
    stack->size++;
}

Token *stationStackPop(StationStack *stack) {
    if (stationStackIsEmpty(stack)) {
        return NULL;
    }
    stack->size--;
    return &stack->tokens[stack->size];
}

Token *stationStackGetToken(StationStack *stack) {
    if (stationStackIsEmpty(stack)) {
        return NULL;
    }
    return &stack->tokens[stack->size - 1];
}
