#include "expression.h"

t_expression* create_expression(char* command, char** argv, int argc) {
    t_expression* expression = (t_expression*)malloc(sizeof(t_expression));
    expression->command = mx_strdup(command);

    expression->argc = argc;
    expression->argv = (char**)malloc(sizeof(char*));

    for (int i = 0; i < argc; i++) {
        expression->argv[i] = mx_strdup(argv[i]);
    }

    return expression;
}

void free_expression(t_expression** expression) {
    t_expression* exp = *expression;
    mx_strdel(&(exp->command));
    mx_del_strarr(&(exp->argv));
    free(exp);
    *expression = NULL;
}

t_expression_node* create_expression_node(char* command, char** argv, int argc) {
    t_expression_node* node = (t_expression_node*)malloc(sizeof(t_expression_node));

    node->expression = create_expression(command, argv, argc);
    node->next = NULL;

    return node;
}

t_expression_node* push_expression_list(t_expression_node** head, char* command, char** argv, int argc) {
    if (*head == NULL) {
        *head = create_expression_node(command, argv, argc);
        return *head;
    } else {
        t_expression_node* current = *head;
        while (current->next != NULL)
        {
            current = current->next;
        }
        current->next = create_expression_node(command, argv, argc);
        return current->next;
    }
}

void free_epression_list(t_expression_node** head) {
    t_expression_node* current = *head;
    t_expression_node* next = NULL;

    while (current)
    {
        next = current->next;
        free_expression(&(current->expression));
        free(current);
        current = next;
    }

}
