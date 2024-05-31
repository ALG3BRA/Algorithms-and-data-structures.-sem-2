#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <stdbool.h>
#include "abit.h"

void printUsage() {
    printf("Неверный ввод.\n");
    printf("main <filename> -f - печать таблицы.\n");
    printf("main <filename> -p <value> - печать части таблицы, в которой среднее значение всех оценок больше value.\n");
}


// ./a.out filename -f
// ./a.out filename -p 30

// 20 | 8 | 6 | 13 | 6 | 6 | 6 | 8
const char * HEADER = "|Surname             |Initials|Gender|School Number|Mark 1|Mark 2|Mark 3|Medalist|\n";
const char * DELIM =  "|--------------------|--------|------|-------------|------|------|------|--------|\n";


void printAbitColumn(char * string, int n) {
    printf("%s", string);
    int len = strlen(string);
    for (int i = 0; i < n - len - 1; i++) {
        putchar(' ');
    }
    printf("|");
}


void printAbit(Abit * abit) {
    putchar('|');

    printAbitColumn(abit->surname, 21);
    printAbitColumn(abit->initials, 10);

    
    char gender = (abit->gender == 0) ? 'W' : 'M';
    
    printf("%c     |", gender);

    char school_number[8], mark1[4], mark2[4], mark3[4];  
    sprintf(school_number, "%d", abit->schoolNumber);
    sprintf(mark1, "%d", abit->marks[0]);
    sprintf(mark2, "%d", abit->marks[1]);
    sprintf(mark3, "%d", abit->marks[2]);

    printAbitColumn(school_number, 14);
    printAbitColumn(mark1, 7);
    printAbitColumn(mark2, 7);
    printAbitColumn(mark3, 7);

    char isMedalist = (abit->isMedalist == 0) ? '-' : '+';
    printf("%c       |", isMedalist);
    
    printf("\n");
} 

void printTable(FILE * file) {

    printf("%s", HEADER);
    printf("%s", DELIM);

    Abit abit;
    while (fread (&abit, sizeof(Abit), 1, file) == 1) {
        printAbit(&abit);
    }
    printf("%s", DELIM);
}

bool isAvgHigher(Abit * abit, double value) {
    int sumMarks = 0;
    int len = sizeof(abit->marks) / sizeof(abit->marks[0]);
    for (int i = 0; i < len; i++) {
        sumMarks += abit->marks[i];
    }
    double avg = sumMarks / (double)len;
    return avg >= value;
}


void printFilteredTable(FILE * file, double value) {
    printf("%s", HEADER);
    printf("%s", DELIM);

    Abit abit;
    while (fread (&abit, sizeof(Abit), 1, file) == 1) {
        if (isAvgHigher(&abit, value)) {
            printAbit(&abit);
        }      
    }
    printf("%s", DELIM);
}


int main(int argc, char **argv) {
    setlocale(LC_ALL, "UTF-8");
    if (argc < 3 || argc > 4) {
        printUsage();
        return 1;
    }
    
    char filename[50];
    int mode = -1; // 1 - file print; 2 - function;
    double p;

    if (argc == 3) {

        if (strcmp(argv[2], "-f") == 0) {
            mode = 1;
        } else {
            printUsage();
            return 1;
        }

    } else if (argc == 4) {
        if (strcmp(argv[2], "-p") == 0) {
            mode = 2;
        } else {
            printUsage();
            return 1;
        }
        
        char *endptr;
        p = strtod(argv[3], &endptr);
        if (*endptr != '\0') {
            printf("Ошибка: не удалось преобразовать строку '%s' в число.\n", argv[3]);
            return 1;
        }
    }

    strcpy(filename, argv[1]);

    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        perror("Ошибка открытия файла");
        exit(1);
    }    
    
    if (mode == 1) {
        printTable(file);
    } else if (mode == 2) {
        printFilteredTable(file, p);
    }

    fclose(file);
}