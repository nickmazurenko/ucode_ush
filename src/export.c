#include "export.h"

int clear_export(char* variable_value) {
    
    int error_status = 0;

    if (variable_value == NULL) {
        extern char **environ;
        for (int var_index = 0; environ[var_index] != NULL; var_index++){
            mx_printstr(environ[var_index]);
            mx_printchar('\n');
        }
        return error_status;
    }

    char** tokens = mx_strsplit(variable_value, '=');

    if (tokens[1] == NULL) {
        if (getenv(tokens[0]) == NULL) {
            setenv(tokens[0], "", 1);
        }
    } else {
        setenv(tokens[0], tokens[1], 1);
    }

    return 0;
}
