#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<string.h>
#include<dirent.h>
#include<limits.h>

//moves files from into directory, renames directories and files

int my_mv(char ** args){

    char *source = args[1]; //source ptr
    char *dest = args[2]; //destination ptr
    char cwd[1024];
    FILE *f;

    if(args[2]==NULL || args[1]==NULL){ //destination or both source and destination not specified
        fprintf(stderr, "Enough number of inputs not provided.\n");
        exit(1);
    } else if(args[3] == NULL || !strcmp(args[3], "&")){ // file rename, directory rename or single file to be moved to the specified directory
        DIR *d = opendir(dest); //check whether destination path is a file or directory
        if(d== NULL){ //destination path is a file
            if(rename(source, dest)!=0){
                fprintf(stderr, "Destination file not found.\n");
            }
        } else{ //making a new file in current working directory

            char *c;
            c = getcwd(cwd, 1024); //path of current working directory
            strcat(cwd,"/");
            strcat(cwd,dest);
            strcat(cwd,"/");
            strcat(cwd,source); //final path is current directory/destination directory/source file or directory
            if(rename(source,c)==-1){
                fprintf(stderr, "Could not move source.\n");
                closedir(d);
                exit(1);
            }
            free(c);
        }
    }else{ //multiple files are to be moved
        int n =1;
        while(args[n]!=NULL){ //loop to count the number of files to be moved to the destination directory
            n++;
        }
        if(!strcmp(args[n-1], "&")){ //checking whether & is give or not
            n = n-1;
        }
        dest = args[n-1];
        DIR *d = opendir(args[n-1]);
        if(d==NULL){
            fprintf(stderr, "Wrong source directory entered.\n");
        } else{
            int i = 1;
            while(args[i+1]!=NULL){ //loop runs till just when args[i] points the destination directory
                char *c;
                c = getcwd(cwd, 1024);
                strcat(cwd,"/");
                strcat(cwd,dest);
                strcat(cwd,"/");
                strcat(cwd,args[i]); //final path to move is cwd/destination directory/file in args[i]
                if(rename(args[i],c)==-1){
                    fprintf(stderr, "Could not move file.\n");
                    closedir(d);
                    exit(1);
                }
                i++;
                free(c);
            }
        }
        closedir(d);
    }
    return(1);
}

