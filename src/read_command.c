#include "ush.h"
#include "console_input_handler.h"

void read_command(char **line) {
    *line = mx_strnew(0);
    size_t size = 0;

    getline(line, &size, stdin);
    size = mx_strlen(*line);
    if(size > 0) {
        (*line)[size - 1] = '\0';
    }
    char* trimmed_line = mx_strtrim(*line);
//    if((*line)[size] == EOF) {
//        mx_printerr("hello\n");
//        mx_printerr("\n");
//    }
    free(*line);

    if (trimmed_line != NULL)
        *line = trimmed_line;
    else
        *line = mx_strnew(0);

    mx_replace_tilda(line);
    //TODO write error
    if (size == 0) {
        handle_ctrl_d(SIGKILL);
    }
}
