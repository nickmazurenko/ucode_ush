#include "ush.h"
#include "echo.h"
#include "utils.h"

// static void process_echo_args(char **args);
static void del_extra_spaces(char **str);
t_echo_flags *create_echo_flags(void);
int find_echo_flags(t_echo_flags *echo_flags, char **args);

static char *replace_back_slash(char *str, t_echo_flags *echo_flag) {
    char *res = (char *)malloc(mx_strlen(str) + 1);
    int len = 0;

    for (int i = 0; i <= mx_strlen(str); i++) {
        if (str[i] == '\\' && str[i + 1] == '\\')
            i++;
        
        if (str[i] == '\\' && str[i + 1] == 'e') {
            if (str[i + 2] != '\\')
                i += 3;
            else
                i += 2;
        }
        
        if (str[i] == '\\' && str[i + 1] == 'c') {
            echo_flag->is_n_flag = true;
            break;
        }
        res[len] = str[i];
        len++;
    }
    res[len] = '\0';
    return res;
}

void process_echo_args(char **args, int idx, t_echo_flags *echo_flag) {
    char *tmp = replace_back_slash(args[idx], echo_flag);
    
    char *sequenses[] = {"\\a","\\b","\\f","\\n","\\r","\\t","\\v", NULL};
    char *escape[] = {"\a","\b","\f","\n","\r","\t","\v", NULL};

    free(args[idx]);
    args[idx] = mx_strdup(tmp);
    free(tmp);
    for (int j = 0; sequenses[j] != NULL; j++) {
        if (strstr(args[idx],sequenses[j])) {
            tmp = mx_replace_substr_new(args[idx],sequenses[j], escape[j]);
            free(args[idx]);
            args[idx] = mx_strdup(tmp);
            free(tmp);
        }
    }
}




