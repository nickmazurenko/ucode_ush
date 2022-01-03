#include "../inc/ush.h"

int mx_builtin_pwd(t_flags_pwd *flags) {
    char *res = (char *)malloc(INT_MAX);

    if (flags->using_L) {
        res = mx_strdup(t_global.PWD);
    }
    else {
        getcwd(res, INT_MAX);
    }
    
    mx_printstr(res);
    mx_printchar('\n');
    
    free(res);
    return 0;
}
