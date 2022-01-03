#include "../inc/ush.h"

void mx_create_process(char *command, char **parameters, char *line) {
    int child_pid = fork();
    if (child_pid == 0) {
        if (getenv("PATH") == NULL) {
            mx_printerr("ush: command not found: ");
            mx_printerr(command);
            mx_printerr("\n");
            exit(1);
        }
        char **path_dir = mx_strsplit(getenv("PATH"), ':');

        for (int i = 0; path_dir[i] != NULL; i++) {
            int exec_status = 0;
            char *cmd = NULL;
            if (command[0] != '/') {
                cmd = mx_strjoin(path_dir[i], "/");
                cmd = mx_strjoin(cmd, command);
            }
            else 
                cmd = mx_strdup(command);
            exec_status = execve(cmd, parameters, environ); //execute command
            free(cmd);
            if (exec_status != -1) {
                mx_del_strarr(&path_dir);
                exit(0);
            }
        }

        mx_printerr("ush: command not found: ");
        mx_printerr(command);
        mx_printerr("\n");
        mx_del_strarr(&path_dir);
        exit(1);
    }
    else {
        t_jobs *new_process = jobs_new_node(child_pid, line);
        jobs_push_back(&jobs, &new_process);
        int child_status = 0;
        waitpid(child_pid, &child_status, WUNTRACED);
        t_global.exit_status = WEXITSTATUS(child_status);
        if (!WIFSTOPPED(child_status))
            jobs_remove(&jobs, child_pid);
        else
            t_global.exit_status = 147;
        
        if (WIFSIGNALED(child_status))
            t_global.exit_status = 130;
    }
}
