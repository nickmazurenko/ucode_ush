#ifndef USH_UTILS
#define USH_UTILS

#include "libmx.h"

typedef struct s_string_node {
    char* str;
    struct s_string_node* next;
}              t_string_node;

t_string_node* create_string_node(char* str);
t_string_node* push_string_list(t_string_node** head, char* str);

t_string_node* get_options(char** argv, int* length);

#endif
