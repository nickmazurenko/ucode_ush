#include "../inc/ush.h"

void mx_apply_escapes(char ***arr) {
    char **data = *arr;

    // Loop SPACE
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

    // Loop for other escapes
    for (int i = 0; data[i] != NULL; i++) {
        //int len = mx_strlen(data[i]);

        char *ptr = mx_strchr(data[i], '\\');
        while ( ptr != NULL ) {
            switch (*(ptr + 1))
            {
            case 'n':
                *ptr = '\n';
                break;
            case 't':
                *ptr = '\t';
                break;
            case '\\':
                *ptr = '\\';
                break;
            case '\'':
                *ptr = '\'';
                break;
            case '"':
                *ptr = '\"';
                break;
            case '`':
                *ptr = '`';
                break;
            case 'a':
                *ptr = '\a';
                break;
            default:
                break;
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
    //mx_print_strarr(data, "\n");
}
