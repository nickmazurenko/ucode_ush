#include "../inc/ush.h"

static void process_echo_args(char **str);
static void del_extra_spaces(char **str);
t_echo_flags *create_echo_flags(void);
int find_echo_flags(t_echo_flags *echo_flags, char **args);


int clear_echo(t_echo_flags *echo_flag, char **args) {
    del_extra_spaces(args);
    *args = mx_strtrim(*args);
    process_echo_args(args);

    char **data = mx_strsplit(*args, '>');
    char *ptr = data[0];
    char *tmp_ptr = ptr;
    while (*tmp_ptr != '\0' && *tmp_ptr != ' ') {
        *tmp_ptr = '\0';
        for (; *(tmp_ptr + 1) != '\0';) {
            mx_swap_char(tmp_ptr, tmp_ptr + 1);
            tmp_ptr++;
        }
        tmp_ptr = ptr;
    }
    *tmp_ptr = '\0';
    for (; *(tmp_ptr + 1) != '\0';) {
        mx_swap_char(tmp_ptr, tmp_ptr + 1);
        tmp_ptr++;
    }
    tmp_ptr = ptr;
    if (*tmp_ptr == '-') {
        while (*tmp_ptr != '\0' && *tmp_ptr != ' ') {
            *tmp_ptr = '\0';
            for (; *(tmp_ptr + 1) != '\0';) {
                mx_swap_char(tmp_ptr, tmp_ptr + 1);
                tmp_ptr++;
            }
            tmp_ptr = ptr;
        }
    }
    if (*tmp_ptr == ' ') {
        *tmp_ptr = '\0';
        for (; *(tmp_ptr + 1) != '\0';) {
            mx_swap_char(tmp_ptr, tmp_ptr + 1);
            tmp_ptr++;
        }
        tmp_ptr = ptr;
    }

    tmp_ptr = ptr;

    if(echo_flag->is_n_flag) {
        bool isWrite = true;
        char *str = NULL;
        str = mx_strnew(PATH_MAX);
        if(data[1] != NULL) {
            FILE *file;
            char *path = mx_strdup(getenv("PWD"));
            char *newpath = mx_strcat(path, "/");
            newpath = mx_strcat(newpath, data[1]);
            file = fopen(newpath, "w");                
            fprintf(file, "%s", ptr);
            fclose(file);
            free(path);
            isWrite = false;
        }
        else {
            mx_strcat(str, data[0]);
        }
        int count = 0;
        bool parants = false;
        for (int i = 0; str[i]; i++) {
            if (str[i] == '"' || str[i] == '\'') {
                for (int j = i; str[j]; j++) {
                    str[j] = str[j + 1];
                }
                count++;
                parants = true;
            }
            else if (str[i] == '\\') {
                if (!parants) {
                    for (int j = i; str[j]; j++) {
                        str[j] = str[j+1];
                    }
                }
            }
        }
        if(count % 2 == 0 || count == 0) {
            if(isWrite) {
                mx_printstr(str);
            }
        }
        else {
            mx_printerr("Odd number of quotes.\n");
            mx_del_strarr(&data);
            return 1;
        }
    }
    else if (echo_flag->is_e_flag || echo_flag->is_E_flag) {
        bool isWrite = true;
        char *str = NULL;
        str = mx_strnew(PATH_MAX);
        if(data[1] != NULL) {
            FILE *file;
            char *path = mx_strdup(getenv("PWD"));
            char *newpath = mx_strcat(path, "/");
            newpath = mx_strcat(newpath, data[1]);
            file = fopen(newpath, "w");                
            fprintf(file, "%s", ptr);
            fclose(file);
            free(path);
            isWrite = false;
        }
        else {
            mx_strcat(str, ptr);
        }
        int count = 0;
        bool parants = false;
        for (int i = 0; str[i]; i++) {
            if (str[i] == '"' || str[i] == '\'') {
                for (int j = i; str[j]; j++) {
                    str[j] = str[j + 1];
                }
                count++;
                parants = true;
            }
            else if (str[i] == '\\') {
                if (!parants) {
                    for (int j = i; str[j]; j++) {
                        str[j] = str[j+1];
                    }
                }
            }
        }
        if(count % 2 == 0 || count == 0) {
            if(isWrite) {
                mx_printstr(str);
                mx_printchar('\n');
            }
        }
        else {
            mx_printerr("Odd number of quotes.\n");
            mx_del_strarr(&data);
            return 1;
        }
    }
    else {
        bool isWrite = true;
        char *str = NULL;
        str = mx_strnew(PATH_MAX);
        if(data[1] != NULL) {
            FILE *file;
            char *path = mx_strdup(getenv("PWD"));
            char *newpath = mx_strcat(path, "/");
            newpath = mx_strcat(newpath, data[1]);
            file = fopen(newpath, "w");                
            fprintf(file, "%s", ptr);
            fclose(file);
            free(path);
            isWrite = false;
        }
        else {
            mx_strcat(str, ptr);
        }
        int count = 0;
        bool parants = false;
        for (int i = 0; str[i]; i++) {
            if (str[i] == '"' || str[i] == '\'') {
                for (int j = i; str[j]; j++) {
                    str[j] = str[j + 1];
                }
                count++;
                parants = true;
            }
            else if (str[i] == '\\') {
                if (!parants) {
                    for (int j = i; str[j]; j++) {
                        str[j] = str[j+1];
                    }
                }
            }
        }
        if(count % 2 == 0 || count == 0) { 
            if (isWrite) {
                mx_printstr(str);
                mx_printchar('\n');
            }
        }
        else {
            mx_printerr("Odd number of quotes.\n");
            mx_del_strarr(&data);
            return 1;
        }
    }

    mx_del_strarr(&data);
    return 0;
}

