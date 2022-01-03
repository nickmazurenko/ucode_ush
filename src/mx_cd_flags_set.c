#include "../inc/ush.h"

int mx_cd_flags_set(t_flags_cd *data, char **flags) {
    if (flags[1] != NULL) {
        if (flags[1][0] == '-') {
            for (int i = 1; i < mx_strlen(flags[1]); i++) {

                switch (flags[1][i])
                {
                case 'P':
                    data->using_P = true;
                    break;
                case 's':
                    data->using_s = true;
                    break;
                default:
                    mx_printerr("ush: cd: -");
                    char c = flags[1][i];
                    mx_printerr(&c);
                    mx_printerr(": invalid option\n");
                    mx_printerr("cd: usage: cd [-sP]\n");
                    return 1;
                    break;
                }

            }
        }
    }
    return 0;
}
