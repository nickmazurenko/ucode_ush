#include "console_input_handler.h"

void handle_ctrl_c(int sig) {
    t_jobs *node = jobs;
    while (node->next != NULL)
        node = node->next;
    int pid = node->pid;

    printf("\n");

    if (pid == jobs->pid) {
        free_dirs();
        exit(130);
    }
    else kill(pid, sig);
}

void handle_ctrl_z(int sig) {
    t_jobs *node = jobs;
    int job_id = 0;
    while (node->next != NULL) {
        if (node->job_id > job_id)
            job_id = node->job_id;
        node = node->next;
    }
    int pid = node->pid;

    if (pid == jobs->pid)
        return;

    kill(pid, sig);
    job_id++;
    node->job_id = job_id;
    printf("[%d]  Stopped\t\t\t%s\n", job_id, node->cmd);
    return;
}

