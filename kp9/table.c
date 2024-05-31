#include "table.h"

bool TABLE_IS_SORTED = false;

// TableInterface ===

Table *tableCreate() {
    Table *table = malloc(sizeof(Table));
    table->capacity = 1;
    table->size = 0;
    table->data = malloc(sizeof(Element));
    return table;
}

void tableDestroy(Table *table) {
    assert(table != NULL);
    free(table->data);
    free(table);
}

Element *createElement(char *string) {
    Element *elem = malloc(sizeof(Element));

    int result = sscanf(string, "%27[^;];%d;%lf", elem->key.string, &elem->key.number, &elem->value);
    if (result != 3) {
        printf("Error: incorrect input format\n");
        free(elem);
        return NULL;
    }

    return elem;
}

void tableResize(Table *table) {
    table->capacity *= 2;
    table->data = realloc(table->data, sizeof(Element) * table->capacity);
}

void tablePush(Table *table, char *string) {
    if (table->capacity <= table->size) {
        tableResize(table);
    }
    Element *element = createElement(string);
    if (element != NULL) {
        table->data[table->size] = *element;
        table->size++;
        free(element);
    }
    
    TABLE_IS_SORTED = false;
}

bool tableIsEmpty(Table *table) {
    return table->size == 0; 
}

// PRINT

void columnPrint(char *string, int n) {
    printf("%s", string);
    int len = strlen(string);
    for (int i = 0; i < n - len - 1; i++) {
        putchar(' ');
    }
    printf("|");
}

void linePrint(char *string, int number, double value) {
    putchar('|');

    char str_num[11], str_val[11];
    sprintf(str_num, "%d", number);
    sprintf(str_val, "%.2f", value);

    columnPrint(string, 31);
    columnPrint(str_num, 12);
    columnPrint(str_val, 12);

    printf("\n");
}

// 30 | 11 | 11 |
const char * HEADER = "|Key 1                         |Key 2      |Value      |\n";
const char * DELIM =  "|------------------------------|-----------|-----------|\n";

void tablePrint(Table *table) {
    printf("%s", HEADER);
    printf("%s", DELIM);
    for (int i = 0; i < table->size; i++) {
        linePrint(
                table->data[i].key.string,
                table->data[i].key.number,
                table->data[i].value
                );
    }   
    printf("%s", DELIM);
}

void printLineInfoById(Table *table, int id) {
    assert(id >= 0);
    Element elem = table->data[id];
    printf("Element: | Key 1: %s | Key 2: %d | Value: %.2f |\n", elem.key.string, elem.key.number, elem.value);
}

// SORT

int keyscmp(Element elem1, Element elem2) { // -1 < || 1 > || 0 == 
    int res = strcmp(elem1.key.string, elem2.key.string);
    if (res != 0) return (res > 0) ? 1 : ((res < 0) ? -1 : 0);
    return (elem1.key.number > elem2.key.number) - (elem1.key.number < elem2.key.number);
}

void swap(Table *table, int a, int b) {
    Element temp = table->data[a];
    table->data[a] = table->data[b];
    table->data[b] = temp;
}

void heapify(Table *table, int n, int i) {
    int largest = i;
    int left = 2 * i + 1;
    int right = 2 * i + 2;

    if (left < n && keyscmp(table->data[left], table->data[largest]) == 1)
        largest = left;

    if (right < n && keyscmp(table->data[right], table->data[largest]) == 1)
        largest = right;

    if (largest != i) {
        swap(table, i, largest);
        heapify(table, n, largest);
    }
}

void buildHeap(Table *table) {
    int startId = (table->size) / 2 - 1;
    for (int i = startId; i >= 0; i--) {
        heapify(table, table->size, i);
    }
}

void tableHeapSort(Table *table) {
    buildHeap(table);

    for (int i = table->size - 1; i > 0; i--) {
        if (keyscmp(table->data[0], table->data[i]) != 0)
            swap(table, 0, i);       
        heapify(table, i, 0);
    }
    TABLE_IS_SORTED = true;
}


// SEARCH

int tableBinarySearch(Table *table, char *keys) {
    Element target;
    int result = sscanf(keys, "%27[^;];%d", target.key.string, &target.key.number);
    if (result != 2) {
        printf("Error: incorrect input format\n");
        return;
    }

    int left = -1;
    int right = table->size;
    int middle;

    while (right - left > 1) {
        middle = (right + left) / 2;
        if (keyscmp(table->data[middle], target) == -1) 
            left = middle;
        else 
            right = middle;
    }
    return (0 <= right && right < table->size) ? right : -1; 
}

// \TableInterface ===


// int main() {

//     Table *table = tableCreate();
//     tablePush(table, "b;20;11");
//     tablePush(table, "b;21;12");

//     tableDestroy(table);
//     return 0;
// }
