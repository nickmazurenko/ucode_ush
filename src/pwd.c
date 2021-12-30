#include "pwd.h"

t_pwd_flag *create_pwd_flag() {
    t_pwd_flag *flag = (t_pwd_flag*)malloc(sizeof(t_pwd_flag));
    flag->l_flag = false;
    flag->no_flag = false;
    flag->p_flag = false;
    flag->wrong_flag = true;

    return flag;
}

void bad_option_error(char *option) {
   char *error = mx_strdup("pwd: bad option: ");
   char *wrong_option = mx_strndup(option, 2);
   error = mx_strjoin(error, wrong_option);
   mx_printerr(error);
   free(error);
   free(wrong_option);
}

int find_pwd_flags(t_pwd_flag *flag, char **argv, int argc) {
    if(argv[0] == NULL) {
        flag->no_flag = true;
        flag->wrong_flag = false;
    } else {
        for(int argv_index = 0; argv_index < argc - 1; argv_index++){
            if(!mx_strcmp(argv[argv_index], "-L")) {
                flag->l_flag = true;
                flag->wrong_flag = false;
            }
            else if(!mx_strcmp(argv[argv_index], "-P")) {
                flag->p_flag = true;
                flag->wrong_flag = false;
            }
            else if(!mx_strcmp(argv[argv_index], "-LP") || !mx_strcmp(argv[argv_index], "-PL")) {
                flag->p_flag = true;
                flag->l_flag = true;
                flag->wrong_flag = false;
            } 
            else {
                flag->wrong_flag = true;
            }
            if(flag->wrong_flag) {
                bad_option_error(argv[argv_index]);
                return 1;
            }
        }
    }
    return 0;
}

char *get_pwd() {
    char *result_path = NULL;
    char *path = NULL;
    char *link = realpath(getenv("PWD"), NULL);
    
    path = getcwd(NULL, 256);

    if(link && !mx_strcmp(path, link)) {
        result_path = mx_strdup(getenv("PWD"));
        free(link);
        free(path);
    } else {
        result_path = mx_strdup(path);
        free(path);
    }

    return result_path;
}

char *get_symbolic_pwd() {
    char *link = realpath(getenv("PWD"), NULL);
    return link;
}




int clear_pwd(t_pwd_flag *flag) {
    char *pwd_string = NULL;
    if(flag->l_flag || flag->no_flag) {
        pwd_string = get_pwd();
    }

    if(flag->p_flag) {
        pwd_string = get_symbolic_pwd();
    }

    if(pwd_string == NULL) {
        return 1;
    } else {
        mx_printstr(pwd_string);
        mx_printstr("\n");
    }

    free(pwd_string);
    free(flag);
    return 0;
}

