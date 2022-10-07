#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<string.h>
#include<dirent.h>
#include<limits.h>


//prints the content of the file passed in the argument

//variables 
#define MAX_FILE_LEN 1024 //maximum size of length in the file
char* line;
size_t linesize = MAX_FILE_LEN;
int no_files = 1; //if more than one file is passed in the arguments, print all

int my_cat( char ** args){

    if(args[1]==NULL || !strcmp(args[1], "&")){ //if no input file is provided
        fprintf(stderr, "No input file provided.");
        return(1);
    } else{
        while(args[no_files]!=NULL && (strcmp(args[no_files], "&")!=0)){ //while the argument points to null, print all files one by one

            FILE* f = fopen(args[no_files], "r");
            if(!f){ //checks if the file is valid or not, 
                fprintf(stderr, "File %s cannot be opened.", args[no_files]);
                return(1);
            }
            printf("\n\nPrinting for file %s\n\n", args[no_files]);
            while(!feof(f)){
                if(getline(&line, &linesize, f)){ //prints the file content until end of file is reached
                   printf("%s", line);
                }           
            }
            fclose(f); //close files and free buffers
            free(line);
            no_files++;
        }
    }
    return(1);
}
