#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "abit.h"
#include <stdlib.h>

bool readBase(Abit * abit, FILE * file) {
	char charGender;
	int intIsMedalist;
	int res = fscanf(file, "%[^;];%c%c;%c;%d;%d;%d;%d;%d\n",
            abit->surname,
            &abit->initials[0],
            &abit->initials[1],
            &charGender,
            &abit->schoolNumber,
            &abit->marks[0],
            &abit->marks[1],
            &abit->marks[2],
            &intIsMedalist);
    if (res == 9) {
        abit->gender = (charGender == 'M') ? true : false;
        abit->isMedalist = (bool)intIsMedalist;
        return true;
    }
    return false;
}


int main(int argc, char **argv) {

    if (argc != 3) {
        printf("Неверный ввод\n");
        return 1;
    }

    FILE * input = fopen(argv[1], "r");
    if (input == NULL) {
        perror("Ошибка открытия файла");
        exit(1);
    } 

    FILE * bin = fopen(argv[2], "wb");
    if (bin == NULL) {
        perror("Ошибка открытия файла");
        exit(1);
    } 

    Abit abit;
    memset(&abit, 0, sizeof(Abit));

    while (readBase(&abit, input)) {
        fwrite(&abit, sizeof(Abit), 1, bin);
        if(ferror(bin) !=0){

        }
    }
    if(ferror(input) !=0){

    }
    fclose(input);
    fclose(bin);
    return 0;
}