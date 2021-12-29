#pragma once 

#include "expression.h"
#include "libmx.h"


typedef struct s_pwd_flag {
    bool l_flag;
    bool p_flag;
    bool no_flag;
    bool wrong_flag;
}              t_pwd_flag;

t_pwd_flag *create_pwd_flag();

void find_pwd_flags(t_pwd_flag *flag, char **argv, int argc);



char *get_pwd();
int clear_pwd(t_pwd_flag *flag);
