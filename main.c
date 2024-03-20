#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h> // this is for system calls

#define TRUE 1

struct StudentRecord {
    char name[50];
    char grade[3];
};

int parse_words(const char *text, char **words, int max_words) {
    int num_words = 0;
    const char *start = text;

    while (*text != '\0' && num_words < max_words) {
        if (*text == ' ') {  // Found a word boundary
            if (start != text) {  // Ignore leading spaces
                int word_len = text - start;
                words[num_words] = (char*)malloc((word_len + 1) * sizeof(char));
                strncpy(words[num_words], start, word_len);
                words[num_words][word_len] = '\0';  // Null-terminate the word
                num_words++;
            }
            start = text + 1; // Update start for the next word
        }
        text++;
    }

    // Handle the last word (if any)
    if (start != text) {
        int word_len = text - start;
        words[num_words] = (char*)malloc((word_len + 1) * sizeof(char));
        strncpy(words[num_words], start, word_len);
        words[num_words][word_len] = '\0';
        num_words++;
    }
    return num_words;
}

int read_records(const char *filename, struct StudentRecord *records) {

    int fd = open(filename, O_RDONLY);
    if (fd == -1) {
        perror("Error opening file");
        return -1;
    }

    // read all the student name and grades from the file and put them into records struct array
    int num_read = 0; // Number of records read
    char line[1024];
    ssize_t n;
    char *lineStart = line;
    while ((n = read(fd, lineStart, 1)) > 0) { // Read one character at a time
        if (*lineStart == '\n' || *lineStart == '\0') { // End of line
            *lineStart = '\0'; // Null-terminate the line

            // Parse the line into words
            char *words[20];
            int num_words = parse_words(line, words, 20);

            // the last word is the grade and the rest is the name
            strcpy(records[num_read].grade, words[num_words - 1]);

            for (int i = 0; i < num_words - 1; i++) {
                strcat(records[num_read].name, words[i]);
                if(i != num_words - 2) {
                    strcat(records[num_read].name, " ");
                }
            }

            lineStart = line; // Reset the line start pointer
            num_read++;
        } else {
            lineStart++;
        }
    }

    close(fd);
    return num_read;
}

void sort_records(struct StudentRecord* records, char* sort_by, char* asc_or_desc, int num_records) {
    // if the sort_by is name and asc_or_desc is + sort the records by name in ascending order
    if(strcmp(sort_by, "name") == 0 && strcmp(asc_or_desc, "+") == 0) {
        // using bubble sort
        for (int i = 0; i < num_records; i++) {
            for (int j = 0; j < num_records - i - 1; j++) {
                if(strcmp(records[j].name, records[j+1].name) > 0) {
                    struct StudentRecord temp = records[j];
                    records[j] = records[j+1];
                    records[j+1] = temp;
                }
            }
        }
    }

    // if the sort_by is name and asc_or_desc is - sort the records by name in descending order
    if(strcmp(sort_by, "name") == 0 && strcmp(asc_or_desc, "-") == 0) {
        // using bubble sort
        for (int i = 0; i < num_records; i++) {
            for (int j = 0; j < num_records - i - 1; j++) {
                if(strcmp(records[j].name, records[j+1].name) < 0) {
                    struct StudentRecord temp = records[j];
                    records[j] = records[j+1];
                    records[j+1] = temp;
                }
            }
        }
    }

    // if the sort_by is grade and asc_or_desc is + sort the records by grade in ascending order
    if(strcmp(sort_by, "grade") == 0 && strcmp(asc_or_desc, "+") == 0) {
        // using bubble sort
        for (int i = 0; i < num_records; i++) {
            for (int j = 0; j < num_records - i - 1; j++) {
                if(strcmp(records[j].grade, records[j+1].grade) > 0) {
                    struct StudentRecord temp = records[j];
                    records[j] = records[j+1];
                    records[j+1] = temp;
                }
            }
        }
    }

    // if the sort_by is grade and asc_or_desc is - sort the records by grade in descending order
    if(strcmp(sort_by, "grade") == 0 && strcmp(asc_or_desc, "-") == 0) {
        // using bubble sort
        for (int i = 0; i < num_records; i++) {
            for (int j = 0; j < num_records - i - 1; j++) {
                if(strcmp(records[j].grade, records[j+1].grade) < 0) {
                    struct StudentRecord temp = records[j];
                    records[j] = records[j+1];
                    records[j+1] = temp;
                }
            }
        }
    }
}

