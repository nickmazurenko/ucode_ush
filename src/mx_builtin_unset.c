#include "../inc/ush.h"

int mx_builtin_unset(const char *name) {
    return unsetenv(name);
}
