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
            printf("Please enter the student name and grade with this format:\naddStudentGrade \"Name Surname\" \"Student Grade (eg. AA)\" \"file_name.txt\"\n");
        }
        if(argc == 3 || argc == 4) {
            printf("Missing Arguments.\nPlease enter the student name and grade with this format:\naddStudentGrade \"Name Surname\" \"Student Grade (eg. AA)\" \"file_name.txt\"\n");
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

    // search for a student in the file
    // if the argv[1] is searchStudent
    if(strcmp(argv[1], "searchStudent") == 0) {
        // if there is no more argument
        if(argc == 2) {
            printf("Please enter the student name and file name with this format:\nsearchStudent \"Name Surname\" \"file_name.txt\"\n");
        }
        if(argc == 3) {
            printf("Missing Arguments.\nPlease enter the student name and file name with this format:\nsearchStudent \"Name Surname\" \"file_name.txt\"\n");
        }
        if(argc == 4) {
            // if all arguments are given create a child process and search for the student in the file
            pid_t pid;
            pid = fork();
            if(pid < 0) {
                const char *message = "Fork failed\n";
                write(STDERR_FILENO, message, strlen(message));
                return 1;
            } else if(pid == 0) {

                // child process
                int file = open(argv[3], O_RDONLY);

                if(file == -1) {
                    printf("Error opening file!\n");
                    exit(1);
                }

                // read from file using system call read with using buffer and search for the student
                // using strncmp to compare the first n characters of the line with the student name

                char line[1024];
                ssize_t n;
                char *lineStart = line;
                while ((n = read(file, lineStart, 1)) > 0) { // Read one character at a time
                    if (*lineStart == '\n' || *lineStart == '\0') { // End of line
                        *lineStart = '\0'; // Null-terminate the line
                        if (strncmp(line, argv[2], strlen(argv[2])) == 0 && line[strlen(argv[2])] == ' ') {
                            printf("Student found: %s\n", line);
                            close(file);
                            exit(0);
                        }
                        lineStart = line; // Reset the line start pointer
                    } else {
                        lineStart++;
                    }
                }
                printf("Student with name: \"%s\" not found in the file.\n", argv[2]);
                close(file);
                exit(0);
            } else {
                // parent process
                wait(NULL);
            }
        }
    }

    // now we should sort the grades in the file
    // The user should be able to sort the student grades in the file.
    // The program should provide options to sort by student name or grade, in ascending or descending order.
    // The command: sortAll “gradest.txt” should print all of the entries sorted by their names.

    if(strcmp(argv[1], "sortAll") == 0) {
        if(argc == 2) {
            // sorting can be made by name or grade in ascending or descending order
            printf("Please enter the command with this format:\nsortAll \"Sort by (name or grade)\" \"Sorting type (+ for ascending, - for descending order)\" \"file_name.txt\"\n");
        }
        else if(argc == 3 || argc == 4) {
            printf("Missing Arguments.\nPlease enter the command with this format:\nsortAll \"Sort by (name or grade)\" \"Sorting type (+ for ascending, - for descending order)\" \"file_name.txt\"\n");
        }
        else if(argc == 5) {
            // if all arguments are given create a child process and sort the file
            pid_t pid;
            pid = fork();
            if(pid < 0) {
                const char *message = "Fork failed\n";
                write(STDERR_FILENO, message, strlen(message));
                return 1;
            } else if(pid == 0) {
                // child process
                int file = open(argv[4], O_RDONLY);
                if(file == -1) {
                    printf("Error opening file!\n");
                    exit(1);
                }
                
                





                close(file);
                exit(0);
            } else {
                // parent process
                wait(NULL);
            }
        }
    }    

    return 0;
}