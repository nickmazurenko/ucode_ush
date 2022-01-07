#ifndef ECHO_H
#define ECHO_H

#include "libmx.h"

typedef struct s_echo_flags
{
    bool is_n_flag;
    bool is_e_flag;
    bool is_E_flag;
    bool is_no_flag;
}              t_echo_flags;

int clear_echo(t_echo_flags *echo_flag, char **args);
t_echo_flags *create_echo_flags(void);
int find_echo_flags(t_echo_flags *echo_flags, char **args);

#endif /*ECHO_H*/
