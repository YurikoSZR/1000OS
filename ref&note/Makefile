.PHONY: clean all
CC = gcc

# Compiler flags
CFLAGS = -Wall -g

# Source files
SRCS = $(wildcard src/*.c)

FILES = $(patsubst src/%, %, $(SRCS))

PREFIXED_FILES = $(addprefix ./bin/, $(FILES))

# Executable files (removes .c extension)
EXES = $(PREFIXED_FILES:%.c=%)

# Default target
all: $(EXES)

# Rule to compile each C file
./bin/%: src/%.c
	$(CC) $(CFLAGS) $< -o $@

# Clean up
clean:
	rm -f $(EXES)