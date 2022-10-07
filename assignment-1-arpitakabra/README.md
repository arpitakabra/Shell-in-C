[![Work in Repl.it](https://classroom.github.com/assets/work-in-replit-14baed9a392b3a25080506f3b7b6d57f295ec2978f6f33ec97e36a161684cbe9.svg)](https://classroom.github.com/online_ide?assignment_repo_id=3009113&assignment_repo_type=AssignmentRepo)
# Assignment1
For all the commands, a header file, header.h is created and included in shell.c. We are using the make file to run the shell. To build run:
`make build
`
The output shall print that shell.c is build. To run the shell run:
`make run`

The shell starts executing with '>' symbol. 

To exit the shell, give the input: `exit`.

To clean shell, run:
`make clean`

The shell runs in a loop until forced exit or exit is given as input. The basic loop of shell involves taking user input until pressed enter. If the input matches with any of the binaries self-build, it is executed by commands specified in shell.c through header file, else it forks to run the inbuilt binaries. 
-The user input is tokenized by space character. 

Switch case is used to run the self-builds commands in the shell.c
