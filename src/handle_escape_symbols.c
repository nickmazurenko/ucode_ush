#include "ush.h"

void handle_escape_symbols(char ***arr) {
    char **data = *arr;

    for (int i = 0; data[i] != NULL; i++) {
        int len = mx_strlen(data[i]);
        if (data[i][len - 1] == '\\') {
            data[i][len - 1] = ' ';
            data[i] = mx_strjoin(data[i], data[i + 1]);
            free(data[i + 1]);
            data[i + 1] = NULL;
            for (int j = i + 1; data[j + 1] != NULL; j++) {
                char *tmp = data[j];
                data[j] = data[j + 1];
                data[j + 1] = tmp;
            }
            i--;
        }
    }

    for (int i = 0; data[i] != NULL; i++) {

        char *ptr = mx_strchr(data[i], '\\');
        while ( ptr != NULL ) {
            if (*(ptr + 1) == 'n') {
                *ptr = '\n';
            } else if (*(ptr + 1) == 't') {
                *ptr = '\t';
            } else if (*(ptr + 1) == '\\') {
                *ptr = '\\';
            } else if(*(ptr + 1) == '\'') {
                *ptr = '\'';
            } else if(*(ptr + 1) == '"') {
                *ptr = '\"';
            } else if(*(ptr + 1) == '`') {
                *ptr = '`';
            } else if(*(ptr + 1) == 'a') {
                *ptr = '\a';
            }
            
            ptr++;
            *ptr = '\0';
            for (; *(ptr + 1) != '\0';) {
                mx_swap_char(ptr, ptr + 1);
                ptr++;
            }

            ptr = mx_strchr(data[i], '\\');
        }

    }
}