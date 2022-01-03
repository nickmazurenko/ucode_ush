
#include "../inc/ush.h"

int mx_unset_check_param(char **data) {
    for (int i = 1; data[i] != NULL; i++) {
    
        for (int j = 0; j < mx_strlen(data[i]); j++) {
        
            if (data[i][j] == '=' || data[i][j] == '-'
            || data[i][j] == '.' || data[i][j] == ','
            || mx_isspace(data[i][j]) || data[i][j] == '('
            || data[i][j] == ')' || data[i][j] == '*'
            || data[i][j] == '^' || data[i][j] == '%'
            || data[i][j] == '#' || data[i][j] == '@'
            || data[i][j] == '!' || data[i][j] == '+'
            || data[i][j] == ']') {//not all list
                mx_printerr("ush: unset: '");
                mx_printerr(data[i]);
                mx_printerr("': not a valid identifier\n");
                return -1;
            }
            
        }
        
    }
    return 0;
}
