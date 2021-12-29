#pragma once

typedef struct s_expression {
    char* command;
    int argc;
    char** argv;
}              t_expression;

t_expression* create_expression(char* command, char** argv, int argc);
void free_expression(t_expression** expression);

typedef struct s_expression_node {

    t_expression* expression;
    struct s_expression_node* next;

}              t_expression_node;

t_expression_node* create_expression_node(char* command, char** argv, int argc);
t_expression_node* push_expression_list(t_expression_node** head, char* command, char** argv, int argc);
void free_epression_list(t_expression_node** head);
