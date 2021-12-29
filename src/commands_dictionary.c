#include "commands_dictionary.h"

t_commands_dictionary_node* create_commands_dictionary_node(char* command, int(*command_func)(t_expression*)) {
    t_commands_dictionary_node* node = (t_commands_dictionary_node*)malloc(sizeof(t_commands_dictionary_node));
    node->command_str = mx_strdup(command);
    node->command_func = command_func;
    node->next = NULL;
    return node;
}

t_commands_dictionary_node* push_commands_dictionary(t_commands_dictionary_node** head, char* command, int(*command_func)(t_expression*)) {
    if (*head == NULL) {
        *head = create_commands_dictionary_node(command, command_func);
        return *head;
    } else {
        t_commands_dictionary_node* current = *head;
        while (current->next != NULL)
        {
            current = current->next;
        }
        current->next = create_commands_dictionary_node(command, command_func);
        return current->next;
    }
}

void free_commands_dictionary_list(t_commands_dictionary_node** head) {
    t_commands_dictionary_node* current = *head;
    t_commands_dictionary_node* next = NULL;

    while (current)
    {
        next = current->next;
        mx_strdel(&(current->command_str));
        free(current);
        current = next;
    }
    
    *head = NULL;
}

command_func get_func_with_name(t_commands_dictionary_node* dictionary, char* command_str) {
    while (dictionary != NULL)
    {
        if (mx_strcmp(dictionary->command_str, command_str) == 0) {
            break;
        } else {
            dictionary = dictionary->next;
        }
    }
    if (dictionary)
        return dictionary->command_func;
    return NULL;
}

t_commands_dictionary_node* create_commands_dictionary_list() {
    t_commands_dictionary_node* head = NULL;
    push_commands_dictionary(&head, "export", export);
    push_commands_dictionary(&head, "pwd", pwd);
    
    return head;
}
