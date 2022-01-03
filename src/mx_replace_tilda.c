#include "../inc/ush.h"

static void move_to_parent(char **str);

void mx_replace_tilda(char **str) {
    char *tmp = NULL;
    int index = -1;

    int pwd_len = mx_strlen(t_global.PWD);
    int oldpwd_len = mx_strlen(t_global.OLDPWD);
    int home_len = mx_strlen(t_global.HOME);

    // Loop for '~+/' which equals to PWD
    while (true) {
        index = mx_get_substr_index(*str, "~+/");
        if (index == -1)
            break;
        tmp = malloc(PATH_MAX);
        mx_memset(tmp, 0, PATH_MAX);
        mx_memcpy(tmp, *str, index);
        mx_memcpy(tmp + index, t_global.PWD, pwd_len);
        if (tmp[index + pwd_len - 1] == '/' && (*str)[index + 2] == '/') {
            tmp[index + pwd_len - 1] = '\0';
            pwd_len--;
        }
        mx_memcpy(tmp + index + pwd_len, 
            *str + index + 2, PATH_MAX - index - 2 - pwd_len);
        free(*str);
        *str = mx_strdup(tmp);
        free(tmp);
        tmp = NULL;
    }

    // Loop for '~-/' which equals to OLDPWD
    while (true) {
        index = mx_get_substr_index(*str, "~-/");
        if (index == -1)
            break;
        tmp = malloc(PATH_MAX);
        mx_memset(tmp, 0, PATH_MAX);
        mx_memcpy(tmp, *str, index);
        mx_memcpy(tmp + index, t_global.OLDPWD, oldpwd_len);
        mx_memcpy(tmp + index + oldpwd_len, 
            *str + index + 2, PATH_MAX - index - 2 - oldpwd_len);
        free(*str);
        *str = mx_strdup(tmp);
        free(tmp);
        tmp = NULL;
    }

    // Loop for '~/' which equals to HOME
    while (true) {
        index = mx_get_substr_index(*str, "~/");
        if (index == -1)
            break;
        tmp = malloc(PATH_MAX);
        mx_memset(tmp, 0, PATH_MAX);
        mx_memcpy(tmp, *str, index);
        mx_memcpy(tmp + index, t_global.HOME, home_len);
        mx_memcpy(tmp + index + home_len, 
            *str + index + 1, PATH_MAX - index - 1 - home_len);
        free(*str);
        *str = mx_strdup(tmp);
        free(tmp);
        tmp = NULL;
    }

    // Loop for '~user/foo'
    while (true) {
        index = mx_get_char_index(*str, '~');
        char *ptr = mx_strchr(*str, '~');
        if (index == -1 || (*str)[index + 1] == ' ' || (*str)[index + 1] == '\0' || !mx_isalpha(*(ptr + 1)))
            break;
        tmp = malloc(PATH_MAX);
        mx_memset(tmp, 0, PATH_MAX);
        mx_memcpy(tmp, *str, index);
        mx_memcpy(tmp + index, t_global.HOME, home_len);

        move_to_parent(&tmp);

        mx_memcpy(tmp + mx_strlen(tmp), 
            *str + index + 1, PATH_MAX - index - 1 - home_len);
        free(*str);
        *str = mx_strdup(tmp);
        free(tmp);
        tmp = NULL;
    }

    // Loop for '~' which equals to HOME
    while (true) {
        index = mx_get_char_index(*str, '~');
        if (index == -1)
            break;
        
        //*str = rep_substr(*str, "~", t_global.HOME);
        tmp = malloc(PATH_MAX);
        mx_memset(tmp, 0, PATH_MAX);
        mx_memcpy(tmp, *str, index);
        mx_memcpy(tmp + index, t_global.HOME, home_len);
        mx_memcpy(tmp + index + home_len, 
            *str + index + 1, PATH_MAX - index - 1 - home_len);
        free(*str);
        *str = mx_strdup(tmp);
        free(tmp);
        tmp = NULL;
    }
}

static void move_to_parent(char **str) {
    char *ptr = strrchr(*str, '/');
    if (ptr)
        mx_memset(ptr + 1, '\0', mx_strlen(ptr));
    else
        (*str)[0] = '/';
}
