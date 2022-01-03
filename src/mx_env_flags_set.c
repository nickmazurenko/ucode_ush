#include "../inc/ush.h"

int mx_env_flags_set(t_flags_env *data, char **param) {
    /*
    for (int i = 1; param[i] != NULL; i++) {
        if (param[i][0] == '-') {
        
            for (int j = 1; j < mx_strlen(param[i]); j++) {
                if (param[i][j] != 'i' && param[i][j] != 'u'
                && param[i][j] != 'P') {
                    mx_printerr("env: invalid option -- '");
                    mx_printerr(&param[i][j]);
                    mx_printerr("'\nTry 'env --help' for more information.\n");
                    return 1;
                }
                
                if (param[i][j] == 'i') data->using_I = true;
                if (param[i][j] == 'u') data->using_U = true;
                if (param[i][j] == 'P') data->using_P = true;
            }
            
        }
    }
    */
   if (param[1] != NULL) {
        if (param[1][0] == '-') {
            for (int i = 1; i < mx_strlen(param[1]); i++) {

                switch (param[1][i])
                {
                case 'P':
                    data->using_P = true;
                    data->using_I = false;
                    data->using_U = false;
                    break;
                case 'i':
                    data->using_I = true;
                    data->using_U = false;
                    data->using_P = false;
                    break;
                case 'u':
                    data->using_I = false;
                    data->using_U = true;
                    data->using_P = false; 
                    break;
                default:
                    mx_printerr("env: invalid option -- '");
                    char c = param[1][i];
                    mx_printerr(&c);
                    mx_printerr("'\nTry 'env --help' for more information.\n");
                    return 1;
                    break;
                }

            }
        }
    }
    return 0;
}
