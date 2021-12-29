#include "libmx.h"

char *mx_replace_substr(const char *str, const char *sub, const char *replace) {

    if (str == NULL || sub == NULL || replace == NULL) {
        return NULL;
    }

    int str_len = mx_strlen(str);
    int sub_len = mx_strlen(sub);
    int replace_len = mx_strlen(replace);

    if (str_len < sub_len) return NULL;

    int len_of_replace = replace_len - sub_len;
    int replaced_str_len = str_len + len_of_replace * mx_count_substr(str, sub);
    char *replaced_str = mx_strnew(replaced_str_len);

    for (int i = 0; i < replaced_str_len; i++, str++) {
        while (mx_strncmp(str, sub, sub_len) == 0) {
            str += sub_len;

            for (int j = 0; j < replace_len; i++, j++) {
                replaced_str[i] = replace[j];
            }
        }

        replaced_str[i] = *str;
        
    }

    return replaced_str;
}
