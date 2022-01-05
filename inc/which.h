
#ifndef WHICH_H
#define WHICH_H

#include "libmx.h"

// WHICH block
//===============================================================
typedef struct s_which_flags
{
    bool is_a_flag;
    bool is_s_flag;
}              t_which_flags;

int clear_which(t_which_flags *which_flag, char **args);
t_which_flags *create_which_flags(void);
int find_which_flags(t_which_flags *which_flag, char **args);
//===============================================================

#endif //WHICH_H
