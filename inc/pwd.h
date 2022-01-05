
#ifndef PWD_H
#define PWD_H

#include "libmx.h"


typedef struct s_pwd_flags
{
    bool is_l_flag;
    bool is_p_flag; //default
}              t_pwd_flags;

int clear_pwd(t_pwd_flags *flags);
t_pwd_flags *create_pwd_flags(void);
int find_pwd_flags(t_pwd_flags *pwd_flag, char **flags);


#endif //UCODE_USH_PWD_H
