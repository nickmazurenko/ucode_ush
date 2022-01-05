#include "ush.h"
#include "utils.h"

static char *get_exe_command(char *line);

int check_pipe_commands(char** commands, char **str, char* echo_command, char* echo_command_copy) {
    for (int command_index = 0; commands[command_index] != NULL; command_index++) {
        char *command = get_exe_command(echo_command);
        FILE *fp;
        char *replace_value = mx_strnew(PATH_MAX);
        fp = popen(command, "r");
        if (fp == NULL) {
            mx_printerr("Failed to run command\n");
            return 1;
        }
        char *replace_value_ptr = replace_value;
        while (fgets(replace_value_ptr, PATH_MAX, fp) != NULL) {
            replace_value_ptr = replace_value + mx_strlen(replace_value);
            replace_value[mx_strlen(replace_value) - 1] = ' ';
        }
        replace_value[mx_strlen(replace_value) - 1] = '\0';
        pclose(fp);

        *str = mx_strrep(*str, echo_command_copy, replace_value);
        free(replace_value);
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
        char *tmp_dollar = dollar;

        if (*(dollar + 1) != '(' && *(dollar + 1) != '{') {
            int dollar_len = mx_strlen(dollar) + 2;
            char *replaced_str = mx_strnew(dollar_len);
            char *variable_name = mx_strnew(dollar_len);

            replaced_str[0] = *tmp_dollar;
            tmp_dollar++;
            for (int replaced_index = 1, variable_name_index = 0; *tmp_dollar != '\0' && *tmp_dollar != ' ' && *tmp_dollar != '$'; replaced_index++, variable_name_index++) {
                if (*tmp_dollar != '?') {
                    if (!mx_isalpha(*tmp_dollar))
                        break;
                }
                replaced_str[replaced_index + 1] = '\0';
                variable_name[variable_name_index + 1] = '\0';

                replaced_str[replaced_index] = *tmp_dollar;
                variable_name[variable_name_index] = *tmp_dollar;
                if (variable_name[0] == '?')
                    break;
                tmp_dollar++;
            }

            if (variable_name[0] == '?') {
                char *status = mx_itoa(t_dirs_to_work.exit_status);
                *str = rep_substr(*str, replaced_str, status);
                free(status);
                free(replaced_str);
                free(variable_name);
                dollar = strchr(*str, '$');
                continue;
            }

            char *get_env_value = getenv(variable_name);
            char *variable_value = NULL;
            if (get_env_value == NULL)
                variable_value = mx_strdup("\0");
            else
                variable_value = mx_strdup(get_env_value);

            *str = rep_substr(*str, replaced_str, variable_value);

            free(variable_value);
            free(replaced_str);
            free(variable_name);
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
        char *tmp_dollar = dollar;
        if (*(dollar + 1) == '{') {
            int dollar_len = mx_strlen(dollar) + 2;
            char *replaced_str = mx_strnew(dollar_len);
            char *variable_name = mx_strnew(dollar_len);

            replaced_str[0] = *tmp_dollar;
            replaced_str[1] = *(tmp_dollar + 1);
            tmp_dollar += 2;
            for (int replaced_str_index = 2, variable_name_index = 0; *tmp_dollar != '}'; replaced_str_index++, variable_name_index++) {
                replaced_str[replaced_str_index + 1] = '\0';
                variable_name[variable_name_index + 1] = '\0';

                replaced_str[replaced_str_index] = *tmp_dollar;
                if (*tmp_dollar != '}')
                    variable_name[variable_name_index] = *tmp_dollar;
                if (*tmp_dollar == ' ' || *tmp_dollar == '\0') {
                    mx_printerr("ush: bad substitution\n");
                    free(replaced_str);
                    free(variable_name);
                    t_dirs_to_work.exit_status = 1;
                    return -1;
                }
                tmp_dollar++;
                if (*tmp_dollar == '}')
                    replaced_str[replaced_str_index + 1] = *tmp_dollar;
            }

            char *get_env_value = getenv(variable_name);
            char *variable_value = NULL;
            if (get_env_value == NULL)
                variable_value = mx_strdup("\0");
            else
                variable_value = mx_strdup(get_env_value);

            *str = rep_substr(*str, replaced_str, variable_value);

            free(variable_value);
            free(replaced_str);
            free(variable_name);
        }
        dollar = strchr(*str, '$');
        if (dollar == tmp_dollar)
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
        char *tmp_dollar = dollar;
        int dollar_len = mx_strlen(dollar) + 2;
        char *replaced_str = mx_strnew(dollar_len);
        char *variable_name = mx_strnew(dollar_len);

        replaced_str[0] = *tmp_dollar;
        replaced_str[1] = *(tmp_dollar + 1);
        tmp_dollar += 2;
        for (int replaced_str_index = 2, variable_name_index = 0; *tmp_dollar != ')'; replaced_str_index++, variable_name_index++) {
            replaced_str[replaced_str_index + 1] = '\0';
            variable_name[variable_name_index + 1] = '\0';

            replaced_str[replaced_str_index] = *tmp_dollar;
            if (*tmp_dollar != ')')
                variable_name[variable_name_index] = *tmp_dollar;
            tmp_dollar++;
            if (*tmp_dollar == ')')
                replaced_str[replaced_str_index + 1] = *tmp_dollar;
        }


        char *income_command = mx_strnew(mx_strlen(variable_name));
        for (int variable_name_index = 0; variable_name[variable_name_index] != ' ' && variable_name[variable_name_index] != '\0' && variable_name[variable_name_index] != ')'; variable_name_index++) {
            char symbol = variable_name[variable_name_index];
            char symbol_copy = symbol;
//            if (mx_isalpha(symbol) && symbol < 97)
//                symbol += 32;
            symbol = mx_char_to_upper(symbol);

            income_command[variable_name_index] = symbol_copy;
            variable_name[variable_name_index] = symbol;
        }
        char *command = get_exe_command(variable_name);
        if (mx_strlen(command) == 0) {
            mx_printerr("ush: command not found: ");
            mx_printerr(income_command);
            mx_printerr("\n");
        }
        FILE *fp;
        char *replace_value = mx_strnew(PATH_MAX);
        fp = popen(command, "r");
        if (fp == NULL) {
            mx_printerr("Failed to run command\n");
            return 1;
        }
        char *replace_value_ptr = replace_value;
        while (fgets(replace_value_ptr, PATH_MAX, fp) != NULL) {
            replace_value_ptr = replace_value + mx_strlen(replace_value);
            replace_value[mx_strlen(replace_value) - 1] = ' ';
        }
        replace_value[mx_strlen(replace_value) - 1] = '\0';
        pclose(fp);

        *str = rep_substr(*str, replaced_str, replace_value);
        free(replace_value);
        free(command);
        free(income_command);

        free(replaced_str);
        free(variable_name);
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
