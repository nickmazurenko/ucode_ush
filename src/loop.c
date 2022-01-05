#include "loop.h"

int execute_commands(char** commands_arr) {


    char **parameters = NULL;
    char * command = NULL;

    for (int i = 0; commands_arr[i] != NULL; ++i) {

        if (mx_command_substitution(&commands_arr[i]) == -1) // TODO: как работает замена?
            continue;

        parameters = mx_strsplit(commands_arr[i], ' ');
        command = mx_strdup(parameters[0]);

        if (mx_execute_builtin(command, parameters, &commands_arr, i)) {
            handle_escape_symbols(&parameters);
            mx_create_process(command, parameters, commands_arr[i]);
        }

        free(command);
        mx_del_strarr(&parameters);

    }
}

int loop() {

//    char *command = NULL, **parameters = NULL, *line = NULL;
    char* line = NULL;

    while (true) {
        signal(SIGINT, handle_ctrl_c);
        signal(SIGTSTP, handle_ctrl_z);

        if (isatty(0))
            mx_print_prompt();
        mx_read_command(&line);

        char **commands_arr = mx_strsplit(line, ';');
        for (int i = 0; commands_arr[i] != NULL; i++) {
            char *ptr = commands_arr[i];
            char *tmp_ptr = ptr;
            while (*tmp_ptr != '\0' && !mx_isalpha(*tmp_ptr)) {
                *tmp_ptr = '\0';
                for (; *(tmp_ptr + 1) != '\0';) {
                    mx_swap_char(tmp_ptr, tmp_ptr + 1);
                    tmp_ptr++;
                }
                tmp_ptr = ptr;
            }
        }
        free(line);
        line = NULL;

        execute_commands(commands_arr);

        mx_del_strarr(&commands_arr);
        if (!isatty(fileno(stdin)))
            break;
    }

    return 0;
}