#include "loop.h"

int main(void) {

    mx_init_global();

    loop();

    mx_free_global();
    return 0;
}


