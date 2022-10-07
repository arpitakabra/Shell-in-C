#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<string.h>
#include<dirent.h>
#include<limits.h>
#include<sys/stat.h>



// creates a new directory
int my_mkdir(char ** args){

    DIR *d;
    
    if(args[1]==NULL || !strcmp(args[1], "&")){ //no argument provided to mkdir
        fprintf(stderr, "Enter a valid directory name.\n");
    } else if((d = opendir(args[1]))!=NULL){   //checking whether the directory already is present or not

        fprintf(stderr, "The entered directory exists.\n");
    } else{
        if((mkdir(args[1], 0777))){           //create a new directory through mkdir call in C
            fprintf(stderr, "created the directory.\n");
            closedir(d);
        }
    }
}
