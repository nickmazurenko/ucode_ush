#include "utils.h"

t_string_node* create_string_node(char* str) {
    t_string_node* node = (t_string_node*)malloc(sizeof(t_string_node));
    node->str = mx_strdup(str);
    node->next = NULL;
    return node;
}

t_string_node* push_string_list(t_string_node** head, char* str) {
    if (*head == NULL) {
        *head = create_string_node(str);
        return *head;
    }
    else {
        t_string_node* current = *head;
        while (current->next != NULL)
        {
            current = current->next;
        }
        current->next = create_string_node(str);
        return current;
    }
    return NULL;
}

t_string_node* get_options(char** argv, int* options_number) {

    *options_number = 0;
    t_string_node* options = NULL;

    for (int arg_index = 0; argv[arg_index] != NULL; arg_index++) {
        char* arg = argv[arg_index];
        int arg_length = mx_strlen(arg);
        if (arg_length >= 2) {
            if (arg[0] == '-') {
                push_string_list(&options, arg + 1);
                (*options_number)++;
            }
        }
    }
    return options;
}