void find_argc_argv(char* command_input, int* argc, char* argv[]) {
        // now we should parse this command input into argv and argc
        // we should split the command_input by space and "" put them into argv
        // for example if the command_input is searchStudent "Name SecondName Surname" "file_name.txt" we should parse them like this
        // argv[0] = NULL
        // argv[1] = searchStudent
        // argv[2] = Name SecondName Surname
        // argv[3] = file_name.txt

        // we should also count the number of arguments and put it into argc
        // Parsing logic

        // first check if there is any " in the command_input
        if(strchr(command_input, 34) == NULL) {
            // parse the command_input by space
            *argc = 0;
            char *token = strtok(command_input, " "); // Split by space
            while (token != NULL) {

                // if the EOL is in the token we should remove it
                if(strchr(token, 10) != NULL) {
                    token[strlen(token) - 1] = '\0';
                }

                argv[*argc] = (char*)malloc((strlen(token)) * sizeof(char));

                strcpy(argv[*argc], token);

                (*argc)++;
                token = strtok(NULL, " ");
            }
            return;
        }

        *argc = 0;
        char *token = strtok(command_input, "\""); // Split by double quotes
        while (token != NULL) {
            
            // if the token is space or EOL we should skip it
            if(token[0] == ' ' || token[0] == '\n' || token[0] == '\0') {
                token = strtok(NULL, "\"");
                continue;
            }

            // if the end of the token is space we should parse words in the token
            if(token[strlen(token) - 1] == ' ') {
                char *words[100];
                int num_words = parse_words(token, words, 100);
                for (int i = 0; i < num_words; i++) {
                    argv[*argc] = (char*)malloc((strlen(words[i])) * sizeof(char));
                    strcpy(argv[*argc], words[i]);
                    // argv[*argc][strlen(argv[*argc])-1] = '\0'; // terminate the string with null character
                    (*argc)++;
                }
            } else {
                argv[*argc] = (char*)malloc((strlen(token)) * sizeof(char));
                strcpy(argv[*argc], token);
                printf("Printing the token: %s\n", argv[*argc]);
                // argv[*argc][strlen(argv[*argc])-1] = '\0'; // terminate the string with null character
                (*argc)++;
            }
            token = strtok(NULL, "\"");
        }
}


