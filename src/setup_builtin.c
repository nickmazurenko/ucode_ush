#include "ush.h"
#include "cd.h"
#include "env.h"
#include "which.h"
#include "echo.h"
#include "utils.h"


int setup_builtin(char *command, char **params, char ***commands_arr, int i) {
    // EXIT
    if (!mx_strcmp("exit", command)) {
        int ext_val = 0;
        if (params[1] != NULL) {
            ext_val = mx_atoi(params[1]);
        }
        free(command);
        mx_del_strarr(&params);
        mx_del_strarr(commands_arr);
        exit(ext_val);

    } else if (!mx_strcmp("unset", command)) {
        if (unset_check_args(params) == 0) {
            for (int i = 1; params[i] != NULL; i++) {
                if (clear_unset(params[i]) < 0) {
                    perror("ush: unset");
                    t_dirs_to_work.exit_status = 1;
                    continue;
                }
            }
        }
        return 0;

    } else if (!mx_strcmp("env", command)) {
        t_env_flags *env_flags = create_env_flags();
        if (!find_env_flags(env_flags, params)) {
            t_dirs_to_work.exit_status = clear_env(env_flags, params);
        } else {
            t_dirs_to_work.exit_status = 1;
        }
        free(env_flags);
        return 0;

    } else if (!mx_strcmp("pwd", command)) {
        t_pwd_flags *pwd_flag = create_pwd_flags();
        if (!find_pwd_flags(pwd_flag, params)) {
            t_dirs_to_work.exit_status = clear_pwd(pwd_flag);
        } else {
            t_dirs_to_work.exit_status = 1;
        }
        free(pwd_flag);
        return 0;

    } else if (!mx_strcmp("cd", command)) {
        t_cd_flags *cd_flag = create_cd_flags();

        if (!find_cd_flags(cd_flag, params)) {
            t_dirs_to_work.exit_status = clear_cd(params, cd_flag);
        } else {
            t_dirs_to_work.exit_status = 1;
        }
        free(cd_flag);
        return 0;

    } else if (!mx_strcmp("which", command)) {
        t_which_flags *which_flag = create_which_flags();
        if (!find_which_flags(which_flag, params)) {
            t_dirs_to_work.exit_status = clear_which(which_flag, params);
        } else {
            t_dirs_to_work.exit_status = 1;
        }
        free(which_flag);
        return 0;
    } else if (!mx_strcmp("echo", command)) {
        t_echo_flags *echo_flag = create_echo_flags();
        if (!find_echo_flags(echo_flag, params)) {
            t_dirs_to_work.exit_status = clear_echo(echo_flag, &(*commands_arr)[i]);
        } else {
            t_dirs_to_work.exit_status = 1;
        }
        free(echo_flag);
        return 0;

    } else if (!mx_strcmp("fg", command)) {
        // if (params[1] == NULL)
        //     return 0;
        
        t_jobs *node = jobs;
        while (node->next != NULL) 
            node = node->next;
        
        if (node == NULL) {
            mx_printerr("ush: fg: no such job\n");
            t_dirs_to_work.exit_status = 1;
            return 0;
        }

        kill(node->pid, SIGCONT);
        int child_status = 0;
        waitpid(node->pid, &child_status, WUNTRACED);
        t_dirs_to_work.exit_status = WEXITSTATUS(child_status);
        if (!WIFSTOPPED(child_status))
            jobs_remove(&jobs, node->pid);
            
        if (WIFSIGNALED(child_status)) {
            t_dirs_to_work.exit_status = 130;
        } else {
            t_dirs_to_work.exit_status = 0;
        }
        return 0;
    } else if (!mx_strcmp("export", command)) {
        if (params[1] == NULL) {
            t_env_flags *env_flag = create_env_flags();
            clear_env(env_flag, params);
            t_dirs_to_work.exit_status = 0;
            free(env_flag);
            return 0;
        }

        for (int i = 1; params[i] != NULL; i++) {
            char *ptr = mx_strchr(params[i], '=');
            if (ptr == NULL)
                continue;
            
            char **arr = mx_strsplit(params[i], '=');
            setenv(arr[0], arr[1], 1);
            mx_del_strarr(&arr);
        }
        t_dirs_to_work.exit_status = 0;
        return 0;
    }

    return -1;
}
