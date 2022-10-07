#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<string.h>
#include<dirent.h>
#include<limits.h>
#include<sys/stat.h>


//ls command lists all the files and subdirectories in the called directory
int my_ls(char ** args){
    //variables
    struct dirent *d;
    int n=1; //to list contents of multiple directories
    DIR*p; 
    FILE *f;
    char * pathname = "."; //represents current working dierctory and ".." represents the parent directory of "."
    
    if(args[1]==NULL || !strcmp(args[1], "&")){ //only ls is given as argument means listing current working directory
        p = opendir(pathname); //pathname will be current working directory "."
            if(p!=NULL){
                while((d=readdir(p))!=NULL){ //readdir reads through the directory contents
                    if(strcmp(d->d_name,".")!=0 && strcmp(d->d_name, "..")!=0){ //not to list them as they are not present under the directory
                        printf("%s\n", d->d_name);
                    }
                }
            }closedir(p);
    }else{
        while(args[n]!=NULL && strcmp(args[n], "&")!=0){ //if a single or multiple directory are listed with ls, list the contents one by one
            pathname = args[n];
            p = opendir(pathname);
            if(p!=NULL){ //checking if the argument is directory only or not
                while((d=readdir(p))!=NULL){
                    if(strcmp(d->d_name,".")!=0 && strcmp(d->d_name, "..")!=0){
                        printf("%s\n", d->d_name);
                    }
                }
                n++;
            }else if((f=fopen(pathname, "r"))!=NULL){ //if not directory, just print the file if it is a file
                printf("%s\n", pathname);
            }else{
                fprintf(stderr, "Invalid Argument to ls.\n"); //if not accessible type of document give error
            }
            
        } 
    }
    return(1);
}

