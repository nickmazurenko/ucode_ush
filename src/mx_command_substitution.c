#include "ush.h"
#include "utils.h"

static char *get_exe_command(char *line);

int check_pipe_commands(char** commands, char **str, char* echo_command, char* echo_command_copy) {
    for (int command_index = 0; commands[command_index] != NULL; command_index++) {
        char *command = get_exe_command(echo_command);
        FILE *fp;
        char *strrep = mx_strnew(PATH_MAX);
        fp = popen(command, "r");
        if (fp == NULL) {
            mx_printerr("Failed to run command\n");
            return 1;
        }
        char *strrep_ptr = strrep;
        while (fgets(strrep_ptr, PATH_MAX, fp) != NULL) {
            strrep_ptr = strrep + mx_strlen(strrep);
            strrep[mx_strlen(strrep) - 1] = ' ';
        }
        strrep[mx_strlen(strrep) - 1] = '\0';
        pclose(fp);

        *str = mx_strrep(*str, echo_command_copy, strrep);
        free(strrep);
        free(command);
    }
}

int handle_back_quotes(char* data, char** str) {
    char *back_quot = mx_strchr(data, '`');

    if (back_quot) {
        int back_quotes_number = mx_count_substr_new(back_quot, "`");
        if (mx_count_substr_new(back_quot, "`") % 2 != 0) {
            //mx_printerr("ush: unmatched: `\n");
            return 0;
        }
        while (back_quot != NULL) {
            char *tmp_back_quot = back_quot;

            *back_quot = '\0';
//            for (; *(tmp_back_quot + 1) != '\0';) {
//                mx_swap_char(tmp_back_quot, tmp_back_quot + 1);
//                tmp_back_quot++;
//            }
//
//            tmp_back_quot = back_quot;

            str_shift_left(back_quot, '\0');

            char *echo_command = mx_strnew(PATH_MAX);
            int echo_command_index = 0;
            for (; *tmp_back_quot != '`' && *tmp_back_quot != '\0'; echo_command_index++) {
                echo_command[echo_command_index] = *tmp_back_quot;
                tmp_back_quot++;
            }
            echo_command[echo_command_index] = '\0';
            *tmp_back_quot = '\0';
//            for (; *(tmp_back_quot + 1) != '\0';) {
//                mx_swap_char(tmp_back_quot, tmp_back_quot + 1);
//                tmp_back_quot++;
//            }
            str_shift_left(tmp_back_quot, '\0');

            char *echo_command_copy = mx_strdup(echo_command);

            for (char *tmp = echo_command; *tmp != '\0' && *tmp != ' '; tmp++) {
                *tmp = mx_char_to_upper(*tmp);
            }


            char **commands = mx_strsplit(echo_command, ';');

            check_pipe_commands(commands, str, echo_command, echo_command_copy);

            free(echo_command_copy);
            mx_del_strarr(&commands);

            data = *str;
            free(echo_command);
            back_quot = mx_strchr(data, '`');
        }
    }
    return 0;
}

