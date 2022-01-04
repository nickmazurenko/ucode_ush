#include "ush.h"

void initialise_dirs(void) {
    t_dirs_to_work.PWD = mx_strnew(PATH_MAX);
    t_dirs_to_work.PWD = mx_strcpy(t_dirs_to_work.PWD, getenv("PWD"));

    t_dirs_to_work.OLDPWD = mx_strnew(PATH_MAX);
    char *system_var = getenv("OLDPWD");
    if (system_var != NULL)
        t_dirs_to_work.OLDPWD = mx_strcpy(t_dirs_to_work.OLDPWD, system_var);

    t_dirs_to_work.HOME = mx_strnew(PATH_MAX);
    system_var = getenv("HOME");
    if (system_var != NULL)
        t_dirs_to_work.HOME = mx_strcpy(t_dirs_to_work.HOME, system_var);

    t_dirs_to_work.PATH = mx_strnew(PATH_MAX);
    system_var = getenv("PATH");
    if (system_var != NULL)
        t_dirs_to_work.PATH = mx_strcpy(t_dirs_to_work.PATH, system_var);

    jobs = jobs_new_node(getpid(), "ush");

    int shlvl = mx_atoi(getenv("SHLVL"));
    setenv("SHLVL", mx_itoa(shlvl + 1), 1);
    char *shell = mx_strjoin(t_dirs_to_work.PWD, "/");
    shell = mx_strjoin(shell, "ush");
    setenv("SHELL", shell, 1);
    free(shell);
}

void free_dirs(void) {
    free(t_dirs_to_work.PWD);
    free(t_dirs_to_work.OLDPWD);
    free(t_dirs_to_work.HOME);
    free(t_dirs_to_work.PATH);
    jobs_clear(&jobs);
}
