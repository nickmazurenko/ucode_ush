#include "ush.h"
#include "which.h"

t_which_flags *create_which_flags(void);
void print_which_error(char wrong_flag);
int find_which_flags(t_which_flags *which_flag, char **args);


int clear_which(t_which_flags *which_flag, char **args) {
    handle_escape_symbols(&args);
    DIR *d;
    struct dirent *main_info;
    struct stat side_info;
    bool is_present = false;
    bool is_here = true;

    char *commands[8] = {"cd", "pwd", "env", "which", "echo", "exit", "unset", "fg"};

    if (which_flag->is_a_flag) {
        int args_idx = 2;
        if (args[args_idx] == NULL) return 1;

        char *name = NULL;
        char **path_to_dir = mx_strsplit(getenv("PATH"), ':');

        while (args[args_idx] != NULL) {
            for (int command_idx = 0; command_idx < 8; command_idx++) {
                if (!mx_strcmp(commands[command_idx], args[args_idx])) {
                    mx_printerr(args[args_idx]);
                    mx_printerr(": shell built-in command\n");
                    is_present = true;
                    break;
                }
            }
            if (!mx_strcmp(args[args_idx], "export")) {
                mx_printerr("export: ush reserved word\n");
                is_present = true;
            }

            if (args[args_idx][0] == '/') {
                lstat(args[args_idx], &side_info);
                if (side_info.st_mode & S_IXUSR) {
                    mx_printstr(args[args_idx]);
                    mx_printchar('\n');
                    is_present = true;
                } else {
                    mx_printerr(args[args_idx]);
                    mx_printerr(" not found\n");
                    args_idx++;
                    is_present = true;
                    is_here = false;
                    continue;
                }
            }

            for (int path_idx = 0; path_to_dir[path_idx] != NULL; path_idx++) {
                d = opendir(path_to_dir[path_idx]);
                if (d != NULL) {
                    while ((main_info = readdir(d)) != NULL) {
                        if (!mx_strcmp(main_info->d_name, args[args_idx])) {
                            name = mx_strnew(256);
                            mx_strcpy(name, path_to_dir[path_idx]);
                            name = mx_strcat(name, "/");
                            name = mx_strcat(name, args[args_idx]);
                            mx_printstr(name);
                            mx_printchar('\n');
                            free(name);
                            is_present = true;
                            break;
                        }       
                    }
                    closedir(d);
                }
            }
            if (is_present == false) {
                is_here = false;
                mx_printerr(args[args_idx]);
                mx_printerr(" not found\n");
            }
            args_idx++;
        }

        mx_del_strarr(&path_to_dir);
        if (!is_here) return 1;
        return 0;
    } else if (args[1] != NULL) {
        int args_idx = 1;
        if (which_flag->is_s_flag) args_idx = 2;
        
        if (args[args_idx] == NULL) return 1;
            
        char *name = NULL;
        char **path_to_dir = mx_strsplit(getenv("PATH"), ':');

        while (args[args_idx] != NULL) {
            int built = 0;
            for (int command_idx = 0; command_idx < 8; command_idx++) {
                if (!mx_strcmp(commands[command_idx], args[args_idx])) {
                    mx_printerr(args[args_idx]);
                    mx_printerr(": shell built-in command\n");
                    is_present = true;
                    built = 1;
                    args_idx++;
                    break;
                }
            }
            if (built) {
                built = 0;
                is_present = false;
                continue;
            }
            if (!mx_strcmp(args[args_idx], "export")) {
                mx_printerr("export: ush reserved word\n");
                is_present = true;
                args_idx++;
                continue;
            }

            if (args[args_idx][0] == '/') {
                lstat(args[args_idx], &side_info);
                if (side_info.st_mode & S_IXUSR) {
                    mx_printstr(args[args_idx]);
                    mx_printchar('\n');
                    args_idx++;
                    is_present = true;
                    continue;
                } else {
                    mx_printerr(args[args_idx]);
                    mx_printerr(" not found\n");
                    args_idx++;
                    is_present = true;
                    is_here = false;
                    continue;
                }
            }

            for (int path_idx = 0; path_to_dir[path_idx] != NULL; path_idx++) {
                d = opendir(path_to_dir[path_idx]);
                if (d != NULL) {
                    while ((main_info = readdir(d)) != NULL) {
                        if (!mx_strcmp(main_info->d_name, args[args_idx])) {
                            name = mx_strnew(256);
                            mx_strcpy(name, path_to_dir[path_idx]);
                            name = mx_strcat(name, "/");
                            name = mx_strcat(name, args[args_idx]);
                            mx_printstr(name);
                            mx_printchar('\n');
                            free(name);
                            is_present = true;
                            break;
                        }       
                    }
                    closedir(d);
                }
            }

            if (is_present == false) {
                is_here = false;
                mx_printerr(args[args_idx]);
                mx_printerr(" not found\n");
            }
            is_present = false;
            args_idx++;
        }
        mx_del_strarr(&path_to_dir);
        if (is_here) return 0;
        return 1;
    } else return 1;
}

t_which_flags *create_which_flags(void) {
    t_which_flags *which_flag = (t_which_flags*)malloc(sizeof(t_which_flags));
    which_flag->is_a_flag = false;
    which_flag->is_s_flag = false;
    return which_flag;
}

void print_which_error(char wrong_flag) {
    mx_printerr("which: bad option: -");
    mx_printerr(&wrong_flag);
    mx_printerr("\n");
}

int find_which_flags(t_which_flags *which_flag, char **args) {
    if (args[1] != NULL) {
        if (args[1][0] == '-') {
            for (int i = 1; i < mx_strlen(args[1]); i++) {
                if (args[1][i] == 'a') {
                    which_flag->is_a_flag = true;
                } else if(args[1][i] == 's') {
                    which_flag->is_s_flag = true;
                } else {
                    print_which_error(args[1][i]);
                    return 1;
                }
            }
        }
    }
    return 0;
}
