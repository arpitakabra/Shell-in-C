#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<string.h>
#include<dirent.h>
#include<limits.h>
#include<sys/stat.h>



// cp performs the following operations: 
// file to file (new or existing both)
// file to be copied in a directory
// directory to directory copy with copying only files inside 

int my_cp(char ** args){

    //variables
    FILE *f1; //source file
    FILE *f2; //destination file
    DIR *d1; //source directory
    DIR *d2; //destination directory
    char c; //for copying from file
    char * fd;
    char cwd[1024];

    if(args[2]==NULL || !strcmp(args[2], "&")){ //argumeants not proper
        fprintf(stderr, "File/Directory to copy not specified.\n");
        return(1);
    }
    if(args[3]== NULL || !strcmp(args[3], "&")){ //total args are 3, therefore -r not given and not directory to directory copy
        f1 = fopen(args[1], "r");
        if(f1!=NULL){ //if source file opens:
            f2 = fopen(args[2], "w");
            if(f2!=NULL){  //ope destination file

                c = fgetc(f1); //get character from source
                while(c!= EOF){
                    fputc(c, f2); //puts in destination
                    c = fgetc(f1);
                }
                fclose(f2);       //close destination
            }
            else{
                d2 = opendir(args[2]); //if destination is not a file, check if it is a directory
                if(d2!=NULL){ 
                    fd = getcwd(cwd, 1024);
                    strcat(cwd,"/");
                    strcat(cwd,args[2]);
                    strcat(cwd,"/");
                    strcat(cwd,args[1]); //path for creating a new file/opening an existing file with name of source f1 in the destination d2
                    f2 = fopen(fd, "w");
                    c = fgetc(f1); //copy source into destination
                    while(c!=EOF){
                        fputc(c, f2);
                        c = fgetc(f1);
                    }
                    fclose(f2); //close destination file and directory
                    closedir(d2);
                    free(fd);
                }
                else{
                    fprintf(stderr, "Cannot copy file %s\n", args[1]); //if not even a directory, give error
                    return(1);
                }
            }
            fclose(f1);  //close f1
        } else{
            fprintf(stderr, "Cannot copy from file %s\n", args[1]); //if f1 does not open, give error
            return(1);
        }
    } else if(!(strcmp(args[1],"-r")) && args[3]!=NULL || !strcmp(args[4], "&")){ // -r argument and total 4 arguments cp -r dir1 dir2
        
        d2 = opendir(args[3]);  //if dir2 destination directory does not exist, create and open
        if (!d2){
            if(mkdir(args[3], 0777)){
                d2 = opendir(fd);
            } 
        }   
        char *pathname2 = malloc(strlen(args[2]) + strlen(args[3])+2); //dir1 is to be copied to dir2
        strcpy(pathname2, args[3]);                                    //therefore create path as dir2/dir1 
        strcat(pathname2, "/");
        strcat(pathname2, args[2]);

        DIR * dest = opendir(pathname2);  //open the final destination folder as dir2/dir1, if not existing create and open
        if(!dest){
            mkdir(pathname2, 0777);
            dest = opendir(pathname2);
        }
        if(dest){
            d1 = opendir(args[2]);  //open source directory
            struct dirent *d;
            struct stat info;
            while((d = readdir(d1))!=NULL){  //read from source dir1 and copy only files to dir2/dir1
                
                if(!stat(d->d_name, &info)){
                    if(S_ISREG(info.st_mode)){ //check if file in dir1, open
                        f1 = fopen(d->d_name, "r");
                        char * path = malloc(strlen(args[2])+strlen(args[3])+strlen(d->d_name)+4); //path for the same file in dir2/dir1
                        strcpy(path, pathname2);
                        strcat(path, "/");
                        strcat(path, d->d_name);
                        f2 = fopen(path, "w");  //creating a file with path as dir2/dir1/file name
                        c = fgetc(f1);
                        while(c!=EOF){    //copying loop
                            fputc(c, f2);
                            c = fgetc(f1);
                        }
                        fclose(f2);  //close both the files and free path otr for next item
                        fclose(f1);
                        free(path);
                    }
                }
            }
            closedir(d1);  //close source dir1 and dir2/dir1
            closedir(dest);
        }else{
            fprintf(stderr,"Cannot open new created directory %s", pathname2);
        }
        closedir(d2);  
        free(pathname2); //close all files and free ptr paths
    } else if((strcmp(args[1], "-r")) && args[3]!=NULL){  //multiple files and a directory in present. 
        int total_args = 0;  //to count total files present for copying
        while(args[total_args]!=NULL){
            total_args++;
        }
        if(!strcmp(args[total_args-1], "&")){
            total_args = total_args-1;
        }
        d2 = opendir(args[total_args-1]);  //open the destination directory 
        if(d2!=NULL){
            int n = 1;
            while(n<(total_args-1)){  //while args point to source dir itself run in the loop
                f1 = fopen(args[n], "r"); //open file= to be copied 
                if(f1!=NULL){
                    char * path = malloc(strlen(args[total_args-1])+ strlen(args[n])); //creating the file with same name in dir2
                    strcpy(path,args[total_args-1]);
                    strcat(path, "/");
                    strcat(path, args[n]);
                    
                    if((f2=fopen(path,"w"))!=NULL){ //create or open the file with same name as args[n] in the destination folder
                        c = fgetc(f1);
                        while(c!=EOF){  //copying
                            fputc(c, f2);
                            c = fgetc(f1);
                        }
                        fclose(f2);
                    }else{
                        fprintf(stderr, "File creation not succesful.\n"); //error if file cannot be created in destination directory
                    }
                    free(path);
                    fclose(f1);

                } else{
                    fprintf(stderr, "Cannot copy from %s\n", args[n]); //error if source final cannot be opened
                }
                n++;
                
            }
        } else{
            fprintf(stderr, "Specified source directory cannot be accessed.\n");
        }
    }
    return(1);
}

