#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h> // this is for system calls

#define TRUE 1

int main(int argc, char* argv[]) {

    // if the argv[1] is gtuStudentGrades and there is no more argument 
    // print all available commands
    if (strcmp(argv[1], "gtuStudentGrades") == 0 && argc == 2) { 
        printf("Going to print all the available commands soon!\n");
    }

    // if the argv[1] is gtuStudentGrades and file name is given
    if(strcmp(argv[1], "gtuStudentGrades") == 0 && argc == 3) {
        pid_t pid;
        pid = fork();
        if(pid < 0) {
            const char *message = "Fork failed\n";
            write(STDERR_FILENO, message, strlen(message));
            return 1;
        } else if(pid == 0) {
            // child process
            // check if the file exists
            int fd = open(argv[2], O_RDONLY);
            if(fd != -1) {
                printf("File exists!\n");
                exit(0);
            }
            // if not exists create a file using system call

            // int file = open(argv[2], O_WRONLY, O_CREAT, 0644);
            int file = creat(argv[2], 0644);
            printf("File created!\n");
            close(file);
            exit(0);
        } else {
            // parent process
            wait(NULL);
        }
    }

    //add new student grade to the file
    // if the argv[1] addStudentGrade
    if(strcmp(argv[1], "addStudentGrade") == 0) {
        // if there is no more argument
        if(argc == 2) {
            printf("Please enter the student name and grade with this format:\naddStudentGrade \"studentName\" \"studentGrade\" \"file_name.txt\"\n");
        }
        if(argc == 3 || argc == 4) {
            printf("Missing Arguments.\nPlease enter the student name and grade with this format:\naddStudentGrade \"studentName\" \"studentGrade\" \"file_name.txt\"\n");
        }
        if(argc == 5) {
            // if all arguments are given create a child process and add the student grade to the file
            pid_t pid;
            pid = fork();
            if(pid < 0) {
                const char *message = "Fork failed\n";
                write(STDERR_FILENO, message, strlen(message));
                return 1;
            } else if(pid == 0) {
                // child process
                int file = open(argv[4], O_WRONLY | O_APPEND);
                
                if(file == -1) {
                    printf("Error opening file!\n");
                    exit(1);
                }
                // write to file using system call write with using buffer
                char buffer[1024];
                snprintf(buffer, sizeof(buffer), "%s %s\n", argv[2], argv[3]);
                write(file, buffer, strlen(buffer));
                close(file);
                printf("Student grade added to the file: %s\n", argv[4]);
                exit(0);
            } else {
                // parent process
                wait(NULL);
            }
        }
    }

    return 0;
}