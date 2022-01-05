#include "ush.h"
#include "utils.h"

int mx_get_substr_index_new(char *str, const char *sub_str) {
    if (str == NULL) return  -1;

    char *sub_str_ptr = mx_strstr_new(str ,sub_str);
    if (sub_str_ptr == NULL) return -1;
    else return (int)sub_str_ptr - (int)str;
}

char mx_char_to_upper(char symbol) {
    if (mx_isalpha(symbol)) {
        if (symbol < 97) symbol += 32;
    }
    return symbol;
}

void str_to_upper_case(char* str) {
    if (str == NULL) return;
    for (; *str != '\0'; str++) {
        if (mx_isalpha(*str)) {
            if (*str < 97) {
                *str = *str + 32;
            }
        }
    }
}

void str_shift_left(char *str, char stop_symbol) {
    if (str == NULL) return;
    char *tmp = str;
    for (; *(tmp + 1) != stop_symbol; tmp++) {
        mx_swap_char(tmp, (tmp + 1));
    }
}

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
