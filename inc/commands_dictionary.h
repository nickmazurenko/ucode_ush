#pragma once

#include "libmx.h"
#include "ush.h"

typedef int(*command_func)(t_expression*);

typedef struct s_commands_dictionary_node {

    char* command_str;
    int(*command_func)(t_expression*);
    struct s_commands_dictionary_node* next;

}              t_commands_dictionary_node;

t_commands_dictionary_node* create_commands_dictionary_node(char* command, int (*command_func)(t_expression*));
t_commands_dictionary_node* push_commands_dictionary(t_commands_dictionary_node** head, char* command, int(*command_func)(t_expression*));
void free_commands_dictionary_list(t_commands_dictionary_node** head);
command_func get_func_with_name(t_commands_dictionary_node* dictionary, char* command_str);
t_commands_dictionary_node* create_commands_dictionary_list();
