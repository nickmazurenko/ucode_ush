#include "../inc/ush.h"

static void go_to_parent();
static void go_to_dir(char *dir);
static int check_lnk_path(char *path);

char *mx_rep_tilda(char *str) {
    int index = mx_get_char_index(str, '~');
    if (index == -1)
        return NULL;
    char *res = malloc(PATH_MAX);
    mx_memset(res, 0, PATH_MAX);
    mx_memcpy(res, str, index);
    mx_memcpy(res + index, t_global.HOME, mx_strlen(t_global.HOME));
    mx_memcpy(res + index + mx_strlen(t_global.HOME), 
        str + index + 1, PATH_MAX - index - 1 - mx_strlen(t_global.HOME));
    return res;
}

int mx_builtin_cd(char **params, t_flags_cd *flags) {
    mx_apply_escapes(&params);
    int argc = 0;
    for (; params[argc] != NULL; argc++);

    char real_buf[PATH_MAX];
    char *path = NULL;

    char tmp_PWD[PATH_MAX];
    mx_memcpy(tmp_PWD, t_global.PWD, PATH_MAX);

    if (argc > 1) {
        if (params[1][0] == '-' && mx_strlen(params[1]) > 1)
            path = mx_strdup(params[2]);
        else
            path = mx_strdup(params[1]);
    }

    switch (argc)
    {
    case 1:
        if (t_global.HOME[0] != '\0') {
            chdir(t_global.HOME);
            if (mx_strcmp(t_global.OLDPWD, t_global.PWD)) {
                mx_memcpy(t_global.OLDPWD, t_global.PWD, PATH_MAX);
                setenv("OLDPWD", t_global.OLDPWD, 1);
            }
            mx_memcpy(t_global.PWD, t_global.HOME, PATH_MAX);
            setenv("PWD", t_global.PWD, 1);
        }
        free(path);
        return 0;
        break;
    case 2:
        if (params[1][0] == '-' && mx_strlen(params[1]) > 1) {
            if (t_global.HOME[0] != '\0') {
                chdir(t_global.HOME);
                if (mx_strcmp(t_global.OLDPWD, t_global.PWD)) {
                    mx_memcpy(t_global.OLDPWD, t_global.PWD, PATH_MAX);
                    setenv("OLDPWD", t_global.OLDPWD, 1);
                }
                mx_memcpy(t_global.PWD, t_global.HOME, PATH_MAX);
                setenv("PWD", t_global.PWD, 1);
            }
            free(path);
            return 0;
        }
        else  if (params[1][0] == '-') {
            if (t_global.OLDPWD[0] != '\0') {
                chdir(t_global.OLDPWD);
                setenv("OLDPWD", t_global.PWD, 1);
                setenv("PWD", t_global.OLDPWD, 1);

                mx_memcpy(t_global.PWD, t_global.OLDPWD, PATH_MAX);
                mx_memset(t_global.OLDPWD, 0, PATH_MAX);
                mx_memcpy(t_global.OLDPWD, getenv("OLDPWD"), mx_strlen(getenv("OLDPWD")));

                char *test = mx_strrep(t_global.PWD, t_global.HOME, TILDA);
                if (test != NULL) {
                    //mx_printstr(test);
                    //mx_printchar('\n');
                    free(test);
                }
                else {
                    //mx_printstr(t_global.PWD);
                    //mx_printchar('\n');
                }
            }
            free(path);
            return 0;
        }
        break;
    case 3:
        if (params[1][0] == '-')
            break;
        else {
            char *tmp = mx_strrep(t_global.PWD, params[1], params[2]);
            if (tmp != NULL) {
                free(path);
                path = tmp;
            }
            else {
                mx_printerr("cd: string not in pwd: ");
                mx_printerr(params[1]);
                mx_printerr("\n");
                return 1;
            }
        }
        break;
    case 4:
        if (params[1][0] != '-') {
            mx_printerr("ush: cd: too many arguments\n");
            free(path);
            return 1;
        }
        else {
            char *tmp = mx_strrep(t_global.PWD, params[2], params[3]);
            if (tmp != NULL) {
                free(path);
                path = tmp;
            }
            else {
                mx_printerr("cd: string not in pwd: ");
                mx_printerr(params[1]);
                mx_printerr("\n");
                return 1;
            }
        }
        break;
    default:
        break;
    }

    char *tilda_path = rep_substr(path, "~", t_global.HOME);//mx_rep_tilda(path);
    if (tilda_path == NULL)
        tilda_path = mx_strdup(path);

    if (flags->using_s) {
        if (check_lnk_path(tilda_path)) {
            mx_printerr("cd: not a directory: ");
            mx_printerr(tilda_path);
            mx_printerr("\n");
            if (path != NULL)
                free(path);
            free(tilda_path);
            return 1;
        }
        if (path != NULL)
            free(path);
        free(tilda_path);
        return 0;
    }

    if (path != NULL && flags->using_P) {
        char *res = NULL;
        res = realpath(tilda_path, real_buf);
        free(tilda_path);
        tilda_path = mx_strdup(real_buf);
        if (!res)
            free(res);
    }

    int status;
    if (tilda_path[0] == '/') {
        mx_memset(t_global.PWD, '\0', mx_strlen(t_global.PWD));
        t_global.PWD[0] = '/';
    }

    // Main algorithm
    //====================================
    char **arr = mx_strsplit(tilda_path, '/');
    for (int i = 0; arr[i] != NULL; i++) {
        if (!mx_strcmp(arr[i], ".."))
            go_to_parent();
        else if (mx_strcmp(arr[i], "."))
            go_to_dir(arr[i]);
    }
    mx_del_strarr(&arr);
    //====================================

    status = chdir(t_global.PWD);
    if (status < 0) {
        mx_printerr("cd: ");
        char *err_tmp = mx_strdup(strerror(errno));
        if (err_tmp[0] < 97)    
            err_tmp[0] += 32;
        mx_printerr(err_tmp);
        free(err_tmp);
        mx_printerr(": ");
        mx_printerr(path);
        mx_printerr("\n");
        mx_memcpy(t_global.PWD, tmp_PWD, PATH_MAX);
        return 1;
    }
    else {
        if (mx_strcmp(t_global.OLDPWD, tmp_PWD)) {
            mx_memcpy(t_global.OLDPWD, tmp_PWD, PATH_MAX);
            status = setenv("OLDPWD", t_global.OLDPWD, 1);
        }
        status = setenv("PWD", t_global.PWD, 1);
    }
    if (path != NULL)
        free(path);
    free(tilda_path);
    return 0;
}

