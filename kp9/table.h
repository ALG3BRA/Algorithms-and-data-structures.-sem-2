#ifndef __TABLE__
#define __TABLE__


#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>


typedef struct Key {
    char string[28];
    int number;
} Key;


typedef struct Element {
    Key key;
    double value;
} Element;


typedef struct Table {
    Element *data;
    size_t size, capacity;
} Table;


// TableInterface ===

Table *tableCreate();
void tableDestroy(Table *table);
Element *createElement(char *string);
void tableResize(Table *table);
void tablePush(Table *table, char *string);
bool tableIsEmpty(Table *table);

// Print
void columnPrint(char *string, int n);
void linePrint(char *string, int number, double value);
void tablePrint(Table *table);
void printLineInfoById(Table *table, int id);

// Sort && Search
int keyscmp(Element elem1, Element elem2);
void swap(Table *table, int a, int b);
void heapify(Table *table, int n, int i);
void buildHeap(Table *table);
void tableHeapSort(Table *table);
int tableBinarySearch(Table *table, char *keys);

// \TableInterface ===

extern bool TABLE_IS_SORTED;

#endif