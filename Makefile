# Compiler settings
CC = gcc
CFLAGS = -Wall -g

# Project name
EXEC = system

# Source files
SRCS = main.c user.c exam.c file_io.c menu.c

# Object files
OBJS = $(SRCS:.c=.o)

# Default target (compiling the project)
all: $(EXEC)

# Rule to create the executable from object files
$(EXEC): $(OBJS)
	$(CC) $(OBJS) -o $(EXEC)

# Rule to compile the .c files into .o object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up object files and executable
clean:
	rm -f $(OBJS) $(EXEC)

# Rebuild the project from scratch (clean + make)
rebuild: clean all

# Run the program after compilation
run: $(EXEC)
	./$(EXEC)

# Prevent make from trying to build a file named 'clean' or 'run'
.PHONY: all clean rebuild run
