#include "loop.h"
#include "utils.h"
#include "args_parser.h"

char **str_array_to_null_end(char** array, int array_size) {
    char** null_end_array = (char**)malloc(sizeof(char*) * (array_size + 1));
    for (int array_index = 0; array_index < array_size; array_index++) {
        null_end_array[array_index] = mx_strdup(array[array_index]);
        if (array[array_index] != NULL) free(array[array_index]);
    }
    null_end_array[array_size] = NULL;
    free(array);
    return null_end_array;
}

int execute_commands(char** commands_arr) {

    char **parameters = NULL;
    char * command = NULL;

    // for (int command_index = 0; commands_arr[command_index] != NULL; command_index++) {
    // }

    for (int i = 0; commands_arr[i] != NULL; ++i) {
        if (mx_command_substitution(&commands_arr[i]) == -1) // TODO: как работает замена?
            continue;
        // parameters = mx_strsplit(commands_arr[i], ' ');
        int argc = 0;
        char**buff = parse_input_to_args(commands_arr[i], &argc);
        parameters = str_array_to_null_end(buff, argc);

        for (int parameter_index = 0; parameters[parameter_index] != NULL; parameter_index++) {
            delete_one_streak_back_slash(&parameters[parameter_index]);
        }

        command = mx_strdup(parameters[0]);
        
        // mx_printstr("parameters: \n");
        // mx_print_strarr(commands_arr, "\n");
        // mx_printstr("parameters end\n");

        if (setup_builtin(command, parameters, &commands_arr, i)) {
            handle_escape_symbols(&parameters);
            create_process(command, parameters, commands_arr[i]);
        }

        free(command);
        mx_del_strarr(&parameters);

    }
    return 0;
}

int loop() {

    char* line = NULL;

    while (true) {
        signal(SIGINT, handle_ctrl_c);
        signal(SIGTSTP, handle_ctrl_z);

        if (isatty(0))
            mx_print_prompt();
        read_command(&line);

        char **commands_arr = mx_strsplit(line, ';');

        // TODO: ush>+a -> command not found: a
//        for (int i = 0; commands_arr[i] != NULL; i++) {
//            char *command = mx_strdup(commands_arr[i]);
//            char *tmp_command = command;
//            while (*tmp_command != '\0' && !mx_isalpha(*tmp_command)) {
//                *tmp_command = '\0';
//                for (; *(tmp_command + 1) != '\0';) {
//                    mx_swap_char(tmp_command, tmp_command + 1);
//                    tmp_command++;
//                }
//                tmp_command = command;
//            }
//
//            if (command[0] != '\0') {
//                free(commands_arr[i]);
//                commands_arr[i] = command;
//            } else {
//                free(command);
//            }
//
//        }
        free(line);
        line = NULL;

        execute_commands(commands_arr);

        mx_del_strarr(&commands_arr);
        if (!isatty(fileno(stdin)))
            break;
    }

    return 0;
}
