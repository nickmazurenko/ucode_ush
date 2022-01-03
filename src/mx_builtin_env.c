#include "../inc/ush.h"

int mx_builtin_env(t_flags_env *flags, char **data) {
    mx_apply_escapes(&data);
    if (data[1] == NULL) { //print all variables
        int i = 1;
        char *s = *environ;
        for (; s; i++) {
            mx_printstr(s);
            mx_printchar('\n');
            s = *(environ + i);
        }
        return 0;
    }
    else {
        if (flags->using_I) {
            int child_pid = fork();
            if (child_pid == 0) {
                
                environ = NULL;
                
                if (data[2] != NULL) {
                    char **path_dir = mx_strsplit(t_global.PATH, ':');

                    for (int i = 0; path_dir[i] != NULL; i++) {
                        int exec_status = 0;
                        char *cmd = NULL;
                        if (data[2][0] != '/') {
                            cmd = mx_strjoin(path_dir[i], "/");
                            cmd = mx_strjoin(cmd, data[2]);
                        }
                        else 
                            cmd = mx_strdup(data[2]);
                        exec_status = execve(cmd, data + 2, environ); //execute command
                        free(cmd);
                        if (exec_status != -1) {
                            mx_del_strarr(&path_dir);
                            exit(0);
                        }
                    }

                    mx_printerr("ush: command not found: ");
                    mx_printerr(data[2]);
                    mx_printerr("\n");
                    mx_del_strarr(&path_dir);
                    exit(1);
                }
                exit(0);
            }
            else {
                t_jobs *new_process = jobs_new_node(child_pid, data[3]);
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

            return 0;
        }
        else if (flags->using_U) {
            if (data[2] != NULL) {
                char *env_var = getenv(data[2]);
                if (env_var != NULL)
                    unsetenv(data[2]);
                else {
                    mx_printerr("Set the environmental variable ");
                    mx_printerr(data[2]);
                    mx_printerr("\n");
                    return 1;
                }
                if (data[3] == NULL) {
                    int i = 1;
                    char *s = *environ;
                    for (; s; i++) {
                        mx_printstr(s);
                        mx_printchar('\n');
                        s = *(environ + i);
                    }
                    t_global.exit_status = 0;
                    return 0;
                }
                mx_create_process(data[3], data + 3, data[3]);
                return 0;
            }
        }
        else if (flags->using_P) {
            if (data[2] != NULL) {
                if (data[3] == NULL) {
                    int i = 1;
                    char *s = *environ;
                    for (; s; i++) {
                        mx_printstr(s);
                        mx_printchar('\n');
                        s = *(environ + i);
                    }
                    t_global.exit_status = 0;
                    return 0;
                }
                int child_pid = fork();
                if (child_pid == 0) {

                    int exec_status = 0;
                    char *cmd = mx_strnew(PATH_MAX);
                    cmd = mx_strcpy(cmd, data[2]);
                    cmd = mx_strcat(cmd, data[3]);
                    exec_status = execve(cmd, data + 3, environ); //execute command
                    free(cmd);
                    if (exec_status != -1)
                        exit(0);

                    mx_printerr("ush: ");
                    mx_printerr(data[3]);
                    mx_printerr(": No such file or directory\n");
                    exit(1);
                }
                else {
                    t_jobs *new_process = jobs_new_node(child_pid, data[3]);
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

        }
        else {
            if (data[1] != NULL) {
                mx_create_process(data[1], data + 1, data[1]);
            }
        }
    }
    return 0;
}
