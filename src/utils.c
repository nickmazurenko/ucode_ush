#include "ush.h"
#include "utils.h"

// TODO: как определить что нужно удалять back slash

bool mx_is_screenable(char c) {
    char screenable[] = {'a', 'b', 'f', 'n', 'r', 't', 'v', '"', '\'', '`', '\\'};
    int screenable_number = 11;

    for (int screenable_index = 0; screenable_index < screenable_number; screenable_index++) {
        if (c == screenable[screenable_index]) {
            return true;
        }
    }

    return false;
}

void delete_one_streak_back_slash(char **str) {
    char *back_slash = mx_strchr(*str, '\\');

    while (back_slash != NULL)
    {
        if (!mx_is_screenable(*(back_slash + 1)) && *(back_slash + 1) != '\0') {
            *back_slash = '\0';
            char *buff = back_slash;
            while (*(buff + 1) != '\0')
            {
                mx_swap_char(buff, buff + 1);
                buff++;
            }
            
        } else {
            back_slash++;
        }
        back_slash = mx_strchr(back_slash, '\\');
    }
    
}


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

char *mx_strjoin_nleak(char const *s1, char const *s2) {
    if(s1 == NULL && s2 == NULL) return NULL;
    else if(s1 == NULL) return mx_strdup(s2);
    else if(s2 == NULL) return mx_strdup(s1);
    char *result = mx_strnew(mx_strlen(s1) + mx_strlen(s2));
    result = mx_strcat(result, s1);
    result = mx_strcat(result, s2);
    char *free = (char*)s1;
    mx_strdel(&free);
    return result;
}

void delete_back_slash(char *str, int back_slash_idx) {
    if(back_slash_idx != -1) {
            mx_memmove(&str[back_slash_idx], &str[back_slash_idx + 1], mx_strlen(str) - back_slash_idx);
    }
}

void delete_back_slashes(char *str) {

    // mx_printstr(str);
    int back_slash_idx = 0;

    int slashes_number = mx_count_substr_new(str, "\\");
    while (slashes_number % 2 != 0 && (back_slash_idx = mx_get_char_index(str, '\\')) != -1) {
        delete_back_slash(str, back_slash_idx);
        slashes_number--;
    }

    
}

void screen_back_slashes(char **str) {
    int back_slash_index = 0;

    int shift = 0;

    while ((back_slash_index = mx_get_char_index(*str + shift, '\\')) != -1)
    {   
        int str_len = mx_strlen(*str);
        if (shift + back_slash_index < str_len - 1) {
            if ((*str)[shift + back_slash_index + 1] == '\\') {
                *str = replace_substr_new(*str, "\\", "");
            }
        }
        shift += back_slash_index + 1;
    }
}
