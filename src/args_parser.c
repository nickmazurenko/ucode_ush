#include "args_parser.h"

char** create_argv(t_string_node* args_list) {
    size_t argc = length_string_list(args_list);

    char** argv = (char**)malloc(sizeof(char *) * argc);

    t_string_node* arg_node = args_list;

    size_t argv_index = 0;
    while (arg_node != NULL)
    {
        argv[argv_index] = mx_strdup(arg_node->str);
        arg_node = arg_node->next;
        argv_index++;
    }
    
    return argv;
}


// TODO: what about "word'""word"
char **parse_input_to_args(char *input, int* argc) {
    
    int input_length = mx_strlen(input);

    bool in_quote = false;
    bool in_dq    = false;


    int arg_start = 0;
    int arg_end   = input_length; 

    t_string_node* args_list = NULL;

    char *arg = mx_strnew(input_length);

    for (int input_index = 0; input_index < input_length + 1; input_index++) {

        // если разделитель кавычки забираем их 
        if (input[input_index] == '"' && !in_quote) {
            if (input_index != 0) {
                if (input[input_index - 1] == '\\') {
                    continue;
                }
            }
            if (in_dq) {
                arg_end = input_index;
                mx_strncpy(arg, input + arg_start + 1, arg_end - arg_start - 1);
                push_string_list(&args_list, arg);
                arg_start = arg_end + 1;
            }
            in_dq = !in_dq;
        }

        if (input[input_index] == '\'' && !in_dq) {
            if (input_index != 0) {
                if (input[input_index - 1] == '\\') {
                    continue;
                }
            }
            if (in_quote) {
                arg_end = input_index;
                mx_strncpy(arg, input + arg_start + 1, arg_end - arg_start - 1);
                push_string_list(&args_list, arg);
                arg_start = arg_end + 1;
            }
            in_quote = !in_quote;
        }

        // если разделитель пробел не забираем его
        if (input[input_index] == ' ') {
            if (in_dq || in_quote || (input_index != 0 && input[input_index - 1] == '\\')) {}
            else {
                arg_end = input_index;
                mx_strncpy(arg, input + arg_start, arg_end - arg_start);
                push_string_list(&args_list, arg);
                arg_start = arg_end + 1;
            }
        }

        if (input[input_index] == '\0') {
            if (arg_start != input_length) {
                arg_end = input_index;
                mx_strncpy(arg, input + arg_start, arg_end - arg_start);
                push_string_list(&args_list, arg);
                arg_start = arg_end + 1;
            }
        }

        mx_memset((void*)arg, '\0', input_length);
    }

    *argc = length_string_list(args_list);
    // char** not_screened_argv = create_argv(args_list);
    // char** argv = screen_argv(not_screened_argv, *argc);
    char** argv = create_argv(args_list);


    // for (int argv_index = 0; argv_index < *argc; argv_index++) {
    //     free(not_screened_argv[argv_index]);
    // }
    // free(not_screened_argv);

    free(arg);
    delete_string_list(&args_list);
    

    return argv;
}
