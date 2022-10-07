#include "my_ls.c"
#include "my_pwd.c"
#include "my_cd.c"
#include "my_grep.c"
#include "my_cat.c"
#include "my_mv.c"
#include "my_rm.c"
#include "my_cp.c"
#include "my_mkdir.c"
#include "my_chmod.c"


#ifndef HEADER_H
#define HEADER_H

int my_ls(char ** args);
int my_pwd();
int my_cd(char ** args);
int my_grep(char ** args);
int my_cat(char ** args);
int my_mv(char ** args);
int my_rm(char ** args);
int my_cp(char ** args);
int my_mkdir(char ** args);
int my_chmod(char ** args);

#endif