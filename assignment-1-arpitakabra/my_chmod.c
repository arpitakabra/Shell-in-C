#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<string.h>
#include<dirent.h>
#include<limits.h>
#include<sys/stat.h>

//changes the specified file or directory permissions
int my_chmod(char ** args){
    if(args[2]==NULL || args[1]==NULL){ //only chmod or chmod mode is passed as argument
        fprintf(stderr, "mode to be changed to or file does not specified./n");
    } else{
        int mode = atoi(args[1]); //changes the specified mode to integer value
        if(!chmod(args[2], mode)){ //chmod changes file permissions
            printf("Succesfully changed the mode of %s.\n", args[2]);
        } else{   //gives error if the permission in argumet does not match with parent directory, or the file cannot be accessed.
            fprintf(stderr, "Cannot change the mode of %s.\n", args[2]);
        }

    }
    return(1);
}

