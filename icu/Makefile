## Makefile to recursively search for C source files 
## using wildcards and build an executable
## Define the project name
PROJECT_NAME = icu_main
BIN_DIR = bin
OBJ_DIR = obj
## Define the source directories to search recursively
## Add more directories as needed
#SRC_DIRS = src include
## OR
SRC_DIRS = .
## Function to recursively find files matching a pattern in given directories
## Usage: $(call rwildcard, <directory_list>, <pattern>)
rwildcard = $(foreach d,$(wildcard $(1:=/*)),$(call rwildcard,$d,$2) $(filter $(subst *,%,$2),$d))
## Find all C source files recursively
## This will find files like src/main.c, src/utils/helper.c, etc.
ALL_SRC_FILES := $(call rwildcard,$(SRC_DIRS),*.c)
## Define all source files
#ALL_SRC_FILES := $(wildcard *.c)
## Specify the file to exclude
EXCLUDED_FILES := ./icu_aio.c
## Filter out the excluded file from the list of source files to compile
SRC_FILES := $(filter-out $(EXCLUDED_FILES), $(ALL_SRC_FILES))
## Define object files based on the filtered source files
## OBJ_FILES := $(SRC_FILES:.c=.o)
## Generate object file names from source files
## e.g., src/main.c -> obj/main.o, src/utils/helper.c -> obj/utils/helper.o
OBJ_FILES := $(patsubst %.c,$(OBJ_DIR)/%.o,$(SRC_FILES))

## Define the C compiler and flags
CC = gcc
CC_STANDARD = -std=c11
CFLAGS = -Iinclude
LDFLAGS = -lm
CC_OPTS = -Wall -Wextra -Wpedantic
DEBUG = -g -O0
DEBUG_MODE = false
#DEBUG_MODE = true
## Define the C flags for ICU
ICU_SRC_RES_DIR = resources/L10n-icu
ICU_OBJ_RES_DIR = resources/i18n-icu
#ICU_SRC_RESOURCES = root.txt en.txt sr.txt
#ICU_SRC_RESOURCES = $(ICU_SRC_RES_DIR)/root.txt $(ICU_SRC_RES_DIR)/en.txt $(ICU_SRC_RES_DIR)/sr.txt
ICU_SRC_RESOURCES := $(call rwildcard,$(ICU_SRC_RES_DIR),*.txt)
ICU_OBJ_RESOURCES = $(ICU_SRC_RESOURCES:$(ICU_SRC_RES_DIR)/%.txt=$(ICU_OBJ_RES_DIR)/%.res)
ICU_HEADERS = -I/usr/include
ICU_LIBS = $(shell icu-config --ldflags --ldflags-icuio --ldflags-toolutil)
## Define the C flags for GTK-3
## ICU is not supported on GTK-4
GTK_HEADERS = $(shell pkg-config --cflags gtk+-3.0)
#GTK_HEADERS = $(shell pkg-config --cflags gtk4)
GTK_LIBS = $(shell pkg-config --libs gtk+-3.0)
#GTK_LIBS = $(shell pkg-config --libs gtk4)

GENRB = $(shell icu-config --invoke=genrb)
## env "LD_LIBRARY_PATH=/usr/lib:${LD_LIBRARY_PATH}" /usr/bin/genrb
#GENRB_OPTS = -s . -d .
GENRB_OPTS = -s . -d $(ICU_OBJ_RES_DIR)
HEADERS = $(ICU_HEADERS) $(GTK_HEADERS) $(CFLAGS)
LIBS = $(ICU_LIBS) $(GTK_LIBS) $(LDFLAGS)
CC_STANDARD = $(shell icu-config --cflags)

## Default target
#all: $(ICU_OBJ_RESOURCES) $(PROJECT_NAME)
all: 
	@echo "Starting..."
	$(MAKE) $(ICU_OBJ_RESOURCES) $(PROJECT_NAME)

## Resources
$(ICU_OBJ_RES_DIR)/%.res : $(ICU_SRC_RES_DIR)/%.txt
	@echo "generating $@"
	@mkdir -p $(dir $@)
	$(GENRB) $(GENRB_OPTS) $^

## Special codepage for genrb --encoding cp1251
$(ICU_OBJ_RES_DIR)/sr.res : $(ICU_SRC_RES_DIR)/sr.txt
	@echo "generating $@"
	@mkdir -p $(dir $@)
	$(GENRB) $(GENRB_OPTS) -e cp1251 $?

## Rule to link the executable
$(PROJECT_NAME): $(OBJ_FILES)
	@mkdir -p $(BIN_DIR)
ifeq ($(DEBUG_MODE), true)
	@echo "Building with debug flags..."
	$(CC) $(HEADERS) -o $(BIN_DIR)/$@ $(OBJ_FILES) $(LIBS) $(CC_OPTS) $(CC_STANDARD) $(DEBUG)
else
	@echo "Building without debug flags..."
	$(CC) $(HEADERS) -o $(BIN_DIR)/$@ $(OBJ_FILES) $(LIBS) $(CC_OPTS) $(CC_STANDARD)
endif
## Rule to compile C source files into object files
## This uses a pattern rule to handle files in different subdirectories
$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
ifeq ($(DEBUG_MODE), true)
	@echo "Compiling with debug flags..."
	$(CC) $(HEADERS) -c $< -o $@ $(LIBS) $(CC_OPTS) $(CC_STANDARD) $(DEBUG)
else
	@echo "Compiling without debug flags..."
	$(CC) $(HEADERS) -c $< -o $@ $(LIBS) $(CC_OPTS) $(CC_STANDARD)
endif
## Clean up build artifacts
clean:
	rm -rfv $(OBJ_DIR) $(BIN_DIR) $(PROJECT_NAME) $(ICU_OBJ_RES_DIR)
.PHONY: all clean run
## If the first argument is "run"...
## WORK FOR:
## e.g.: make run ARGS="arg1 arg2 \"arg3 with spaces\""
TARGETS = run target3 target3
ifneq ($(strip $(ARGS)),)
    RUN_ARGS = $(ARGS)
## DO NOT WORK FOR:
## e.g.: make run arg1 arg2 "arg3 with spaces"
## ALSO, ESCAPING SPACES DO NOT WORK:
## e.g.: make run arg1 arg2 "arg3\ with\ spaces"
## INTERPRETED AS IF THEY WERE SEPARATED ARGUMENTS LIKE:
## e.g.: make run arg1 arg2 arg3 with spaces
## WORK FOR:
## e.g.: make run "arg1 arg2 \"arg3 with spaces\""
## ISSUES:
## DO NOT WORK if you need to quote an argument with spaces
## Tab inside the "ifeq" DO NOT WORK as equal as spaces
#else ifeq (run,$(firstword $(MAKECMDGOALS)))
#else ifneq ($(findstring $(firstword $(MAKECMDGOALS)),run target3 target3),)
else ifneq ($(findstring $(firstword $(MAKECMDGOALS)),$(TARGETS)),)
    ## use the rest as arguments for "run"
    RUN_ARGS := $(wordlist 2,$(words $(MAKECMDGOALS)),$(MAKECMDGOALS))
    ## ...and turn them into do-nothing targets
    $(eval $(RUN_ARGS):;@:)
endif
#run: $(PROJECT_NAME)
	#@echo ./$(BIN_DIR)/$(PROJECT_NAME) $(RUN_ARGS)
run: all
	@echo "Executing..."
	@echo ./$(BIN_DIR)/$(PROJECT_NAME) $(RUN_ARGS)
	#valgrind --tool=memcheck --leak-check=full ./$(BIN_DIR)/$(PROJECT_NAME) $(RUN_ARGS)
	./$(BIN_DIR)/$(PROJECT_NAME) $(RUN_ARGS)
## BETTER APPROACH:
## Intead of processing arguments in Makefile
## Create a bash script with any name.sh
## Run ./name.sh arg1 arg2 "arg3 with spaces"
## Before run, Copy & Paste as its content
## The next 6 lines:
#!/bin/bash
## compile and build
#make all ## line 3
## run with any arguments
#./bin/icu_main "$@" ## line 5
#exit 0 ## line 6

## DO NOT FORGET TO UNCOMMENT 
## lines 3, 5 & 6
