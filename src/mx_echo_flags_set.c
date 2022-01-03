#include "../inc/ush.h"

int mx_echo_flags_set(t_flags_echo *data, char **flags) {
    if (flags[1] != NULL) {
        if (flags[1][0] == '-') {
            for (int i = 1; i < mx_strlen(flags[1]); i++) {

                switch (flags[1][i])
                {
                case 'n':
                    data->using_N = true;
                    break;
                case 'e':
                    data->using_e = true;
                    data->using_E = false;
                    break;
                case 'E':
                    data->using_E = true;
                    data->using_e = false;
                    break;
                default:
                    return 1;
                    break;
                }

            }
        }
    }
    return 0;
}