/*
    Remove the name of the last directory in the t_global.PWD variable
*/
static void go_to_parent() {
    char *ptr = strrchr(t_global.PWD, '/');
    if (ptr)
        mx_memset(ptr, '\0', mx_strlen(ptr));
    else
        t_global.PWD[0] = '/';
    
    if (t_global.PWD[0] == '\0')
        t_global.PWD[0] = '/';
}

/*
    Append to the t_global.PWD variable the name of the directory, specified in the 'dir'
*/
static void go_to_dir(char *dir) {
    int len = mx_strlen(t_global.PWD);
    if (t_global.PWD[len - 1] != '/')
        t_global.PWD[mx_strlen(t_global.PWD)] = '/';
    mx_memcpy(t_global.PWD + mx_strlen(t_global.PWD), dir, mx_strlen(dir));
}


/*
    Execute the same algorithm as in the main body, 
    but checking every directory on the path for link
   
    Return value: 1 if link was found, 0 otherwise
*/
static int check_lnk_path(char *path) {
    char tmp_lnk_PWD[PATH_MAX];
    mx_memcpy(tmp_lnk_PWD, t_global.PWD, PATH_MAX);
    if (path[0] == '/') {
        memset(t_global.PWD, 0, PATH_MAX);
        t_global.PWD[0] = '/';
    }

    char buf[PATH_MAX];
    memset(buf, 0, PATH_MAX);
    ssize_t nbytes = 0;
    char **lnk_arr = mx_strsplit(path, '/');

    for (int i = 0; lnk_arr[i] != NULL; i++) {
        if (!mx_strcmp(lnk_arr[i], ".."))
            go_to_parent();
        else if (mx_strcmp(lnk_arr[i], "."))
            go_to_dir(lnk_arr[i]);

        nbytes = readlink(t_global.PWD, buf, PATH_MAX);
        if (nbytes > 0 && mx_strcmp(lnk_arr[i], "..") && mx_strcmp(lnk_arr[i], ".")) {
            mx_del_strarr(&lnk_arr);
            mx_memcpy(t_global.PWD, tmp_lnk_PWD, PATH_MAX);
            return 1;
        }
        mx_memset(buf, 0, mx_strlen(buf));
    }
    mx_del_strarr(&lnk_arr);

    int status = chdir(t_global.PWD);
    if (status < 0) {
        mx_printerr("cd: ");
        char *err_tmp = mx_strdup(strerror(errno));
        if (err_tmp[0] < 97)    
            err_tmp[0] += 32;
        mx_printerr(err_tmp);
        free(err_tmp);
        mx_printerr(": ");
        mx_printerr(path);
        mx_printerr("\n");
        mx_memcpy(t_global.PWD, tmp_lnk_PWD, PATH_MAX);
    }
    else {
        if (mx_strcmp(t_global.OLDPWD, tmp_lnk_PWD)) {
            mx_memcpy(t_global.OLDPWD, tmp_lnk_PWD, PATH_MAX);
            status = setenv("OLDPWD", t_global.OLDPWD, 1);
        }
        status = setenv("PWD", t_global.PWD, 1);
    }

    return 0;
}