int clear_echo(t_echo_flags *echo_flag, char **args) {
    del_extra_spaces(args);
    *args = mx_strtrim(*args);
    if (!echo_flag->is_E_flag && strstr(args[0], "\\") && mx_get_char_index(args[0], '\"') != -1) 
        process_echo_args(args, 0, echo_flag);
    
    if (echo_flag->is_E_flag)
        args[0] = replace_back_slash(args[0], echo_flag);
    // TODO check if macos works the same
    char **data = mx_strsplit(*args, '>');
    char *ptr = data[0];
    char *tmp_ptr = ptr;
    while (*tmp_ptr != '\0' && *tmp_ptr != ' ') {
        *tmp_ptr = '\0';
        while (*(tmp_ptr + 1) != '\0') {
            mx_swap_char(tmp_ptr, tmp_ptr + 1);
            tmp_ptr++;
        }
        tmp_ptr = ptr;
    }
    *tmp_ptr = '\0';
    while (*(tmp_ptr + 1) != '\0') {
        mx_swap_char(tmp_ptr, tmp_ptr + 1);
        tmp_ptr++;
    }
    tmp_ptr = ptr;
    //TODO
    if (*tmp_ptr == '-' && !echo_flag->is_no_flag) {
        while (*tmp_ptr != '\0' && *tmp_ptr != ' ') {
            *tmp_ptr = '\0';
            while (*(tmp_ptr + 1) != '\0') {
                mx_swap_char(tmp_ptr, tmp_ptr + 1);
                tmp_ptr++;
            }
            tmp_ptr = ptr;
        }
    }
    if (*tmp_ptr == ' ') {
        *tmp_ptr = '\0';
        while ( *(tmp_ptr + 1) != '\0') {
            mx_swap_char(tmp_ptr, tmp_ptr + 1);
            tmp_ptr++;
        }
//        tmp_ptr = ptr;
    }
//    tmp_ptr = ptr;
    if(echo_flag->is_n_flag) {
        bool isWrite = true;
        char *str = NULL;
        str = mx_strnew(PATH_MAX);
        if(data[1] != NULL) {
            FILE *file;
            char *path = mx_strdup(getenv("PWD"));
            char *new_path = mx_strcat(path, "/");
            new_path = mx_strcat(new_path, data[1]);
            file = fopen(new_path, "w");
            fprintf(file, "%s", ptr);
            fclose(file);
            free(path);
            isWrite = false;
        } else {
            mx_strcat(str, data[0]);
        }
        int count = 0;
        bool is_par = false;
        for (int str_idx = 0; str[str_idx]; str_idx++) {
            if (str[str_idx] == '"' || str[str_idx] == '\'') {
                for (int tmp_idx = str_idx; str[tmp_idx]; tmp_idx++) {
                    str[tmp_idx] = str[tmp_idx + 1];
                }
                count++;
                is_par = true;
            } else if (str[str_idx] == '\\') {
                if (!is_par) {
                    for (int tmp_idx = str_idx; str[tmp_idx]; tmp_idx++) {
                        str[tmp_idx] = str[tmp_idx + 1];
                    }
                }
            }
        }
        if(count % 2 == 0 || count == 0) {
            if(isWrite) {
                mx_printstr(str);
                if(echo_flag->is_no_flag) 
                    mx_printstr("\n");
            }
        } else {
            mx_printerr("Odd number of quotes.\n");
            mx_del_strarr(&data);
            return 1;
        }
    } else if (echo_flag->is_e_flag || echo_flag->is_E_flag) {
        bool isWrite = true;
        char *str = NULL;
        str = mx_strnew(PATH_MAX);
        if(data[1] != NULL) {
            FILE *file;
            char *path = mx_strdup(getenv("PWD"));
            char *new_path = mx_strcat(path, "/");
            new_path = mx_strcat(new_path, data[1]);
            file = fopen(new_path, "w");
            fprintf(file, "%s", ptr);
            fclose(file);
            free(path);
            isWrite = false;
        } else {
            mx_strcat(str, ptr);
        }
        int count = 0;
        bool is_par = false;
        for (int str_idx = 0; str[str_idx]; str_idx++) {
            if (str[str_idx] == '"' || str[str_idx] == '\'') {
                for (int tmp_idx = str_idx; str[tmp_idx]; tmp_idx++) {
                    str[tmp_idx] = str[tmp_idx + 1];
                }
                count++;
                is_par = true;
            } else if (str[str_idx] == '\\') {
                if (!is_par) {
                    for (int tmp_idx = str_idx; str[tmp_idx]; tmp_idx++) {
                        str[tmp_idx] = str[tmp_idx + 1];
                    }
                }
            }
        }
        if(count % 2 == 0 || count == 0) {
            if(isWrite) {
                mx_printstr(str);
                mx_printchar('\n');
            }
        } else {
            mx_printerr("Odd number of quotes.\n");
            mx_del_strarr(&data);
            return 1;
        }
    } else {
        // printf("ptr %s\n", ptr);
        bool isWrite = true;
        char *str = NULL;
        str = mx_strnew(PATH_MAX);
        if(data[1] != NULL) {
            FILE *file;
            char *path = mx_strdup(getenv("PWD"));
            char *new_path = mx_strcat(path, "/");
            new_path = mx_strcat(new_path, data[1]);
            file = fopen(new_path, "w");
            fprintf(file, "%s", ptr);
            fclose(file);
            free(path);
            isWrite = false;
        } else {
            mx_strcat(str, ptr);
        }
        int count = 0;
        bool is_par = false;
        for (int str_idx = 0; str[str_idx]; str_idx++) {
            if (str[str_idx] == '"' || str[str_idx] == '\'') {
                for (int j = str_idx; str[j]; j++) {
                    str[j] = str[j + 1];
                }
                count++;
                is_par = true;
            } else if (str[str_idx] == '\\') {
                if (!is_par) {
                    for (int tmp_idx = str_idx; str[tmp_idx]; tmp_idx++) {
                        str[tmp_idx] = str[tmp_idx + 1];
                    }
                }
            }
        }
        if(count % 2 == 0 || count == 0) {
            if (isWrite) {
                mx_printstr(str);
                mx_printchar('\n');
            }
        } else {
            mx_printerr("Odd number of quotes.\n");
            mx_del_strarr(&data);
            return 1;
        }
        free(str);
    }
    mx_del_strarr(&data);
    return 0;
}

