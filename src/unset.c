#include "ush.h"

int clear_unset(const char *name) {
    return unsetenv(name);
}
