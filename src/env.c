#include "ush.h"
#include "env.h"

t_env_flags *create_env_flags(void);
void print_env_error(char wrong_flag);
int find_env_flags(t_env_flags *env_flag, char **args);
void wrong_command_error(char *command);
void set_var_error(char *arg);
void no_fd_error(char *fd);

int clear_env(t_env_flags *env_flag, char **args) {
    handle_escape_symbols(&args);

    if (args[1] == NULL) { //print all variables
        int i = 1;
        char *s = *environ;
        while(s) {
            mx_printstr(s);
            mx_printchar('\n');
            s = *(environ + i);
            i++;
        }
        return 0;
    } else {
        if (env_flag->is_i_flag) {
            int pid = fork();
            if (pid == 0) {
                environ = NULL;
                
                if (args[2] != NULL) {
                    char **path_to_dir = mx_strsplit(t_dirs_to_work.PATH, ':');

                    for (int i = 0; path_to_dir[i] != NULL; i++) {
                        int status = 0;
                        char *comm_line = NULL;
                        if (args[2][0] != '/') {
                            comm_line = mx_strjoin(path_to_dir[i], "/");
                            comm_line = mx_strjoin(comm_line, args[2]);
                        } else {
                            comm_line = mx_strdup(args[2]);
                        }

                        status = execve(comm_line, args + 2, environ); //execute command
                        free(comm_line);

                        if (status != -1) {
                            mx_del_strarr(&path_to_dir);
                            exit(0);
                        }
                    }

                    wrong_command_error(args[2]);
                    mx_del_strarr(&path_to_dir);
                    exit(1);
                }
                exit(0);
            }
            else {
                t_jobs *new_command = jobs_new_node(pid, args[3]);
                jobs_push_back(&jobs, &new_command);
                int status = 0;
                waitpid(pid, &status, WUNTRACED);
                t_dirs_to_work.exit_status = WEXITSTATUS(status);

                if (!WIFSTOPPED(status)) {
                    jobs_remove(&jobs, pid);
                } else {
                    t_dirs_to_work.exit_status = 147;
                }

                if (WIFSIGNALED(status)) {
                    t_dirs_to_work.exit_status = 130;
                }
            }

            return 0;
        } else if (env_flag->is_u_flag) {

            if (args[2] != NULL) {

                char *env_var = getenv(args[2]);
                if (env_var != NULL) {
                    unsetenv(args[2]);
                } else {
                    set_var_error(args[2]);
                    return 1;
                }
                if (args[3] == NULL) {
                    int i = 1;
                    char *s = *environ;
                    for (; s; i++) {
                        mx_printstr(s);
                        mx_printchar('\n');
                        s = *(environ + i);
                    }
                    t_dirs_to_work.exit_status = 0;
                    return 0;
                }
                create_process(args[3], args + 3, args[3]);
                return 0;
            }
        } else if (env_flag->is_p_flag) {
            if (args[2] != NULL) {
                if (args[3] == NULL) {
                    int i = 1;
                    char *s = *environ;
                    for (; s; i++) {
                        mx_printstr(s);
                        mx_printchar('\n');
                        s = *(environ + i);
                    }
                    t_dirs_to_work.exit_status = 0;
                    return 0;
                }
                int child_pid = fork();
                if (child_pid == 0) {

                    int exec_status = 0;
                    char *cmd = mx_strnew(PATH_MAX);
                    cmd = mx_strcpy(cmd, args[2]);
                    cmd = mx_strcat(cmd, args[3]);
                    exec_status = execve(cmd, args + 3, environ); //execute command
                    free(cmd);
                    if (exec_status != -1)
                        exit(0);

                    no_fd_error(args[3]);
                    exit(1);
                } else {
                    t_jobs *new_process = jobs_new_node(child_pid, args[3]);
                    jobs_push_back(&jobs, &new_process);
                    int child_status = 0;
                    waitpid(child_pid, &child_status, WUNTRACED);
                    t_dirs_to_work.exit_status = WEXITSTATUS(child_status);
                    if (!WIFSTOPPED(child_status)) {
                        jobs_remove(&jobs, child_pid);
                    } else {
                        t_dirs_to_work.exit_status = 147;
                    }
                    if (WIFSIGNALED(child_status))
                        t_dirs_to_work.exit_status = 130;
                }
            }

        } else {
            if (args[1] != NULL) {
                create_process(args[1], args + 1, args[1]);
            }
        }
    }
    return 0;
}


t_env_flags *create_env_flags(void) {
    t_env_flags *env_flag = (t_env_flags *) malloc(sizeof(t_env_flags));
    env_flag->is_i_flag = false;
    env_flag->is_u_flag = false;
    env_flag->is_p_flag = false;
    return env_flag;
}

void no_fd_error(char *fd) {
    mx_printerr("ush: ");
    mx_printerr(fd);
    mx_printerr(": No such file or directory\n");
}
void set_var_error(char *arg) {
    mx_printerr("Set the environmental variable ");
    mx_printerr(arg);
    mx_printerr("\n");
}

void print_env_error(char wrong_flag) {
    char *err = mx_strnew(1);
    err[0] = wrong_flag;
    mx_printerr("env: invalid option -- '");
    mx_printerr(err);
    mx_printerr("'\nTry 'env --help' for more information.\n");
    free(err);
}

void wrong_command_error(char *command) {
    mx_printerr("ush: command not found: ");
    mx_printerr(command);
    mx_printerr("\n");
}

int find_env_flags(t_env_flags *env_flag, char **args) {
    if (args[1] != NULL) {
        if (args[1][0] == '-') {
            for (int i = 1; i < mx_strlen(args[1]); i++) {
                if (args[1][i] == 'P') {
                    env_flag->is_p_flag = true;
                    env_flag->is_i_flag = false;
                    env_flag->is_u_flag = false;
                } else if (args[1][i] == 'i') {
                    env_flag->is_i_flag = true;
                    env_flag->is_u_flag = false;
                    env_flag->is_p_flag = false;
                } else if (args[1][i] == 'u') {
                    env_flag->is_i_flag = false;
                    env_flag->is_u_flag = true;
                    env_flag->is_p_flag = false;
                } else {
                    print_env_error(args[1][i]);
                    return 1;
                }
            }
        }
    }
    return 0;
}