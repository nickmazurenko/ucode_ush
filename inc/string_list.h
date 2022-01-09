#ifndef STRING_LIST_H
#define STRING_LIST_H

#include "libmx.h"
#include "stdio.h"

typedef struct s_string_node
{
    char *str;
    struct s_string_node* next;
}              t_string_node;

t_string_node* new_string_node(char *str);

t_string_node* push_string_list(t_string_node** head, char *str);
void           delete_string_list(t_string_node** head);

void to_string_string_list(t_string_node* head);

size_t length_string_list(t_string_node* head);

#endif
