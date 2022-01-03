#include "../inc/ush.h"

int mx_builtin_which(t_flags_which *flags, char **data) {
    mx_apply_escapes(&data);
    DIR *d;
    struct dirent *dir;
    struct stat sb;
    bool found = false;
    bool here = true;

    char *builtins[8] = {"cd", "pwd", "env", "which", "echo", "exit", "unset", "fg"};

    if (flags->using_A) {
        int i = 2;
        if (data[i] == NULL)
            return 1;
        char *name = NULL;
        char **path_dir = mx_strsplit(getenv("PATH"), ':');

        while (data[i] != NULL) {
            for (int z = 0; z < 8; z++) {
                if (!mx_strcmp(builtins[z], data[i])) {
                    mx_printerr(data[i]);
                    mx_printerr(": ush built-in command\n");
                    found = true;
                    break;
                }
            }
            if (!mx_strcmp(data[i], "export")) {
                mx_printerr("export: ush reserved word\n");
                found = true;
            }

            if (data[i][0] == '/') {
                lstat(data[i], &sb);
                if (sb.st_mode & S_IXUSR) {
                    mx_printstr(data[i]);
                    mx_printchar('\n');
                    found = true;
                }
                else {
                    mx_printerr(data[i]);
                    mx_printerr(" not found\n");
                    i++;
                    found = true;
                    here = false;
                    continue;
                }
            }

            for (int j = 0; path_dir[j] != NULL; j++) {
                d = opendir(path_dir[j]);
                if (d != NULL) {
                    while ((dir = readdir(d)) != NULL) {
                        if (!mx_strcmp(dir->d_name, data[i])) {
                            name = mx_strnew(256);
                            mx_strcpy(name, path_dir[j]);
                            name = mx_strcat(name, "/");
                            name = mx_strcat(name, data[i]);
                            mx_printstr(name);
                            mx_printchar('\n');
                            free(name);
                            found = true;
                            break;
                        }       
                    }
                    closedir(d);
                }
            }
            if (found == false) {
                here = false;
                mx_printerr(data[i]);
                mx_printerr(" not found\n");
            }
            i++;
        }

        mx_del_strarr(&path_dir);
        if (!here)
            return 1;
        return 0;
    }
    else if (data[1] != NULL) {
        int i = 1;
        if (flags->using_S)
            i = 2;
        
        if (data[i] == NULL)
            return 1;
            
        char *name = NULL;
        char **path_dir = mx_strsplit(getenv("PATH"), ':');

        while (data[i] != NULL) {
            int built = 0;
            for (int z = 0; z < 8; z++) {
                if (!mx_strcmp(builtins[z], data[i])) {
                    mx_printerr(data[i]);
                    mx_printerr(": ush built-in command\n");
                    found = true;
                    built = 1;
                    i++;
                    break;
                }
            }
            if (built) {
                built = 0;
                found = false;
                continue;
            }
            if (!mx_strcmp(data[i], "export")) {
                mx_printerr("export: ush reserved word\n");
                found = true;
                i++;
                continue;
            }

            if (data[i][0] == '/') {
                lstat(data[i], &sb);
                if (sb.st_mode & S_IXUSR) {
                    mx_printstr(data[i]);
                    mx_printchar('\n');
                    i++;
                    found = true;
                    continue;
                }
                else {
                    mx_printerr(data[i]);
                    mx_printerr(" not found\n");
                    i++;
                    found = true;
                    here = false;
                    continue;
                }
            }

            for (int j = 0; path_dir[j] != NULL; j++) {
                d = opendir(path_dir[j]);
                if (d != NULL) {
                    while ((dir = readdir(d)) != NULL) {
                        if (!mx_strcmp(dir->d_name, data[i])) {
                            name = mx_strnew(256);
                            mx_strcpy(name, path_dir[j]);
                            name = mx_strcat(name, "/");
                            name = mx_strcat(name, data[i]);
                            mx_printstr(name);
                            mx_printchar('\n');
                            free(name);
                            found = true;
                            break;
                        }       
                    }
                    closedir(d);
                }
            }

            if (found == false) {
                here = false;
                mx_printerr(data[i]);
                mx_printerr(" not found\n");
            }
            found = false;
            i++;
        }
        mx_del_strarr(&path_dir);
        if (here)
            return 0;
        return 1;
    }
    else
        return 1;
}
