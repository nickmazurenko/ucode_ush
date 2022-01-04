#include "ush.h"
static int handle_links(char *path_to_file);
static void change_to_root(void);
static void add_dir(char *dir);
void print_cd_error(char wrong_flag);
int find_cd_flags(t_cd_flags *data, char **flags);
int count_dest(char **dest);
t_cd_flags *create_cd_flags(void);

// full cd function
int clear_cd(char **dest, t_cd_flags *flags) {
    mx_apply_escapes(&dest);

    int dest_idx = 0;
    char buff[PATH_MAX];
    char *full_path = NULL;
    char pwd[PATH_MAX];

    dest_idx = count_dest(dest);
    mx_memcpy(pwd, t_dirs_to_work.PWD, PATH_MAX);

    if (dest_idx > 1) {
        if (dest[1][0] == '-' && mx_strlen(dest[1]) > 1)
            full_path = mx_strdup(dest[2]);
        else
            full_path = mx_strdup(dest[1]);
    }

    switch (dest_idx) {

    case 1:
        if (t_dirs_to_work.HOME[0] != '\0') {

            chdir(t_dirs_to_work.HOME);

            if (mx_strcmp(t_dirs_to_work.OLDPWD, t_dirs_to_work.PWD)) {

                mx_memcpy(t_dirs_to_work.OLDPWD, t_dirs_to_work.PWD, PATH_MAX);
                setenv("OLDPWD", t_dirs_to_work.OLDPWD, 1);
            }

            mx_memcpy(t_dirs_to_work.PWD, t_dirs_to_work.HOME, PATH_MAX);
            setenv("PWD", t_dirs_to_work.PWD, 1);
        }

        free(full_path);
        return 0;
        break;

    case 2:
        if (dest[1][0] == '-' && mx_strlen(dest[1]) > 1) {
            if (t_dirs_to_work.HOME[0] != '\0') {

                chdir(t_dirs_to_work.HOME);

                if (mx_strcmp(t_dirs_to_work.OLDPWD, t_dirs_to_work.PWD)) {
                    mx_memcpy(t_dirs_to_work.OLDPWD, t_dirs_to_work.PWD, PATH_MAX);
                    setenv("OLDPWD", t_dirs_to_work.OLDPWD, 1);
                }

                mx_memcpy(t_dirs_to_work.PWD, t_dirs_to_work.HOME, PATH_MAX);
                setenv("PWD", t_dirs_to_work.PWD, 1);
            }

            free(full_path);
            return 0;
        } else  if (dest[1][0] == '-') {

            if (t_dirs_to_work.OLDPWD[0] != '\0') {
                chdir(t_dirs_to_work.OLDPWD);
                setenv("OLDPWD", t_dirs_to_work.PWD, 1);
                setenv("PWD", t_dirs_to_work.OLDPWD, 1);

                mx_memcpy(t_dirs_to_work.PWD, t_dirs_to_work.OLDPWD, PATH_MAX);
                mx_memset(t_dirs_to_work.OLDPWD, 0, PATH_MAX);
                mx_memcpy(t_dirs_to_work.OLDPWD, getenv("OLDPWD"), mx_strlen(getenv("OLDPWD")));

                char *test = mx_strrep(t_dirs_to_work.PWD, t_dirs_to_work.HOME, TILDA);

                if (test != NULL)
                    free(test);
            }
            free(full_path);
            return 0;
        }
        break;

    case 3:
        if (dest[1][0] == '-')
            break;
        else {
            char *tmp = mx_strrep(t_dirs_to_work.PWD, dest[1], dest[2]);
            if (tmp != NULL) {
                free(full_path);
                full_path = tmp;
            }
            else {
                mx_printerr("cd: string not in pwd: ");
                mx_printerr(dest[1]);
                mx_printerr("\n");
                return 1;
            }
        }
        break;

    case 4:
        if (dest[1][0] != '-') {
            mx_printerr("ush: cd: too many arguments\n");
            free(full_path);
            return 1;
        } else {
            char *tmp = mx_strrep(t_dirs_to_work.PWD, dest[2], dest[3]);
            if (tmp != NULL) {
                free(full_path);
                full_path = tmp;
            } else {
                mx_printerr("cd: string not in pwd: ");
                mx_printerr(dest[1]);
                mx_printerr("\n");
                return 1;
            }
        }
        break;
    }

    char *tilda_path = rep_substr(full_path, "~", t_dirs_to_work.HOME);//mx_rep_tilda(path);
    if (tilda_path == NULL)
        tilda_path = mx_strdup(full_path);

    if (flags->is_s_flag) {
        if (handle_links(tilda_path)) {
            mx_printerr("cd: not a directory: ");
            mx_printerr(tilda_path);
            mx_printerr("\n");
            if (full_path != NULL)
                free(full_path);
            free(tilda_path);
            return 1;
        }
        if (full_path != NULL)
            free(full_path);
        free(tilda_path);
        return 0;
    }

    if (full_path != NULL && flags->is_p_flag) {
        char *res = NULL;
        res = realpath(tilda_path, buff);
        free(tilda_path);
        tilda_path = mx_strdup(buff);
        if (!res)
            free(res);
    }

    int status = 0;
    if (tilda_path[0] == '/') {
        mx_memset(t_dirs_to_work.PWD, '\0', mx_strlen(t_dirs_to_work.PWD));
        t_dirs_to_work.PWD[0] = '/';
    }

    char **arr = mx_strsplit(tilda_path, '/');
    for (int i = 0; arr[i] != NULL; i++) {
        if (!mx_strcmp(arr[i], ".."))
            change_to_root();
        else if (mx_strcmp(arr[i], "."))
            add_dir(arr[i]);
    }
    mx_del_strarr(&arr);


    status = chdir(t_dirs_to_work.PWD);
    if (status < 0) {
        mx_printerr("cd: ");
        char *err_tmp = mx_strdup(strerror(errno));
        if (err_tmp[0] < 97)    
            err_tmp[0] += 32;
        mx_printerr(err_tmp);
        free(err_tmp);
        mx_printerr(": ");
        mx_printerr(full_path);
        mx_printerr("\n");
        mx_memcpy(t_dirs_to_work.PWD, pwd, PATH_MAX);
        return 1;
    } else {
        if (mx_strcmp(t_dirs_to_work.OLDPWD, pwd)) {
            mx_memcpy(t_dirs_to_work.OLDPWD, pwd, PATH_MAX);
            status = setenv("OLDPWD", t_dirs_to_work.OLDPWD, 1);
        }
        status = setenv("PWD", t_dirs_to_work.PWD, 1);
    }
    if (full_path != NULL)
        free(full_path);
    free(tilda_path);
    return 0;
}

