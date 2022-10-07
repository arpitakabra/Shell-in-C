#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<string.h>
#include<dirent.h>
#include<limits.h>
#include<sys/stat.h>



//changes the present working directory
int my_cd(char ** args){
    if(args[1]==NULL){ //directory to be changed into not specified
        fprintf(stderr, "Enter a valid path.\n");
    }
    else{
        char *dest =args[1];
        DIR *d = opendir(dest);
        if(d==NULL){ //checking whether the entered directory is present or not
            fprintf(stderr, "Enter a valid directory path.\n");
        }
        else{
            chdir(dest); //command to change the directory
        }
    }
    return(1);
}
