#include "ush.h"

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

