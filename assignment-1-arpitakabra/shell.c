#include "header.h"
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<string.h>
#include<dirent.h>
#include<limits.h>
#include<sys/stat.h>

//tokenizing user input
#define DELIM " \t\r\n\a" //tokenizing delimeters inlcude tab, space, next line
char **tokenize(char *input){
    int bufsize = 64; //intial buffer size to store tokenized parameters
    int i = 0;
    char **tokens = malloc(bufsize * sizeof(char*)); //tokens is an array of pointers pointing to to each token
    char *token;

    token = strtok(input, DELIM); //tokenizing
    while(token!=NULL){
        tokens[i] = token;
        i++;
         if(i>=bufsize){ //if tokens exceed token buffer size, allocate more space
            bufsize += 64;
            tokens = realloc(tokens, bufsize * sizeof(char*));
        }
        token = strtok(NULL, DELIM); //last pointer to NULL
    }
    tokens[i] = NULL;
    return tokens; //returning tokens
}


//reading the user input
char *read_input(void){  

    int bufsize = 1024; //user input size is fixed
    char *buf = malloc(sizeof(char)*bufsize);
    if(!buf){
        fprintf(stderr, "Cannot allocate memory\n"); //if buffer is not allocated at all
        exit(EXIT_FAILURE);
    }
    while(fgets(buf, bufsize, stdin)!=0){ //fgets is used to read input from stdin in the buf
        if(feof(stdin) || "\n"){ //if enter pressed, stop reading input and return buf
            break;
        }
    }
    return buf;
}


int launch_shell(char ** args){ //if any built in library is called, fork and call exec in the child process to run built in commands
    pid_t pid;
    pid = fork();
    
    //counting the total args in args, and making the last arg NULL if has originally &
    while(args[total_args]!=NULL){
        total_args++;
    }
    if(!strcmp(args[total_args-1], "&")){
            args[total_args-1]= NULL;
    }
    
    pid = fork(); //creating a child process
 
    if(pid<0){
        fprintf(stderr, "Cannot Fork.");
    } else if (pid == 0){
        if(execvp(args[0], args)==-1){
            fprintf(stderr, "Could not execute the input command.\n");
        }
        exit(0);
    }else{
        wait(NULL); //wait till the child exits
    }
    return(1);
}


//defining all possible built in commands
int run_input(char** args){
    int total_commands = 10; //total commands not to be called from built in binaries
    int i =0;
    int switch_case = 0;
    char * list[total_commands];
    int built_in = 2; 
    
    //counting the total args:
    int total_args = 0;
    while(args[total_args]!=NULL){
        total_args++;
    }
    
   //list of commands 
    list[0] = "ls";
    list[1] = "grep";
    list[2] = "cat";
    list[3] = "cp";
    list[4] = "mv";
    list[5] = "rm";
    list[6] = "chmod";
    list[7] = "cd";
    list[8] = "pwd";
    list[9] = "mkdir";

    if(args[0]== NULL){ //if no argument is passed return back to the main shell loop
        return(1);
    } else{
        // int built_in = 1; //to check whether built-in bianries are called or not
        for(i = 0; i<total_commands; i++){ //if the input matches with commands in the list execute from self written commands, else launch_shell
            if(strcmp(args[0], list[i]) == 0){
                switch_case = i+1;
                built_in = 0;
                break;
            }
        }
    }
    //switch cases, the functions are defined in separate c files included in header.h
    switch (switch_case){
        case 1:
            my_ls(args);
            break;
        case 2:
            my_grep(args);
            break;
        case 3:
            my_cat(args);
            break;
        case 4:
             my_cp(args);
             break;
        case 5:
            my_mv(args);
            break;
        case 6:
            my_rm(args);
            break;
        case 7:
            my_chmod(args);
            break;
        case 8:
            my_cd(args);
            break;
        case 9:
            my_pwd(args);
            break;
        case 10:
            my_mkdir(args);
            break;
        default:
            break;
    }

    if(built_in!=0){ //if self-made binary not called, invoke the function from built in libraries
        if(!strcmp(args[total_args-1], "&")){ //if & present, remove it
            args[total_args-1]== NULL;
        }
        launch_shell(args);
    }
    else{
        return(1);
    }
}

//run command is called from the main shell loop to check whether & parameter is given or not
int run(char ** args){
    int status;
    int n = 0;//to count the number of arguments
    if(args[0]==NULL){
        return(1);
    }
    while(args[n]!=NULL){
        n++;
    }
    if(!strcmp(args[n-1], "&")){ //if last argument is &, then only fork into a child process and call run_input
        int pid = fork();
        if(pid<0){
            fprintf(stderr, "Could not execute & operation");
        } else if(pid == 0){
            run_input(args); //the child process executes the command now
            exit(0);
        } else{
            waitpid(pid,&status,0); //waitpid in this case just returns the child status without affecting parent execution
            return(1);
        }

    }
    else{
        run_input(args); //if no & passed, simply run_input
        return(1);
    }
    // return(1);
}

//shell loop is called by the main function and runs till exit input is given by the user or the program  is closed
void loop(void){
    int condition =1 ;
    char* input;
    char** args;

    do{
        printf(">");
    
        input = read_input(); //input taken
        args = tokenize(input); //tokenized
        if(args[0]==NULL){
            condition = 1;
        } else if(!strcmp(args[0],"exit")){ //if exit passed, exited and return to terminal
            exit(0);
        } else{
            condition = run(args); //run args, check for & and accordingly run run_input
            free(input);
            free(args);
        }
    
    } while(condition); 
}

//main loop
int main(int argc, char **argv){
    
    loop();

}