static void change_to_root() {
    char *pipe_ptr = strrchr(t_dirs_to_work.PWD, '/');
    if (pipe_ptr)
        mx_memset(pipe_ptr, '\0', mx_strlen(pipe_ptr));
    else
        t_dirs_to_work.PWD[0] = '/';
    
    if (t_dirs_to_work.PWD[0] == '\0')
        t_dirs_to_work.PWD[0] = '/';
}

static int handle_links(char *path_to_file) {
    char pwd_tmp[PATH_MAX];
    char buff[PATH_MAX];
    ssize_t bytes_num = 0;

    mx_memcpy(pwd_tmp, t_dirs_to_work.PWD, PATH_MAX);

    if(path_to_file[0] == '/') {
        memset(t_dirs_to_work.PWD, 0, PATH_MAX);
        t_dirs_to_work.PWD[0] = '/';
    }

    memset(buff, 0, PATH_MAX);

    char **links = mx_strsplit(path_to_file, '/');

    for(int link_idx = 0; links[link_idx] != NULL; link_idx++) {
        if(mx_strcmp(links[link_idx], "..") == 0) {
            change_to_root();
        } else if (mx_strcmp(links[link_idx], ".") == 0) {
            add_dir(links[link_idx]);
        }
        bytes_num = readlink(t_dirs_to_work.PWD, buff, PATH_MAX);

        if (bytes_num > 0 && mx_strcmp(links[link_idx], "..") && mx_strcmp(links[link_idx], ".")) {
            mx_del_strarr(&links);
            mx_memcpy(t_dirs_to_work.PWD, pwd_tmp, PATH_MAX);
            return 1;
        }
        mx_memset(buff, 0, mx_strlen(buff));
    }

    mx_del_strarr(&links);

    int exit_status = chdir(t_dirs_to_work.PWD);

    if (exit_status < 0) {
        mx_printerr("cd: ");
        char *err_tmp = mx_strdup(strerror(errno));
        if (err_tmp[0] < 97)
            err_tmp[0] += 32;
        mx_printerr(err_tmp);
        free(err_tmp);
        mx_printerr(": ");
        mx_printerr(path_to_file);
        mx_printerr("\n");
        mx_memcpy(t_dirs_to_work.PWD, pwd_tmp, PATH_MAX);
    } else {
        if (mx_strcmp(t_dirs_to_work.OLDPWD, pwd_tmp)) {
            mx_memcpy(t_dirs_to_work.OLDPWD, pwd_tmp, PATH_MAX);
            exit_status = setenv("OLDPWD", t_dirs_to_work.OLDPWD, 1);
        }
        exit_status = setenv("PWD", t_dirs_to_work.PWD, 1);
    }

    return 0;
}

t_cd_flags *create_cd_flags(void) {
    t_cd_flags *flag = (t_cd_flags *)malloc(sizeof(t_cd_flags));

    flag->is_s_flag = false;
    flag->is_p_flag = false;

    return flag;
}

//TODO send to file with errors
void print_cd_error(char wrong_flag) {
    mx_printerr("ush: cd: -");
    mx_printerr(&wrong_flag);
    mx_printerr(": invalid option\n");
    mx_printerr("cd: usage: cd [-sP]\n");
}

int find_cd_flags(t_cd_flags *data, char **flags) {
    if (flags[1] != NULL) {
        int flags_len = mx_strlen(flags[1]);
        if (flags[1][0] == '-') {
            for (int i = 1; i < flags_len; i++) {

                switch (flags[1][i]) {
                    case 'P':
                        data->is_p_flag = true;
                        break;
                    case 's':
                        data->is_s_flag = true;
                        break;
                    default:
                        print_cd_error(flags[1][i]);
                        return 1;
                        break;
                }
            }
        }
    }
    return 0;
}

int count_dest(char **dest) {
    int dest_quantity = 0;
    while(dest[dest_quantity] != NULL) {
        dest_quantity++;
    }
    return dest_quantity;
}

static void add_dir(char *dir) {

    int pwd_len = mx_strlen(t_dirs_to_work.PWD);
    if (t_dirs_to_work.PWD[pwd_len - 1] != '/')
        t_dirs_to_work.PWD[mx_strlen(t_dirs_to_work.PWD)] = '/';
    mx_memcpy(t_dirs_to_work.PWD + mx_strlen(t_dirs_to_work.PWD), dir, mx_strlen(dir));

}
