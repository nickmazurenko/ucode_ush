#include "../inc/ush.h"

t_jobs *jobs_new_node(int pid, char *cmd) {
    t_jobs *res = (t_jobs *)malloc(sizeof(t_jobs));
    res->pid = pid;
    res->job_id = 0;
    res->cmd = mx_strdup(cmd);
    res->next = NULL;
    res->prev = NULL;
    return res;
}

void jobs_clear(t_jobs **head) {
    t_jobs *ptr = *head;
    while (ptr != NULL) {
        t_jobs *tmp = ptr->next;
        if (ptr->pid != getpid())
            kill(ptr->pid, SIGINT);
        free(ptr->cmd);
        free(ptr);
        ptr = tmp;
    }
    *head = NULL;
}

void jobs_push_back(t_jobs **list, t_jobs **data) {
    t_jobs *tmp = NULL;
    if (list == NULL || *list == NULL) {
        *list = *data;
        (*list)->next = NULL;
        (*list)->prev = NULL;
    }
    else {
        tmp = *list;
        while (tmp->next != NULL)
            tmp = tmp->next;
        (*data)->prev = tmp;
        tmp->next = *data;
        tmp->next->prev = tmp;
        tmp->next->next = NULL;
        (*list)->prev = tmp->next;
    }
}

int jobs_remove(t_jobs **head, int pid) {
    t_jobs *node = *head;
    while (node->next->pid != pid && node->next != NULL)
        node = node->next;
    t_jobs *tmp = NULL;
    if (node->next != NULL)
        tmp = node->next->next;
    free(node->next->cmd);
    free(node->next);
    node->next = tmp;
    if (tmp != NULL)
        node->next->prev = node;
    return 0;
}
