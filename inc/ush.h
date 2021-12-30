#ifndef USH_H
#define USH_H

#include "libmx.h"
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <pwd.h>
#include <string.h>
#include <limits.h>
#include <termios.h>
#include <fcntl.h>
#include <ctype.h>
#include <regex.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <spawn.h>
#include <libgen.h>
#include <wordexp.h>
#include "pwd.h"
#include "expression.h"
#include "export.h"
#include "interpreter.h"
#include "loop.h"

int pwd(t_expression* expression);
int export(t_expression* expression);

#endif
