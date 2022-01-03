#include "ush.h"

void mx_read_command(char **line) {
    *line = mx_strnew(0);
    int size = 0;

    getline(line, &size, stdin);
    char* trimmed_line = mx_strtrim(*line);

    free(*line);
    *line = trimmed_line;

    mx_replace_tilda(line);
    
    if (size == 0) return;
}
