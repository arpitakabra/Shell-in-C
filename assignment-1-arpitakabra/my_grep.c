#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<string.h>
#include<dirent.h>
#include<limits.h>


//searches for a pattern in the specified file


int my_grep(char ** args){

    //variales 
    // #define MAX_FILE_LEN 1024
    
    size_t linesize = 1024; //line size to be considered for each line in a file
    char* line = malloc(linesize);

    if(args[1]==NULL || !strcmp(args[1], "&")){
        fprintf(stderr, "No pattern and files provided\n"); //o pattern or file is provided as an argument
        return(1);
        

    } else if(args[2] == NULL || !strcmp(args[2], "&")){ //input to be taken from the user rather than from a file
        char* pattern = args[1];
        // if(strspn(args[1],".")){
        //     fprintf(stderr,"No lines matched");
        //     return(1);
        // }
        int lin_no = 1;
        while((getline(&line, &linesize, stdin))!=-1){ //reading the input line
            if(strstr(line, pattern)){ //comparing the line and the pattern inserted by the user
                 //the line containing the pattern is printed as soon as pattern is found
                printf("%s", line);
            }
            lin_no++;
        }
        
    } else if(args[3] == NULL || !strcmp(args[3], "&")){ //a single file is present to be searched for the pattern
        // if(strspn(args[1],".")){
        //     fprintf(stderr,"No lines matched");
        //     return(1);
        // }
        char* pattern = args[1];

        FILE* f = fopen(args[2], "r");
        if(!f){ //check whether the file is valid or not else return
            fprintf(stderr, "File %s cannot be opened.\n", args[2]);
            return(1);
        }
        int lin_no = 1;
        while(!feof(f)){ //until end of file is reached read from file line by line
            if(getline(&line, &linesize, f)){
                if(strstr(line, pattern)){ //if pattern matched, print pattern with line number
                    printf("%d %s %s", lin_no, args[2], line);
                }
                //increasing line number
            }
            lin_no++; 
        }
        free(line);
        fclose(f);
        

    } else{ //more than one file is present, filename in which pattern is found atleast once is printed
        int no_files = 2;
        char* pattern = args[1];
        while(args[no_files]!=NULL && strcmp(args[no_files], "&")!=0){ //while the arguments do not point to null

            FILE* f = fopen(args[no_files], "r"); //open file in pointed by args[no_files] starting from argument indexed at 2
            int lin_no = 1; //line counter

            if(!f){ //check if the file can be accessed or not
                fprintf(stderr, "File %s cannot be opened.\n", args[no_files]);
                return(1);
            }
            while(!feof(f)){ //while end of file is reached
                if(getline(&line, &linesize, f)){ //get the line
                    if(strstr(line, pattern)){ //compare the line with pattern
                        printf("%s\n", args[no_files]);
                        break; //if pattern found even once, print the current file and move to next, if next exists in the arguments
                    }
                    lin_no++;
                }
                
            }
            fclose(f);
            free(line);
            
            no_files++; //incrementing argument pointer
        }
    }
    return(1);
}

