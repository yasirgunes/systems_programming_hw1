#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define TRUE 1

int main(int argc, char* argv[]) {

    printf("Enter your command.\n\n");

    char input[100]; // command buffer

    char *command; // command buffer
    char *args[10]; // arguments buffer

    // infinite loop to get commands
    while(TRUE) {

        printf("$ ");

        // get the input from user
        fgets(input, 100, stdin);
        input[strlen(input) - 1] = '\0'; // remove the newline character
        
        // tokenize command and args
        command = strtok(input, " ");
        for(int i = 0; i < 10; i++) {
            args[i] = strtok(NULL, " ");

            if(args[i] == NULL) {
                break;
            }
        }

        // trim the leading " and trailing " from the args
        for(int i = 0; i < 10; i++) {
            if(args[i] == NULL) {
                break;
            }
            args[i] = strtok(args[i], "\"");
        }


        // print command and args
        printf("Command: %s\n", command);
        for(int i = 0; i < 10; i++) {
            if(args[i] == NULL) {
                break;
            }
            printf("Arg %d: %s\n", i, args[i]);
        }


        // if command is exit, then exit the program
        if(strcmp(command, "exit") == 0) {
            printf("Exiting the program.\n");
            break;
        }

        // list all the commands can be used

        else if (strcmp(command, "gtuStudentGrades") == 0 && (args[0] == NULL)) {
            // list all the commands can be used
            printf("All available commands will be listed here soon! \n");
        }

        // if command gtuStudentGrades "file.txt"=> create a file named file.txt
        // but it should use a child process to do it via fork() to create a new process

        else if (strcmp(command, "gtuStudentGrades") == 0 && (args[0] != NULL)) {

            // if it has more than 1 argument, print error
            if(args[1] != NULL) {
                printf("Error: Too many arguments.\n");
                continue;
            }

            pid_t pid = fork();
            if (pid == 0) {
                // if the file already exists do nothing else create the file
                if(fopen(args[0], "r") != NULL) {
                    printf("File already exists.\n");
                }
                else {
                    FILE *file = fopen(args[0], "w");
                    if (file == NULL) {
                        printf("Error creating file.\n");
                    }
                    else {
                        printf("File created.\n");
                        fclose(file);
                    }
                }
                exit(0);
            }
            else {
                wait(NULL);
            }
        }

        // if command addStudentGrade "Name Surname" "AA" => add a student with name and surname and grade
        // but it should use a child process to do it via fork() to create a new process

        else if (strcmp(command, "addStudentGrade") == 0 && (args[0] != NULL) && (args[1] != NULL)) {

            // if it has more than 2 arguments, print error
            if(args[2] != NULL) {
                printf("Error: Too many arguments.\n");
                continue;
            }

            pid_t pid = fork();
            if (pid == 0) {
                FILE *file = fopen("gtuStudentGrades.txt", "a");
                if (file == NULL) {
                    printf("Error opening file.\n");
                }
                else {
                    fprintf(file, "%s %s\n", args[0], args[1]);
                    printf("Student added.\n");
                    fclose(file);
                }
                exit(0);
            }
            else {
                wait(NULL);
            }
        }
        
        
    }
    return 0;
}