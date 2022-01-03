#include "../inc/ush.h"

void mx_init_global(void) {
    t_global.PWD = mx_strnew(PATH_MAX);
    t_global.PWD = mx_strcpy(t_global.PWD, getenv("PWD"));

    t_global.OLDPWD = mx_strnew(PATH_MAX);
    char *system_var = getenv("OLDPWD");
    if (system_var != NULL)
        t_global.OLDPWD = mx_strcpy(t_global.OLDPWD, system_var);

    t_global.HOME = mx_strnew(PATH_MAX);
    system_var = getenv("HOME");
    if (system_var != NULL)
        t_global.HOME = mx_strcpy(t_global.HOME, system_var);

    t_global.PATH = mx_strnew(PATH_MAX);
    system_var = getenv("PATH");
    if (system_var != NULL)
        t_global.PATH = mx_strcpy(t_global.PATH, system_var);

    jobs = jobs_new_node(getpid(), "ush");

    int shlvl = mx_atoi(getenv("SHLVL"));
    setenv("SHLVL", mx_itoa(shlvl + 1), 1);
    char *shell = mx_strjoin(t_global.PWD, "/");
    shell = mx_strjoin(shell, "ush");
    setenv("SHELL", shell, 1);
    free(shell);
}

void mx_free_global(void) {
    free(t_global.PWD);
    free(t_global.OLDPWD);
    free(t_global.HOME);
    free(t_global.PATH);
    jobs_clear(&jobs);
}
