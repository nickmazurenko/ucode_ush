#include "ush.h"

void create_process(char *command, char **parameters, char *line) {
    int child_process_id = fork();
    if (child_process_id == 0) {
        if (getenv("PATH") == NULL) {
            mx_printerr("ush: command not found: ");
            mx_printerr(command);
            mx_printerr("\n");
            exit(1);
        }
        char **dir_paths = mx_strsplit(getenv("PATH"), ':');

        for (int dir_path_index = 0; dir_paths[dir_path_index] != NULL; dir_path_index++) {
            int exec_status = 0;
            char *command_full_path = NULL;
            if (command[0] != '/') {
                command_full_path = mx_strjoin(dir_paths[dir_path_index], "/");
                command_full_path = mx_strjoin(command_full_path, command);
            }
            else
                command_full_path = mx_strdup(command);
            exec_status = execve(command_full_path, parameters, environ); //execute command
            free(command_full_path);
            if (exec_status != -1) {
                mx_del_strarr(&dir_paths);
                exit(0);
            }
        }

        mx_printerr("ush: command not found: ");
        mx_printerr(command);
        mx_printerr("\n");
        mx_del_strarr(&dir_paths);
        exit(1);
    }
    else {
        t_jobs *new_process = jobs_new_node(child_process_id, line);
        jobs_push_back(&jobs, &new_process);
        int child_status = 0;
        waitpid(child_process_id, &child_status, WUNTRACED);
        t_dirs_to_work.exit_status = WEXITSTATUS(child_status);
        if (!WIFSTOPPED(child_status))
            jobs_remove(&jobs, child_process_id);
        else
            t_dirs_to_work.exit_status = 147;
        
        if (WIFSIGNALED(child_status))
            t_dirs_to_work.exit_status = 130;
    }
}
