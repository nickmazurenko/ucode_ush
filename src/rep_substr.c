#include "../inc/ush.h"

char *rep_substr(char *str, char *substr, char *new_str) {
    int index = mx_get_substr_index(str, substr);
    if (index == -1)
        return NULL;
    int new_len = mx_strlen(new_str);
    int len = mx_strlen(str) + mx_strlen(new_str);
    char *res = mx_strnew(len);
    mx_memcpy(res, str, index);
    if (new_len > 0)
        mx_memcpy(res + index, new_str, new_len);
    mx_memcpy(res + index + new_len, 
        str + index + mx_strlen(substr), len - index - mx_strlen(substr) - new_len);
    free(str);

    return res;
}
