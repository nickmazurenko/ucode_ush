#include "ush.h"
#include "utils.h"
#include "pwd.h"

t_pwd_flags *create_pwd_flags(void);
void print_pwd_error(char wrong_flag);
int find_pwd_flags(t_pwd_flags *pwd_flag, char **flags);

int get_last_char_index(char *str, char c) {
    if (str == NULL) return -2;

    for (int i = mx_strlen(str); ; i--) {
        if (str[i] == c) return i;
        if (i == 0) break;
    }

    return -1;
}

int clear_pwd(t_pwd_flags *flags) {
    char *res = NULL;

    if (flags->is_l_flag) {
        res = mx_strdup(t_dirs_to_work.PWD);
    }
    else {
        getcwd(res, INT_MAX);
        if(res == NULL) {
            char* buff = NULL;
            // readlink(getenv("PWD"), buff, PATH_MAX);
            buff = realpath(getenv("PWD"), NULL);
            if (buff != NULL && buff[0] == '\0')
                res = mx_strndup(getenv("PWD"), 1 + get_last_char_index(getenv("PWD"), '/'));
            else
                res = mx_strjoin(res, buff);
            free(buff);
        }
    }
    
    mx_printstr(res);
    mx_printchar('\n');
    
    free(res);
    return 0;
}


void print_pwd_error(char wrong_flag) {
    char *err = mx_strnew(1);
    err[0] = wrong_flag;
    mx_printerr("ush: pwd: -");
    mx_printerr(err);
    mx_printerr(": invalid option\n");
    mx_printerr("pwd: usage: pwd [-LP]\n");
    free(err);
}

int find_pwd_flags(t_pwd_flags *pwd_flag, char **args) {
    if (args[1] != NULL) {
        if (args[1][0] == '-') {
            for (int i = 1; i < mx_strlen(args[1]); i++) {
                if(args[1][i] == 'P') {
                    pwd_flag->is_p_flag = true;
                    pwd_flag->is_l_flag = false;
                } else if(args[1][i] == 'L') {
                    pwd_flag->is_l_flag = true;
                    pwd_flag->is_p_flag = false;
                } else {
                    print_pwd_error(args[1][i]);
                    return 1;
                }
            }
        }
    }
    return 0;
}

t_pwd_flags *create_pwd_flags(void) {
    t_pwd_flags *pwd_flag = (t_pwd_flags*)malloc(sizeof(t_pwd_flags));
    pwd_flag->is_l_flag = true;
    pwd_flag->is_p_flag = false;
    return pwd_flag;
}
