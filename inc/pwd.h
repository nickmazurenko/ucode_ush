#pragma once 

#include "ush.h"
#define IS_LPWD_FLAG(flag) (strcmp(flag, "-L") == 0 ? false : true)
#define IS_PPWD_FLAG(flag) (strcmp(flag, "-P") == 0 ? false : true)


char *get_pwd();
int clear_pwd(char *flag);
