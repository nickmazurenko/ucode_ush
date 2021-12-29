#include "ush.h"

int main() {
    
    char *buffer = mx_strnew(1);
    size_t size = 0;
    getline(&buffer, &size, stdin);
    mx_printstr(buffer);
    
}
