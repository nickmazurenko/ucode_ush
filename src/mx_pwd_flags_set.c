#include "../inc/ush.h"

int mx_pwd_flags_set(t_flags_pwd *data, char **flags) {
    if (flags[1] != NULL) {
        if (flags[1][0] == '-') {
            for (int i = 1; i < mx_strlen(flags[1]); i++) {

                switch (flags[1][i])
                {
                case 'P':
                    data->using_P = true;
                    data->using_L = false;
                    break;
                case 'L':
                    data->using_L = true;
                    data->using_P = false;
                    break;
                default:
                    mx_printerr("ush: pwd: -");
                    char c = flags[1][i];
                    mx_printerr(&c);
                    mx_printerr(": invalid option\n");
                    mx_printerr("pwd: usage: pwd [-LP]\n");
                    return 1;
                    break;
                }

            }
        }
    }
    return 0;
}
