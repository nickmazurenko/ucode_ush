
#ifndef USH_ENV_H
#define USH_ENV_H

#include "libmx.h"

typedef struct s_env_flags
{
    bool is_i_flag;
    bool is_u_flag;
    bool is_p_flag;
}              t_env_flags;

int clear_env(t_env_flags *env_flag, char **data);
t_env_flags *create_env_flags(void);
int find_env_flags(t_env_flags *env_flag, char **param);
int clear_unset(const char *name);
int unset_check_args(char **args);

#endif //USH_ENV_H
