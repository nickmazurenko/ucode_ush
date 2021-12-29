#include "ush.h"

int main(int argc, char **argv) {
    
    argc = 0;
    // char *buffer = mx_strnew(1);
    // size_t size = 0;
    // getline(&buffer, &size, stdin);
    interpret(argv[1]);
    // free(buffer);
}
