#pragma once
#define _GNU_SOURCE
#define _SVID_SOURCE
#define _POSIX_C_SOURCE 200809L //for setenv() and unsetenv()
#define _XOPEN_SOURCE 500 // for realpath() on Linux
#include <sys/wait.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/dir.h>
#include <signal.h>
#include <dirent.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <limits.h>

#include <pwd.h>
#include <time.h>
#include <grp.h>
#include <sys/acl.h>
#include <sys/xattr.h>
#include <sys/param.h>
#include <termios.h>
#include <term.h>
#include <curses.h>
#include <termios.h>

#include "libmx.h"

#define TILDA "~\0"


extern char **environ;
void initialise_dirs(void);
void free_dirs(void);
char *mx_strrep(char *str, char *substr, char *replace);
void mx_replace_tilda(char **str);
int mx_command_substitution(char **str);
void mx_apply_escapes(char ***arr);
void mx_create_process(char *command, char **parameters, char *cmd);
char *rep_substr(char *str, char *substr, char *new_str);

struct s_dirs_to_work
{
    char *PWD;
    char *OLDPWD;
    char *HOME;
    int exit_status;
    char *PATH;
}      t_dirs_to_work;

typedef struct s_jobs
{
    int pid;
    int job_id;
    char *cmd;
    struct s_jobs *next;
    struct s_jobs *prev;
}      t_jobs;
t_jobs *jobs;
t_jobs *jobs_new_node(int pid, char *cmd);
void jobs_clear(t_jobs **head);
void jobs_push_back(t_jobs **list, t_jobs **data);
int jobs_remove(t_jobs **head, int pid);

void mx_print_prompt(void);
int mx_input(char **str, int win_len);
void mx_read_command(char **line);
int mx_execute_builtin(char *command, char **params, char ***commands_arr, int i);

// CD block
//===============================================================
typedef struct s_cd_flags
{
    bool is_s_flag;
    bool is_p_flag;
}              t_cd_flags;

int clear_cd(char **params, t_cd_flags *flags);
t_cd_flags *create_cd_flags(void);
int find_cd_flags(t_cd_flags *data, char **flags);
//===============================================================


// ENV block
//===============================================================
typedef struct s_env_flags
{
    bool is_i_flag;
    bool is_u_flag;
    bool is_p_flag;
}              t_env_flags;

int clear_env(t_env_flags *env_flag, char **data);
t_env_flags *create_env_flags(void);
int find_env_flags(t_env_flags *env_flag, char **param);
//===============================================================


// PWD block
//===============================================================
typedef struct s_pwd_flags
{
    bool is_l_flag;
    bool is_p_flag; //default
}              t_pwd_flags;

int clear_pwd(t_pwd_flags *flags);
t_pwd_flags *create_pwd_flags(void);
int find_pwd_flags(t_pwd_flags *pwd_flag, char **flags);
//===============================================================

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

// ECHO block
//===============================================================
typedef struct s_echo_flags
{
    bool is_n_flag;
    bool is_e_flag;
    bool is_E_flag;
}              t_echo_flags;

int clear_echo(t_echo_flags *echo_flag, char **args);
t_echo_flags *create_echo_flags(void);
int find_echo_flags(t_echo_flags *echo_flags, char **args);
//===============================================================

int mx_builtin_unset(const char *name);
int mx_unset_check_param(char **data);

//===============================================================
int mx_atoi(char *);