static void process_echo_args(char **args) {
    char *str_ptr = mx_strchr(*args, '"');
    if (str_ptr != NULL) {
        char *slash_ptr = mx_strchr(str_ptr, '\\');
        while ( slash_ptr != NULL ) {
            if (*(slash_ptr + 1) == 'n') {
                *slash_ptr = '\n';
            } else if (*(slash_ptr + 1) == 't') {
                *slash_ptr = '\t';
            } else if (*(slash_ptr + 1) == '\\') {
                *slash_ptr = '\\';
            } else if (*(slash_ptr + 1) == 'a') {
                *slash_ptr = '\a';
            } else if (*(slash_ptr + 1) == 'v') {
                *slash_ptr = '\v';
            } else if (*(slash_ptr + 1) == 'b') {
                *slash_ptr = '\b';
            } else if (*(slash_ptr + 1) == 'f') {
                *slash_ptr = '\f';
            } else if (*(slash_ptr + 1) == 'r') {
                *slash_ptr = '\r';
            }

            slash_ptr++;
            *slash_ptr = '\0';

            while (*(slash_ptr + 1) != '\0') {
                mx_swap_char(slash_ptr, slash_ptr + 1);
                slash_ptr++;
            }

            slash_ptr = str_ptr + 1;

            if (mx_get_char_index(slash_ptr, '\\') > mx_get_char_index(slash_ptr, '"')
                || mx_get_char_index(slash_ptr, '\\') == -1) {
                str_ptr = mx_strchr(slash_ptr, '\'');
                break;
            }
            slash_ptr = mx_strchr(str_ptr, '\\');
        }
    } else {
        str_ptr = mx_strchr(*args, '\'');
    }

    if (str_ptr != NULL) {
        char *slash_ptr = mx_strchr(str_ptr, '\\');
        while ( slash_ptr != NULL ) {
            if (*(slash_ptr + 1) == 'n') {
                *slash_ptr = '\n';
            } else if (*(slash_ptr + 1) == 't') {
                *slash_ptr = '\t';
            } else if (*(slash_ptr + 1) == '\\') {
                *slash_ptr = '\\';
            } else if (*(slash_ptr + 1) == 'a') {
                *slash_ptr = '\a';
            } else if (*(slash_ptr + 1) == 'v') {
                *slash_ptr = '\v';
            } else if (*(slash_ptr + 1) == 'b') {
                *slash_ptr = '\b';
            } else if (*(slash_ptr + 1) == 'f') {
                *slash_ptr = '\f';
            } else if (*(slash_ptr + 1) == 'r') {
                *slash_ptr = '\r';
            }

            slash_ptr++;
            *slash_ptr = '\0';
            while (*(slash_ptr + 1) != '\0') {
                mx_swap_char(slash_ptr, slash_ptr + 1);
                slash_ptr++;
            }

            if (mx_get_char_index(slash_ptr, '\\') > mx_get_char_index(slash_ptr, '\'')
                || mx_get_char_index(slash_ptr, '\\') == -1)
                break;
            slash_ptr = mx_strchr(str_ptr, '\\');
        }
    }
}

// TODO: change mx_delete_extra_spaces on mx_del_extra_spaces
static void del_extra_spaces(char **str) {
    if (strchr(*str, '"') || strchr(*str, '\''))
        return;
    *str = mx_del_extra_spaces(*str);
}

t_echo_flags *create_echo_flags(void) {
    t_echo_flags *echo_flag = (t_echo_flags*)malloc(sizeof(t_echo_flags));
    echo_flag->is_n_flag = false;
    echo_flag->is_e_flag = false;
    echo_flag->is_E_flag = false;
    return echo_flag;
}

int find_echo_flags(t_echo_flags *echo_flags, char **args) {
    if (args[1] != NULL) {
        if (args[1][0] == '-') {
            for (int i = 1; i < mx_strlen(args[1]); i++) {
                if (args[1][i] == 'n') {
                    echo_flags->is_n_flag = true;
                } else if (args[1][i] == 'e') {
                    echo_flags->is_e_flag = true;
                    echo_flags->is_E_flag = false;
                } else if (args[1][i] == 'E') {
                    echo_flags->is_E_flag = true;
                    echo_flags->is_e_flag = false;
                } else {
                    return 1;
                }
            }
        }
    }
    return 0;
}