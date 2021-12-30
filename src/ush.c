#include "ush.h"

int pwd(t_expression *expression) {
    errno = 0;
    t_pwd_flag *pwd_flag = create_pwd_flag();
    find_pwd_flags(pwd_flag, expression->argv, expression->argc);
    if(errno) {
        mx_printerr(strerror(errno));
    }
    int error_idx = clear_pwd(pwd_flag);
    return error_idx;
}

int export(t_expression* expression) {
    expression = NULL;
    clear_export();
    return 0;
}
