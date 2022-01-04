#include "../inc/ush.h"
t_pwd_flags *create_pwd_flags(void);
void print_pwd_error(char wrong_flag);
int find_pwd_flags(t_pwd_flags *pwd_flag, char **flags);



int clear_pwd(t_pwd_flags *flags) {
    char *res = (char *)malloc(INT_MAX);

    if (flags->is_l_flag) {
        res = mx_strdup(t_dirs_to_work.PWD);
    }
    else {
        getcwd(res, INT_MAX);
    }
    
    mx_printstr(res);
    mx_printchar('\n');
    
    free(res);
    return 0;
}


void print_pwd_error(char wrong_flag) {
    mx_printerr("ush: pwd: -");
    mx_printerr(&wrong_flag);
    mx_printerr(": invalid option\n");
    mx_printerr("pwd: usage: pwd [-LP]\n");
}

int find_pwd_flags(t_pwd_flags *pwd_flag, char **flags) {
    if (flags[1] != NULL) {
        if (flags[1][0] == '-') {
            for (int i = 1; i < mx_strlen(flags[1]); i++) {

                switch (flags[1][i])
                {
                    case 'P':
                        pwd_flag->is_p_flag = true;
                        pwd_flag->is_l_flag = false;
                        break;
                    case 'L':
                        pwd_flag->is_l_flag = true;
                        pwd_flag->is_p_flag = false;
                        break;
                    default:
                        print_pwd_error(flags[1][i]);
                        return 1;
                        break;
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