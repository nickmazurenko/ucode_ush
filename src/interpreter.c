#include "interpreter.h"

char** get_lines(char* str) {
    char** lines = mx_strsplit(str, ';');

    for (int line_index = 0; lines[line_index] != NULL; line_index++) {
        char* current_line = lines[line_index];
        lines[line_index] = mx_strtrim(current_line);
        free(current_line);
    }
    return lines;
}

char** get_tokens(char* line) {
    char **tokens = mx_strsplit(line, ' ');
    for (int token_index = 0; tokens[token_index] != NULL; token_index++) {
        char* current_token = tokens[token_index];
        tokens[token_index] = mx_strtrim(current_token);
        free(current_token);
    }
    return tokens;
}

void execute_command(t_commands_dictionary_node* commands_dict, t_expression* expression) {

    command_func func = get_func_with_name(commands_dict, expression->command);
    if (func) func(expression);
    else {
        // check in system
        // errors
        // printf("non buildin\n");
    }
}

void interpret(char *str) {

    t_commands_dictionary_node* commands_dictionary = create_commands_dictionary_list();

    // TODO: emty string?
    char** lines = get_lines(str);

    t_expression_node* expressions = NULL;

    for (int line_index = 0; lines[line_index] != NULL; line_index++) {
        char** tokens = get_tokens(lines[line_index]);
        int argc = 0;
        for (; tokens[argc] != NULL; argc++) {}
        if (argc != 0) {
            push_expression_list(&expressions, tokens[0], tokens + 1, argc);
        }
        mx_del_strarr(&tokens);
    }

    t_expression_node* current_expression = expressions;

    while (current_expression)
    {
        execute_command(commands_dictionary, current_expression->expression);
        current_expression = current_expression->next;
    }

}
