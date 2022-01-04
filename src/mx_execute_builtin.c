#include "../inc/ush.h"

int mx_execute_builtin(char *command, char **params, char ***commands_arr, int i) {
    // EXIT
    if (!mx_strcmp("exit", command)) {
        int ext_val = 0;
        if (params[1] != NULL)
            ext_val = mx_atoi(params[1]);

        free(command);
        mx_del_strarr(&params);
        mx_del_strarr(commands_arr);

        exit(ext_val);
    }

    // UNSET
    else if (!mx_strcmp("unset", command)) {
        if (mx_unset_check_param(params) == 0) {
        
            for (int i = 1; params[i] != NULL; i++) {
                if (mx_builtin_unset(params[i]) < 0) {
                    perror("ush: unset");
                    t_dirs_to_work.exit_status = 1;
                    continue;
                }
            }
            
        }
        return 0;
    }

    // ENV
    else if (!mx_strcmp("env", command)) {
        t_env_flags *env_flags = create_env_flags();
        if (!find_env_flags(env_flags, params))
            t_dirs_to_work.exit_status = clear_env(env_flags, params);
        else
            t_dirs_to_work.exit_status = 1;
        free(env_flags);
        return 0;
    }

    // PWD
    else if (!mx_strcmp("pwd", command)) {
        t_pwd_flags *pwd_flags = create_pwd_flags();
        if (!find_pwd_flags(pwd_flags, params))
            t_dirs_to_work.exit_status = clear_pwd(pwd_flags);
        else
            t_dirs_to_work.exit_status = 1;
        free(pwd_flags);
        return 0;
    }

    // CD
    else if (!mx_strcmp("cd", command)) {
        t_cd_flags *cd_flag = create_cd_flags();

        if (!find_cd_flags(cd_flag, params))
            t_dirs_to_work.exit_status = clear_cd(params, cd_flag);
        else
            t_dirs_to_work.exit_status = 1;

        free(cd_flag);
        return 0;
    }
    
    // WHICH
    else if (!mx_strcmp("which", command)) {
        t_flags_which which_flags;
        mx_which_flags_init(&which_flags);
        if (!mx_which_flags_set(&which_flags, params))
            t_dirs_to_work.exit_status = mx_builtin_which(&which_flags, params);
        else
            t_dirs_to_work.exit_status = 1;
        return 0;
    }

    // ECHO
    else if (!mx_strcmp("echo", command)) {
        t_flags_echo echo_flags;
        mx_echo_flags_init(&echo_flags);
        if (!mx_echo_flags_set(&echo_flags, params))
            t_dirs_to_work.exit_status = mx_builtin_echo(&echo_flags, &(*commands_arr)[i]);
        else
            t_dirs_to_work.exit_status = 1;
        return 0;
    }

    // fg
    else if (!mx_strcmp("fg", command)) {
        if (params[1] == NULL)
            return 0;
        
        t_jobs *node = jobs;
        while (node != NULL && node->job_id != mx_atoi(params[1])) 
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
            
        if (WIFSIGNALED(child_status))
            t_dirs_to_work.exit_status = 130;
        else
            t_dirs_to_work.exit_status = 0;
        return 0;
    }

    // EXPORT
    else if (!mx_strcmp("export", command)) {
        if (params[1] == NULL) {
            t_env_flags *env_flags = create_env_flags();
            clear_env(env_flags, params);
            t_dirs_to_work.exit_status = 0;
            free(env_flags);
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
