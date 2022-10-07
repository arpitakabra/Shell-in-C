#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<string.h>
#include<dirent.h>
#include<limits.h>
#include<sys/stat.h>


//prints the present working directory
int my_pwd(){

    char cwd[1024];
    
    if(getcwd(cwd, 1024) != NULL){ //getcwd is used to get the present working directory in cwd
        printf("%s\n", cwd);
    } else{
        printf("Could not fetch presenting working directory.\n");
    }
    return(1);
}

