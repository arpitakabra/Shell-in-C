#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<string.h>
#include<dirent.h>
#include<limits.h>
#include<sys/stat.h>


// rm deletes a single file, multiple files or the entire directory upon being called with proper arguments

//recursive call to delete directory
void recursive_delete(const char * path){
    struct dirent *dir;
    DIR * d;
    d = opendir(path);
    if(d){  //if input is actually a directory

        while((dir = readdir(d))!=NULL){ //check contents of path directory

            char * next_path = malloc(strlen(path)+strlen(dir->d_name)+2); //to refer to the item to be deleted
            if(next_path){
                struct stat sb;
                strcpy(next_path, path);
                strcat(next_path, "/");
                strcat(next_path, dir->d_name); //next_path has the path of the concerned file or subdirectory 
                if((strcmp(dir->d_name,".")!=0 && strcmp(dir->d_name, "..")!=0)){ //do not interfere with present and parent directory
                    if(!stat(next_path, &sb)){  
                        if(S_ISDIR(sb.st_mode)){  //if directory is read, recursively call this same function
                            recursive_delete(next_path);
                        }
                        else{
                            remove(next_path); //else remove the file
                        }
                    }
                }
            }
            free(next_path); 
        }
        closedir(d);
        rmdir(path); //after deleting the directory contents, delete the directory itself
        return;
    } else{
        fprintf(stderr, "Cannot delete %s directory.\n", path);
    }
}

int my_rm(char ** args){
    if(!(strcmp(args[1], "-r"))){ // -r argument represents recursive call for deleting direcory
        recursive_delete(args[2]);
    } else{
        int n = 1;
        while(args[n]!=NULL && strcmp(args[n], "&")!=0){ //rm can delete multiple files also
            struct stat f;
            stat(args[n], &f);
            if(S_ISREG(f.st_mode)){ //stat struct mode checks whether args[n] is file or not
                remove(args[n]);    //removing/deleting file
            } else{
                fprintf(stderr, "Cannot delete the file %s.\n", args[n]);
            }
            n++;
        }
    }
    return(1);
}


