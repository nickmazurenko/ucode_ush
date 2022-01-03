#include "../inc/ush.h"

void mx_read_command(char **line) {
    *line = mx_strnew(PATH_MAX);

    //read one line
    int read = mx_input(line, PATH_MAX);
    
    mx_replace_tilda(line);
    
    if (read == 0) return;
}
