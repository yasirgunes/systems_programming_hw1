# Name of your executable 
EXECUTABLE = gtuStudentGrades

# Source files (adapt if you have functions.c)
SOURCES = main.c

# Compiler and flags  
CC = gcc
CFLAGS = -Wall -g


# Main target: Builds your executable 
all: $(EXECUTABLE)

$(EXECUTABLE): $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES) -o $(EXECUTABLE)

# Target to remove temporary files 
clean:
	rm -f $(EXECUTABLE) *.o 
