#pragma once

#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

// ================
// Tree print
// ================

#define MAX_STRING_LENGTH 200

typedef struct _TextChunkNode {
    char *str;
    struct _TextChunkNode *next;
} TextChunkNode;

// Представление для прямоугольного куска текста. Хранит в себе односвязный список строк.
typedef struct {
    int rootIndex;
    int rowCount;
    int columnCount;
    TextChunkNode *first, *last;
} TextChunk;

// === String functions ===

char* intToString(int val, int base);

char *stringGenerate(char c, int n);

void stringConcatenate(char *str1, char *str2);

char *stringGenerateCell(char *str);

// ========================\ String functions

// === TextChunk interface ===

TextChunkNode *TextChunkNodeCreate();

TextChunk *textChunkCreate();

// Result in chunk1
void textChunkAdd(TextChunk *chunk1, TextChunk *chunk2);

// Result in chunk1
void textChunkConcatenate(TextChunk *chunk1, TextChunk *chunk2);

TextChunk *textChunkGenerateBlank(int n, int m);

TextChunkNode *textChunkGet(TextChunk *chunk, int index);

void textChunkPush(TextChunk *chunk, char *str);

void textChunkComplement(TextChunk *chunk, int n);

void textChunkEven(TextChunk *chunk1, TextChunk *chunk2);

void textChunkPrint(TextChunk *chunk);

void textChunkNodeDestroy(TextChunkNode *node);

void textChunkDestroy(TextChunk *chunk);

// ===========================\ TextChunk interface

#define SUBTREE_SPACING 1 // Must be odd
#define CELL_LENGTH 7 // Must be odd

TextChunk *treeGenerateTextChunkForSubtree(void *node,
                                           char *(*valueAccessor)(void*),
                                           void *(*leftChildAccessor)(void*),
                                           void *(*rightChildAccessor)(void*));

// valueAccessor must return unsafe string.
void treePrintUniversal(void *node,
                        char *(*valueAccessor)(void*),
                        void *(*leftChildAccessor)(void*),
                        void *(*rightChildAccessor)(void*));