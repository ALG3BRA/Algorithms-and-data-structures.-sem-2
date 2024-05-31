#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char** split_string(const char* str, const char* operators, int* num_tokens) {
    char** tokens = NULL;
    int count = 0;
    const char* p = str;
    
    while (*p != '\0') {
        // Пропускаем пробелы
        while (isspace(*p)) {
            p++;
        }
        
        // Определяем, является ли текущий символ оператором
        if (strchr(operators, *p) != NULL) {
            // Если да, добавляем его в качестве отдельного токена
            tokens = realloc(tokens, (count + 1) * sizeof(char*));
            if (tokens == NULL) {
                return NULL;
            }
            tokens[count] = malloc(2); // Для операторов выделяем 2 байта (символ и завершающий нуль)
            if (tokens[count] == NULL) {
                return NULL;
            }
            tokens[count][0] = *p;
            tokens[count][1] = '\0';
            count++;
            p++;
        } else {
            // Если текущий символ не оператор, считываем слово
            const char* start = p;
            while (*p != '\0' && !isspace(*p) && strchr(operators, *p) == NULL) {
                p++;
            }
            int len = p - start;
            if (len > 0) {
                tokens = realloc(tokens, (count + 1) * sizeof(char*));
                if (tokens == NULL) {
                    return NULL;
                }
                tokens[count] = malloc(len + 1); // Выделяем память для слова
                if (tokens[count] == NULL) {
                    return NULL;
                }
                strncpy(tokens[count], start, len); // Копируем слово в токен
                tokens[count][len] = '\0'; // Добавляем завершающий нуль
                count++;
            }
        }
    }
    
    *num_tokens = count;
    return tokens;
}

void free_tokens(char** tokens, int num_tokens) {
    for (int i = 0; i < num_tokens; i++) {
        free(tokens[i]);
    }
    free(tokens);
}

int main() {
    const char* str = "hello+world- \thow*are/you";
    const char* operators = "+-*/";
    int num_tokens = 0;

    char** tokens = split_string(str, operators, &num_tokens);
    if (tokens == NULL) {
        printf("Ошибка при разделении строки.\n");
        return 1;
    }

    printf("Результат разделения:\n");
    for (int i = 0; i < num_tokens; i++) {
        printf("%s\n", tokens[i]);
    }

    free_tokens(tokens, num_tokens);

    return 0;
}