int handle_dollars(char* data, char** str) {
    char* dollar = strchr(data, '$');
    if (!dollar)
        return 0;

    // Loop for replacing simple $COMMAND
    while (dollar != NULL) {
        if (*(dollar + 1) == '{' || *(dollar + 1) == '(') {
            dollar = strchr(++dollar, '$');
            continue;
        }
        char *tmp_ptr = dollar;

        if (*(dollar + 1) != '(' && *(dollar + 1) != '{') {
            int dollar_len = mx_strlen(dollar) + 2;
            char *replaced_str = mx_strnew(dollar_len);
            char *str_var = mx_strnew(dollar_len);

            replaced_str[0] = *tmp_ptr;
            tmp_ptr++;
            for (int i = 1, j = 0; *tmp_ptr != '\0' && *tmp_ptr != ' ' && *tmp_ptr != '$'; i++, j++) {
                if (*tmp_ptr != '?') {
                    if (!mx_isalpha(*tmp_ptr))
                        break;
                }
                replaced_str[i + 1] = '\0';
                str_var[j + 1] = '\0';

                replaced_str[i] = *tmp_ptr;
                str_var[j] = *tmp_ptr;
                if (str_var[0] == '?')
                    break;
                tmp_ptr++;
            }

            if (str_var[0] == '?') {
                char *status = mx_itoa(t_dirs_to_work.exit_status);
                *str = rep_substr(*str, replaced_str, status);
                free(status);
                free(replaced_str);
                free(str_var);
                dollar = strchr(*str, '$');
                continue;
            }

            char *env_val = getenv(str_var);
            char *env = NULL;
            if (env_val == NULL)
                env = mx_strdup("\0");
            else
                env = mx_strdup(env_val);

            *str = rep_substr(*str, replaced_str, env);

            free(env);
            free(replaced_str);
            free(str_var);
        }

        dollar = strchr(*str, '$');
    }

    dollar = strchr(*str, '$');
    if (!dollar)
        return 0;

    // Loop for other simple substitutions like ${COMMAND}
    while (dollar != NULL) {
        if (*(dollar + 1) == '(') {
            dollar = strchr(++dollar, '$');
            continue;
        }
        /*if (mx_count_substr(dollar, "}") != mx_count_substr(dollar, "{")) {
            //mx_printerr("ush: unmatched: }\n");
            break;
        }*/
        char *tmp_ptr = dollar;
        if (*(dollar + 1) == '{') {
            int ptr_len = mx_strlen(dollar) + 2;
            char *str_to_replace = mx_strnew(ptr_len);
            char *str_var = mx_strnew(ptr_len);

            str_to_replace[0] = *tmp_ptr;
            str_to_replace[1] = *(tmp_ptr + 1);
            tmp_ptr += 2;
            for (int i = 2, j = 0; *tmp_ptr != '}'; i++, j++) {
                str_to_replace[i + 1] = '\0';
                str_var[j + 1] = '\0';

                str_to_replace[i] = *tmp_ptr;
                if (*tmp_ptr != '}')
                    str_var[j] = *tmp_ptr;
                if (*tmp_ptr == ' ' || *tmp_ptr == '\0') {
                    mx_printerr("ush: bad substitution\n");
                    free(str_to_replace);
                    free(str_var);
                    t_dirs_to_work.exit_status = 1;
                    return -1;
                }
                tmp_ptr++;
                if (*tmp_ptr == '}')
                    str_to_replace[i + 1] = *tmp_ptr;
            }

            char *env_val = getenv(str_var);
            char *env = NULL;
            if (env_val == NULL)
                env = mx_strdup("\0");
            else
                env = mx_strdup(env_val);

            *str = rep_substr(*str, str_to_replace, env);

            free(env);
            free(str_to_replace);
            free(str_var);
        }
        dollar = strchr(*str, '$');
        if (dollar == tmp_ptr)
            dollar = strchr(++dollar, '$');
    }

    dollar = strrchr(*str, '$');
    if (!dollar)
        return 0;

    // Loop for other simple substitutions like $(COMMANDS)
    while (dollar != NULL) {
        if (*(dollar + 1) != '(') {
            dollar = strchr(++dollar, '$');
            continue;
        }
        /*if (mx_count_substr(dollar, ")") != mx_count_substr(dollar, "(")) {
            //mx_printerr("ush: unmatched: )\n");
            break;
        }*/
        char *tmp_ptr = dollar;
        int ptr_len = mx_strlen(dollar) + 2;
        char *str_to_replace = mx_strnew(ptr_len);
        char *str_var = mx_strnew(ptr_len);

        str_to_replace[0] = *tmp_ptr;
        str_to_replace[1] = *(tmp_ptr + 1);
        tmp_ptr += 2;
        for (int i = 2, j = 0; *tmp_ptr != ')'; i++, j++) {
            str_to_replace[i + 1] = '\0';
            str_var[j + 1] = '\0';

            str_to_replace[i] = *tmp_ptr;
            if (*tmp_ptr != ')')
                str_var[j] = *tmp_ptr;
            tmp_ptr++;
            if (*tmp_ptr == ')')
                str_to_replace[i + 1] = *tmp_ptr;
        }


        char *income_command = mx_strnew(mx_strlen(str_var));
        for (int j = 0; str_var[j] != ' ' && str_var[j] != '\0' && str_var[j] != ')'; j++) {
            char c = str_var[j];
            char z = c;
            if (mx_isalpha(c) && c < 97)
                c += 32;
            income_command[j] = z;
            str_var[j] = c;
        }
        char *command = get_exe_command(str_var);
        if (mx_strlen(command) == 0) {
            mx_printerr("ush: command not found: ");
            mx_printerr(income_command);
            mx_printerr("\n");
        }
        FILE *fp;
        char *strrep = mx_strnew(PATH_MAX);
        fp = popen(command, "r");
        if (fp == NULL) {
            mx_printerr("Failed to run command\n");
            return 1;
        }
        char *strrep_ptr = strrep;
        while (fgets(strrep_ptr, PATH_MAX, fp) != NULL) {
            strrep_ptr = strrep + mx_strlen(strrep);
            strrep[mx_strlen(strrep) - 1] = ' ';
        }
        strrep[mx_strlen(strrep) - 1] = '\0';
        pclose(fp);

        *str = rep_substr(*str, str_to_replace, strrep);
        free(strrep);
        free(command);
        free(income_command);

        free(str_to_replace);
        free(str_var);
        dollar = strrchr(*str, '$');
    }
    return 0;
}

int mx_command_substitution(char **str) {
    char *data = *str;

    int result = handle_back_quotes(data, str);

    if (result) return result;

    result = handle_dollars(data, str);

    if (result) return result;

    return 0;
}

static char *get_exe_command(char *line) {
    char **arr = mx_strsplit(line, ' ');

    char *res = mx_strnew(PATH_MAX);
    char **path_dir = mx_strsplit(getenv("PATH"), ':');

    struct stat sb;
    for (int i = 0; path_dir[i] != NULL; i++) {
        char *cmd = NULL;
        if (arr[0][0] != '/') {
            cmd = mx_strdup(path_dir[i]);
            cmd = mx_strjoin(cmd, "/");
            cmd = mx_strjoin(cmd, arr[0]);
        }
        else 
            cmd = mx_strdup(arr[0]);
        if (lstat(cmd, &sb) != -1) {
            res = mx_strjoin(res, cmd);
            free(cmd);
            res = mx_strjoin(res, line + mx_strlen(arr[0]));
            mx_del_strarr(&path_dir);
            return res;
        }

        if (cmd != NULL) {
            free(cmd);
            cmd = NULL;
        }
    }
    
    return res;
}
