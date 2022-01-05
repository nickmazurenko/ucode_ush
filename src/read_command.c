#include "ush.h"

void read_command(char **line) {
    *line = mx_strnew(0);
    int size = 0;

    getline(line, &size, stdin);
    char* trimmed_line = mx_strtrim(*line);

    free(*line);

    if (trimmed_line != NULL)
        *line = trimmed_line;
    else
        *line = mx_strnew(0);

    mx_replace_tilda(line);

    if (size == 0) return;
}