// static void process_echo_args(char **args) {
//     char *str_ptr = mx_strchr(*args, '"');
//     if (str_ptr != NULL) {
//         char *slash_ptr = mx_strchr(str_ptr, '\\');
//         while ( slash_ptr != NULL ) {
//             if (*(slash_ptr + 1) == 'n') {
//                 *slash_ptr = '\n';
//             } else if (*(slash_ptr + 1) == 't') {
//                 *slash_ptr = '\t';
//             } else if (*(slash_ptr + 1) == '\\') {
//                 *slash_ptr = '\\';
//             } else if (*(slash_ptr + 1) == 'a') {
//                 *(slash_ptr + 1) = '\a';
//             } else if (*(slash_ptr + 1) == 'v') {
//                 *slash_ptr = '\v';
//             } else if (*(slash_ptr + 1) == 'b') {
//                 *slash_ptr = '\b';
//             } else if (*(slash_ptr + 1) == 'f') {
//                 *slash_ptr = '\f';
//             } else if (*(slash_ptr + 1) == 'r') {
//                 *slash_ptr = '\r';
//             }

//             mx_printerr(str_ptr);
//             slash_ptr++;
//             *slash_ptr = '\0';
//             while (*(slash_ptr + 1) != '\0') {
//                 mx_swap_char(slash_ptr, slash_ptr + 1);
//                 slash_ptr++;
//             }

//             slash_ptr = str_ptr + 1;

//             if (mx_get_char_index(slash_ptr, '\\') > mx_get_char_index(slash_ptr, '"')
//                 || mx_get_char_index(slash_ptr, '\\') == -1) {
//                 str_ptr = mx_strchr(slash_ptr, '\'');
//                 break;
//             }
//             slash_ptr = mx_strchr(str_ptr, '\\');
//         }
//     } else {
//         str_ptr = mx_strchr(*args, '\'');
//     }

//     if (str_ptr != NULL) {
//         char *slash_ptr = mx_strchr(str_ptr, '\\');
//         while ( slash_ptr != NULL ) {
//             if (*(slash_ptr + 1) == 'n') {
//                 *slash_ptr = '\n';
//             } else if (*(slash_ptr + 1) == 't') {
//                 *slash_ptr = '\t';
//             } else if (*(slash_ptr + 1) == '\\') {
//                 *slash_ptr = '\\';
//             } else if (*(slash_ptr + 1) == 'a') {
//                 *(slash_ptr + 1) = '\a';
//             } else if (*(slash_ptr + 1) == 'v') {
//                 *slash_ptr = '\v';
//             } else if (*(slash_ptr + 1) == 'b') {
//                 *slash_ptr = '\b';
//             } else if (*(slash_ptr + 1) == 'f') {
//                 *slash_ptr = '\f';
//             } else if (*(slash_ptr + 1) == 'r') {
//                 *slash_ptr = '\r';
//             }

//             slash_ptr++;
//             *slash_ptr = '\0';
//             while (*(slash_ptr + 1) != '\0') {
//                 mx_swap_char(slash_ptr, slash_ptr + 1);
//                 slash_ptr++;
//             }

//             if (mx_get_char_index(slash_ptr, '\\') > mx_get_char_index(slash_ptr, '\'')
//                 || mx_get_char_index(slash_ptr, '\\') == -1)
//                 break;
//             slash_ptr = mx_strchr(str_ptr, '\\');
//         }
//     }
// }

// TODO: change mx_delete_extra_spaces on mx_del_extra_spaces
static void del_extra_spaces(char **str) {
    if (strchr(*str, '"') || strchr(*str, '\''))
        return;
    char *buff = *str;
    *str = mx_del_extra_spaces(*str);
    free(buff);
}

t_echo_flags *create_echo_flags(void) {
    t_echo_flags *echo_flag = (t_echo_flags*)malloc(sizeof(t_echo_flags));
    echo_flag->is_n_flag = false;
    echo_flag->is_e_flag = false;
    echo_flag->is_E_flag = false;
    echo_flag->is_no_flag = false;
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
                    echo_flags->is_no_flag = true;
                    return 0;
                }
            }
        }
    }
    return 0;
}
