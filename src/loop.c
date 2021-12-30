#include "loop.h"

int loop() {
    
    int in_loop = isatty(0);
    int error_code = 0;
    do {
        char* buffer = mx_strnew(1);
        size_t size = 0;
        if (in_loop) {
            mx_printstr("u$h> ");
        }
        getline(&buffer, &size, stdin);
        interpret(buffer);
        free(buffer);

    } while(in_loop);

    return error_code;
}
