#ifndef STACK_H
#define STACK_H

#include <stdbool.h>
#include <stdlib.h>
#include "tree.h"

typedef struct {
    Token *tokens;
    size_t size, capacity; 
} StationStack;

StationStack *createStationStack();
bool stationStackIsFull(StationStack *stack);
void stationStackIncrease(StationStack *stack);
void stationStackPush(StationStack *stack, Token token);
Token *stationStackPop(StationStack *stack);
Token *stationStackGetToken(StationStack *stack);
bool stationStackIsEmpty(StationStack *stack);

#endif /* STACK_H */
