#include "pwd.h"

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


int clear_pwd(char *flag) {
    char *pwd_string = NULL;
    
    if(IS_LPWD_FLAG(flag) /*&& no flags*/) {
        pwd_string = get_symbolic_pwd();
    }

    if(IS_PPWD_FLAG(flag)) {
        pwd_string = get_pwd();
    }

    if(pwd_string == NULL) {
        return 1;
    } else {
        mx_printstr(pwd_string);
    }

    free(pwd_string);
    return 0;
}
