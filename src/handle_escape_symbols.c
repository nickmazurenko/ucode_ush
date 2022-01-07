#include "ush.h"

void handle_escape_symbols(char ***arr) {
    char **data = *arr;

    for (int i = 0; data[i] != NULL; i++) {
        int len = mx_strlen(data[i]);
        if (data[i][len - 1] == '\\') {
            data[i][len - 1] = ' ';
            data[i] = mx_strjoin(data[i], data[i + 1]);
            if (data[i + 1] != NULL) {
                free(data[i + 1]);
            }
            data[i + 1] = NULL;
            for (int j = i + 1; data[j] != NULL && data[j + 1] != NULL; j++) {
                char *tmp = data[j];
                data[j] = data[j + 1];
                data[j + 1] = tmp;
            }
            i--;
            mx_printerr("there\n");
        }
    }
    for (int i = 0; data[i] != NULL; i++) {
        char *ptr = mx_strchr(data[i], '\\');
        int is_changed = 0;

        while ( ptr != NULL ) {
            if (*(ptr + 1) == 'n') {
                *ptr = '\n';
                is_changed = 1;
            } else if (*(ptr + 1) == 't') {
                *ptr = '\t';
                is_changed = 1;
            } else if (*(ptr + 1) == '\\') {
                *ptr = '\\';
                is_changed = 1;
            } else if (*(ptr + 1) == '\'') {
                *ptr = '\'';
                is_changed = 1;
            } else if (*(ptr + 1) == '\"') {
                *ptr = '\"';
                is_changed = 1;
            } else if (*(ptr + 1) == '`') {
                *ptr = '`';
                is_changed = 1;
            } else if (*(ptr + 1) == 'a') {
                *ptr = '\a';
                is_changed = 1;
            } else if (*(ptr + 1) == '0') {
                *ptr = '\0';
                is_changed = 1;
            }
            
            ptr++;
            char *from_ptr = ptr;
            if (is_changed) {
                *ptr = '\0';
                for (;*(ptr + 1) != '\0';) {
                    mx_swap_char(ptr, ptr + 1);
                    ptr++;
                }
            }
            
            is_changed = 0;
            ptr = mx_strchr(from_ptr, '\\');
        }
    }
}
