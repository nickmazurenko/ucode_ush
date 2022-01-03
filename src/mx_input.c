#include "../inc/ush.h"

int mx_input(char **str, int win_len) {
    int res = read(0, *str, win_len);
    if (!res) {
        size_t n = win_len - 5;
        res = getline(str, &n, 0);
    }
    (*str)[res - 1] = '\0';
    return res - 1;
}
