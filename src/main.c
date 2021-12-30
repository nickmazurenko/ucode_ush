#include "ush.h"

// int main(int argc, char **argv) {
//     argc = 0;
//     interpret(argv[1]);
// }

int main() {
    char *buffer = mx_strnew(1);
    size_t size = 0;
    getline(&buffer, &size, stdin);
    interpret(buffer);
    free(buffer);
}
