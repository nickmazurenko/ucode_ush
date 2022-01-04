#include "loop.h"

int main(void) {
    initialise_dirs();

    loop();

    free_dirs();
    return 0;
}


