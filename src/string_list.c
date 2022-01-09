#include "string_list.h"

t_string_node* new_string_node(char *str) {

    t_string_node* node = (t_string_node*)malloc(sizeof(t_string_node));

    node->next = NULL;
    node->str = mx_strdup(str);

    return node;
}

t_string_node* push_string_list(t_string_node** head, char *str) {

    t_string_node* current = *head;

    if (current == NULL) {
        *head = new_string_node(str);
        return *head;
    } else {
        while (current->next != NULL)
        {
            current = current->next;
        }
        current->next = new_string_node(str);
    }

    return current->next;
}

void delete_string_list(t_string_node** head) {

    if (*head == NULL) return;

    t_string_node* current = *head;
    t_string_node* next    = current->next;

    while (current)
    {
        next = current->next;
        mx_strdel(&(current->str));
        free(current);
        current = next;
    }
    
}

void to_string_string_list(t_string_node* head) {
    printf("\n\n");
    printf("String List: %p\n\n", (void*)head);
    int index = 0;
    while (head != NULL)
    {
        printf("\tnode [%i]: %p", index,(void*)head);
        printf("\tstr: %s", head->str);
        printf("\tnext: %p", (void*)head->next);
        printf("\n\n");
        index++;
        head = head->next;
    }
    printf("\n\n");
}

size_t length_string_list(t_string_node* head) {
    size_t length = 0;

    while (head != NULL)
    {
        length++;
        head = head->next;
    }
    
    return length;
}