int main() {
    
    printf("Enter your command (q to exit):\n");
    char* command_input = (char*)malloc(1024 * sizeof(char));
    int argc;
    char* argv[100];

    while(TRUE) {
        // free command_input and allocate new memory for command_input
        free(command_input);
        char* command_input = (char*)malloc(1024 * sizeof(char));

        // free argv and allocate new memory for argv
        for (int i = 0; i < argc; i++) {
            free(argv[i]);
        }

        printf("$ ");
        fflush(stdout);

        // take the command input from the user
        int bytes_read = read(STDIN_FILENO, command_input, 1024);

        if(bytes_read < 0) {
            perror("Error reading from stdin");
            return 1;
        }

        printf("Command: %s\n", command_input);

        // now we should parse this command input into argv and argc
        find_argc_argv(command_input, &argc, argv);

        
        // print the argc and argv
        printf("argc: %d\n", argc);
        for (int i = 0; i < argc; i++) {
            printf("argv[%d]: %s\n", i, argv[i]);
        }

        // if the command is q exit the program
        if(strcmp(argv[0], "q") == 0) {
            printf("Exiting the program...\n");
            break;
        }

        // if the argv[0] is gtuStudentGrades and there is no more argument 
        // print all available commands
        if (strcmp(argv[0], "gtuStudentGrades") == 0 && argc == 1) {
            printf("Going to print all the available commands soon!\n");
        }

        // if the argv[0] is gtuStudentGrades and file name is given
        if(strcmp(argv[0], "gtuStudentGrades") == 0 && argc == 2) {
            pid_t pid;
            pid = fork();
            if(pid < 0) {
                const char *message = "Fork failed\n";
                write(STDERR_FILENO, message, strlen(message));
                return 1;
            } else if(pid == 0) {
                // child process
                // check if the file exists
                int fd = open(argv[1], O_RDONLY);
                if(fd != -1) {
                    printf("File exists!\n");
                    exit(0);
                }
                // if not exists create a file using system call

                // int file = open(argv[1], O_WRONLY, O_CREAT, 0644);
                int file = creat(argv[1], 0644);
                printf("File created!\n");
                close(file);
                exit(0);
            } else {
                // parent process
                wait(NULL);
            }
        }

        //add new student grade to the file
        // if the argv[0] addStudentGrade
        if(strcmp(argv[0], "addStudentGrade") == 0) {
            // if there is no more argument
            if(argc == 1) {
                printf("Please enter the student name and grade with this format:\naddStudentGrade \"Name Surname\" \"Student Grade (eg. AA)\" \"file_name.txt\"\n");
            }
            if(argc == 2 || argc == 3) {
                printf("Missing Arguments.\nPlease enter the student name and grade with this format:\naddStudentGrade \"Name Surname\" \"Student Grade (eg. AA)\" \"file_name.txt\"\n");
            }
            if(argc == 4) {
                // if all arguments are given create a child process and add the student grade to the file
                pid_t pid;
                pid = fork();
                if(pid < 0) {
                    const char *message = "Fork failed\n";
                    write(STDERR_FILENO, message, strlen(message));
                    return 1;
                } else if(pid == 0) {
                    // child process
                    int file = open(argv[3], O_WRONLY | O_APPEND);
                    
                    if(file == -1) {
                        printf("Error opening file!\n");
                        exit(1);
                    }
                    // write to file using system call write with using buffer
                    char buffer[1024];
                    snprintf(buffer, sizeof(buffer), "%s %s\n", argv[1], argv[2]);
                    write(file, buffer, strlen(buffer));
                    close(file);
                    printf("Student grade added to the file: %s\n", argv[3]);
                    exit(0);
                } else {
                    // parent process
                    wait(NULL);
                }
            }
        }

        // search for a student in the file
        // if the argv[0] is searchStudent
        if(strcmp(argv[0], "searchStudent") == 0) {
            // if there is no more argument
            if(argc == 1) {
                printf("Please enter the student name and file name with this format:\nsearchStudent \"Name Surname\" \"file_name.txt\"\n");
            }
            if(argc == 2) {
                printf("Missing Arguments.\nPlease enter the student name and file name with this format:\nsearchStudent \"Name Surname\" \"file_name.txt\"\n");
            }
            if(argc == 3) {
                // if all arguments are given create a child process and search for the student in the file
                pid_t pid;
                pid = fork();
                if(pid < 0) {
                    const char *message = "Fork failed\n";
                    write(STDERR_FILENO, message, strlen(message));
                    return 1;
                } else if(pid == 0) {

                    // child process
                    int file = open(argv[2], O_RDONLY);

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
                            if (strncmp(line, argv[1], strlen(argv[1])) == 0 && line[strlen(argv[1])] == ' ') {
                                printf("Student found: %s\n", line);
                                close(file);
                                exit(0);
                            }
                            lineStart = line; // Reset the line start pointer
                        } else {
                            lineStart++;
                        }
                    }
                    printf("Student with name: \"%s\" not found in the file.\n", argv[1]);
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

        if(strcmp(argv[0], "sortAll") == 0) {
            if(argc == 1) {
                // sorting can be made by name or grade in ascending or descending order
                printf("Please enter the command with this format:\nsortAll \"Sort by (name or grade)\" \"Sorting type (+ for ascending, - for descending order)\" \"file_name.txt\"\n");
            }
            else if(argc == 2) {
                // the command is sortAll "file_name.txt"
                // sort by name in ascending order
                pid_t pid;
                pid = fork();
                if(pid < 0) {
                    const char *message = "Fork failed\n";
                    write(STDERR_FILENO, message, strlen(message));
                    return 1;
                } else if(pid == 0) {
                    // child process
                    int file = open(argv[1], O_RDONLY);
                    if(file == -1) {
                        printf("Error opening file!\n");
                        exit(1);
                    }
                    
                    // read the names and grades from the file and sort them by name in ascending order
                    // using system calls

                    struct StudentRecord records[100];
                    int num_records = read_records(argv[1], records);
                    printf("Number of records: %d\n", num_records);
                    // print all the records
                    for (int i = 0; i < num_records; i++) {
                        printf("%s %s\n", records[i].name, records[i].grade);
                    }

                    close(file);
                    // now we should sort the records by name in ascending order
                    
                    sort_records(records, "name", "+", num_records);

                    // print all the records after sorting
                    printf("\nAfter sorted:\n");
                    printf("file name: %s\n", argv[1]);
                    for (int i = 0; i < num_records; i++) {
                        printf("%s %s\n", records[i].name, records[i].grade);
                    }

                    // now we should overwrite the sorted records to the file
                    int file2 = open(argv[1], O_WRONLY | O_TRUNC);
                    if(file2 == -1) {
                        printf("Error opening file!\n");
                        exit(1);
                    }
                    for (int i = 0; i < num_records; i++) {
                        char buffer[1024];
                        snprintf(buffer, sizeof(buffer), "%s %s\n", records[i].name, records[i].grade);
                        write(file2, buffer, strlen(buffer));
                    }
                    printf("The student records in the file \"%s\" are sorted by name in ascending order successfully!.\n", argv[1]);
                    
                    exit(0);

                } else {
                    // parent process
                    wait(NULL);
                }
            }
            else if(argc == 3) {
                printf("Missing Arguments.\nPlease enter the command with this format:\nsortAll \"Sort by (name or grade)\" \"Sorting type (+ for ascending, - for descending order)\" \"file_name.txt\"\n");
            }
            else if(argc == 4) {
                // if all arguments are given create a child process and sort the file
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
                    
                    close(file);
                    exit(0);
                } else {
                    // parent process
                    wait(NULL);
                }
            }
        }
    }

    return 0;
}