#include "textmenu.h"
#include "table.h"

// C & D
void *creator_table() {
    Table *table = tableCreate();
    return (void*)table;
}

void destroyer_table(void *obj) {
    Table *table = (Table*)obj;
    tableDestroy(table);
}

// Handlers
void table_add_record(void *obj) {
    Table *table = (Table*)obj;
    char string[28];
    scanf("%s", string);
    tablePush(table, string);
}

void table_sort(void *obj) {
    Table *table = (Table*)obj;
    tableHeapSort(table);
}

void table_find_element(void *obj) {
    Table *table = (Table*)obj;

    if (tableIsEmpty(table)) {
        printf("Table is empty.\n");
        return;
    }
    if (!(TABLE_IS_SORTED)) {
        printf("Cannot find element. The table is not sorted.\n");
        return;
    }

    char string[38]; //int 10 + string 28
    scanf("%s", string);
    int id = tableBinarySearch(table, string);
    if (id < 0 || id >= table->size) {
        printf("This element doesn't exist.\n");
        return;
    }
    printLineInfoById(table, id);
}

void table_print(void *obj) {
    Table *table = (Table*)obj;
    tablePrint(table);
}


int main() {
    textMenuFastInitialize(4,
                           "print", "print - prints table.", table_print,
                           "add", "add <key1;key2;value> - adds line to table.", table_add_record,
                           "sort", "sort - sorts table.", table_sort,
                           "find", "find <key1;key2> - finds line in table by keys", table_find_element
                            );
    textMenuFastStart(creator_table, destroyer_table);
    textMenuFastClose();
}