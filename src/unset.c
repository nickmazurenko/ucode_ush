#include "ush.h"

int mx_unset_check_param(char **data);

int clear_unset(const char *name) {
    return unsetenv(name);
}


int unset_check_args(char **args) {
    for (int i = 1; args[i] != NULL; i++) {

        for (int j = 0; j < mx_strlen(args[i]); j++) {

            if (args[i][j] == '=' || args[i][j] == '-'
                || args[i][j] == '.' || args[i][j] == ','
                || mx_isspace(args[i][j]) || args[i][j] == '('
                || args[i][j] == ')' || args[i][j] == '*'
                || args[i][j] == '^' || args[i][j] == '%'
                || args[i][j] == '#' || args[i][j] == '@'
                || args[i][j] == '!' || args[i][j] == '+'
                || args[i][j] == ']') {//not all list
                mx_printerr("ush: unset: '");
                mx_printerr(args[i]);
                mx_printerr("': not a valid identifier\n");
                return -1;
            }
        }
    }
    return 0;
}