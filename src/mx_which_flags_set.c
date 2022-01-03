#include "../inc/ush.h"

int mx_which_flags_set(t_flags_which *data, char **flags) {
    if (flags[1] != NULL) {
        if (flags[1][0] == '-') {
            for (int i = 1; i < mx_strlen(flags[1]); i++) {

                switch (flags[1][i])
                {
                case 'a':
                    data->using_A = true;
                    break;
                case 's':
                    data->using_S = true;
                    break;
                default:
                    mx_printerr("which: bad option: -");
                    char c = flags[1][i];
                    mx_printerr(&c);
                    mx_printerr("\n");
                    return 1;
                    break;
                }

            }
        }
    }
    return 0;
}
