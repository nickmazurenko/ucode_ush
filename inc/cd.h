
#ifndef CD_H
#define CD_H

#include "libmx.h"


typedef struct s_cd_flags
{
    bool is_s_flag;
    bool is_p_flag;
}              t_cd_flags;

int clear_cd(char **params, t_cd_flags *flags);
t_cd_flags *create_cd_flags(void);
int find_cd_flags(t_cd_flags *data, char **flags);


#endif //UCODE_USH_CD_H
