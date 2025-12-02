# Makefile to recursively search for C source files 
# using wildcards and build an executable
# Define the project name
PROJECT_NAME = lab06e1fp_main
# Define the source directories to search recursively
# Add more directories as needed
#SRC_DIRS = src include
# OR
SRC_DIRS = .
# Function to recursively find files matching a pattern in given directories
# Usage: $(call rwildcard, <directory_list>, <pattern>)
rwildcard = $(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))
# Find all C source files recursively
# This will find files like src/main.c, src/utils/helper.c, etc.
ALL_SRC_FILES := $(call rwildcard,$(SRC_DIRS),*.c)
# Define all source files
#ALL_SRC_FILES := $(wildcard *.c)
# Specify the file to exclude
EXCLUDED_FILES := ./lab06e1fp_aio.c
# Filter out the excluded file from the list of source files to compile
SRC_FILES := $(filter-out $(EXCLUDED_FILES), $(ALL_SRC_FILES))
# Define object files based on the filtered source files
# OBJ_FILES := $(SRC_FILES:.c=.o)
# Generate object file names from source files
# e.g., src/main.c -> obj/main.o, src/utils/helper.c -> obj/utils/helper.o
OBJ_FILES := $(patsubst %.c,obj/%.o,$(SRC_FILES))
# Define the C compiler and flags
CC = gcc
STANDARD = -std=c11
CFLAGS = -Iinclude
LDFLAGS = -lm
OPTIONS = -Wall -Wextra -Wpedantic
DEBUG = -g -O0
#DEBUG_MODE = true
DEBUG_MODE = false
# Default target
#all: $(PROJECT_NAME)
all: 
	@echo "Starting..."
	$(MAKE) $(PROJECT_NAME)
# Rule to link the executable
$(PROJECT_NAME): $(OBJ_FILES)
	@mkdir -p bin
ifeq ($(DEBUG_MODE), true)
	@echo "Building with debug flags..."
	$(CC) $(CFLAGS) -o bin/$@ $(OBJ_FILES) $(LDFLAGS) $(OPTIONS) $(STANDARD) $(DEBUG)
else
	@echo "Building without debug flags..."
	$(CC) $(CFLAGS) -o bin/$@ $(OBJ_FILES) $(LDFLAGS) $(OPTIONS) $(STANDARD)
endif
# Rule to compile C source files into object files
# This uses a pattern rule to handle files in different subdirectories
obj/%.o: %.c
	@mkdir -p $(dir $@)
ifeq ($(DEBUG_MODE), true)
	@echo "Compiling with debug flags..."
	$(CC) $(CFLAGS) -c $< -o $@ $(LDFLAGS) $(OPTIONS) $(STANDARD) $(DEBUG)
else
	@echo "Compiling without debug flags..."
	$(CC) $(CFLAGS) -c $< -o $@ $(LDFLAGS) $(OPTIONS) $(STANDARD)
endif
# Clean up build artifacts
clean:
	rm -rfv bin obj $(PROJECT_NAME)
.PHONY: all clean run
# If the first argument is "run"...
# WORK FOR:
# e.g.: make run ARGS="arg1 arg2 \"arg3 with spaces\""
TARGETS = run target2 target3
ifneq ($(strip $(ARGS)),)
    RUN_ARGS = $(ARGS)
# DO NOT WORK FOR:
# e.g.: make run arg1 arg2 "arg3 with spaces"
# ALSO, ESCAPING SPACES DO NOT WORK:
# e.g.: make run arg1 arg2 "arg3\ with\ spaces"
# INTERPRETED AS IF THEY WERE SEPARATED ARGUMENTS LIKE:
# e.g.: make run arg1 arg2 arg3 with spaces
# WORK FOR:
# e.g.: make run "arg1 arg2 \"arg3 with spaces\""
# ISSUES:
# DO NOT WORK if you need to quote an argument with spaces
# Tab inside the "ifeq" DO NOT WORK as equal as spaces
#else ifeq (run,$(firstword $(MAKECMDGOALS)))
#else ifneq ($(findstring $(firstword $(MAKECMDGOALS)),run target2 target3),)
else ifneq ($(findstring $(firstword $(MAKECMDGOALS)),$(TARGETS)),)
    # use the rest as arguments for "run"
    RUN_ARGS := $(wordlist 2,$(words $(MAKECMDGOALS)),$(MAKECMDGOALS))
    # ...and turn them into do-nothing targets
    $(eval $(RUN_ARGS):;@:)
endif
#run: $(PROJECT_NAME)
	#@echo ./bin/$(PROJECT_NAME) $(RUN_ARGS)
run: all
	@echo "Executing..."
	@echo ./bin/$(PROJECT_NAME) $(RUN_ARGS)
	#valgrind --tool=memcheck --leak-check=full ./bin/$(PROJECT_NAME) $(RUN_ARGS)
	./bin/$(PROJECT_NAME) $(RUN_ARGS)
# BETTER APPROACH:
# Intead of processing arguments in Makefile
# Create a bash script with any name.sh
# Run ./name.sh arg1 arg2 "arg3 with spaces"
# Before run, Copy & Paste as its content
# The next 6 lines:
#!/bin/bash
# compile and build
#make all # line 3
# run with any arguments
#./bin/lab06e1fp_main "$@" # line 5
#exit 0 # line 6

# DO NOT FORGET TO UNCOMMENT 
# lines 3, 5 & 6
