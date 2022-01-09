#include "ush.h"
#include "utils.h"

void remove_double_quotes_from_parameters(char **parameters) {

    for (int parameter_index = 0; parameters[parameter_index] != NULL; parameter_index++) {
        int double_quotes_number = mx_count_substr_new(parameters[parameter_index], "\"");
        int screenable_double_quotes_number = mx_count_substr_new(parameters[parameter_index], "\\\"");
        if ((double_quotes_number - screenable_double_quotes_number) == 0) continue;

        int double_quot_index = -1;
        while ((double_quot_index = mx_get_char_index(parameters[parameter_index], '"')) != -1)
        {
            char *buff = parameters[parameter_index];
            parameters[parameter_index] = replace_substr_new(buff, "\"", "");
            // free(buff);
        }
    }
}

void create_process(char *command, char **parameters, char *line) {
    // for (int i = 0; parameters[i] != NULL; i++) {
    //     mx_printstr(parameters[i]);
    //     mx_printchar('\n');
    // }
    if (strstr(command, "echo")) {
        remove_double_quotes_from_parameters(parameters + 1);
    }
    // TODO remove double quotes from command
    int child_process_id = fork();
    if (child_process_id == 0) {
        if (getenv("PATH") == NULL) {
            mx_printerr("ush: command not found: ");
            delete_back_slashes(command);
            screen_back_slashes(&command);
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
        delete_back_slashes(command);
        screen_back_slashes(&command);
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
