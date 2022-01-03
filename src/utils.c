#include "ush.h"

char *mx_strstr_new(const char *haystack, const char *needle) {

    if (*needle == '\0') return (char *)haystack;
    while (1)
    {
        haystack = mx_strchr(haystack, needle[0]);
        if (haystack == NULL) break;
        if (mx_strncmp(haystack, needle, mx_strlen(needle)) == 0) {
            return (char *)haystack;
        }
        if (*haystack == '\0') break;
        haystack++;
    }

    return NULL;
}

int mx_count_substr_new(const char* str, char* sub) {

    if (str == NULL || sub == NULL) return -1;
    int result = 0;

    while (1) {
        str = mx_strstr_new(str, sub);
        if (str) {
            result++;
            if (*str != '\0') {
                str++;
            }
            if (*str == '\0') break;
        } else
            break;
    }

    return result;
}

void mx_print_str_arr(char** arr) {
    for (int i = 0; arr[i] != NULL; i++) {
        printf("%i: %s\n", i, arr[i]);
    }
}

int mx_atoi(char *str) {
    int result = 0;
    int sign = 1;

    if (*str == '-') {
        sign = -1;
        str++;
    };

    while (mx_isdigit(*str) && *str != '\0')
    {
        result = result * 10 + (*str - '0');
        str++;
    }

    return sign * result;
}
