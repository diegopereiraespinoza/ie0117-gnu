## Makefile to recursively search for C source files 
## using wildcards and build an executable
## Define the project name
PROJECT_NAME = gt_main
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
EXCLUDED_FILES := ./gt_aio.c ./gt_gen.c
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
RES_BASE = resources
## Define the C flags for GETTEXT
GT_SRC_RES_DIR = resources/L10n-gt
GT_OBJ_RES_DIR = resources/i18n-gt
GT_SRC_RES_NAME = gt_gen
GT_OBJ_RES_NAME = i18n_gt
GT_URL_RES_NAME = $(RES_BASE)/$(GT_OBJ_RES_NAME)
#GT_SRC_RESOURCES = root.po en.po sr.po
#GT_SRC_RESOURCES = $(GT_SRC_RES_DIR)/root/LC_MESSAGES/i18n_gt.po $(GT_SRC_RES_DIR)/en/LC_MESSAGES/i18n_gt.po $(GT_SRC_RES_DIR)/sr/LC_MESSAGES/i18n_gt.po
GT_SRC_RESOURCES := $(call rwildcard,$(GT_SRC_RES_DIR),*.po)
GT_OBJ_RESOURCES = $(GT_SRC_RESOURCES:$(GT_SRC_RES_DIR)/%.po=$(GT_OBJ_RES_DIR)/%.mo)
GT_HEADERS = 
GT_LIBS = 
## Define the C flags for GTK-3
## ICU is not supported on GTK-4
GTK_HEADERS = $(shell pkg-config --cflags gtk+-3.0)
#GTK_HEADERS = $(shell pkg-config --cflags gtk4)
GTK_LIBS = $(shell pkg-config --libs gtk+-3.0)
#GTK_LIBS = $(shell pkg-config --libs gtk4)

## MSGFMT = $(shell msgfmt)
MSGFMT = msgfmt
HEADERS = $(GT_HEADERS) $(GTK_HEADERS) $(CFLAGS)
LIBS = $(GT_LIBS) $(GTK_LIBS) $(LDFLAGS)
CC_STANDARD = $(shell icu-config --cflags)

## Find all C source files recursively
## This will find files like resources/L10n-gt/root/LC_MESSAGES/i18n_gt.po
GT_SRC_FILES := $(call rwildcard,$(GT_SRC_RES_DIR),*.po)
## Define .mo (Machine Object) files on the filtered .po (Portable Object) files
## GT_OBJ_FILES := $(GT_SRC_FILES:.po=.mo)
## Generate .mo file names from .po source files
## e.g., resources/L10n-gt/root/LC_MESSAGES/i18n_gt.po -> resources/i18n-gt/root/LC_MESSAGES/i18n_gt.mo
GT_OBJ_DIR = .
GT_OBJ_FILES := $(patsubst %.po,$(GT_OBJ_DIR)/%.mo,$(GT_SRC_FILES))

## Default target
#all: $(GT_OBJ_RESOURCES) $(PROJECT_NAME)
all: 
	@echo "Starting..."
	$(MAKE) $(PROJECT_NAME)

## Resources
## Rule to compile PO source files into MO object files
## Generate .mo file names from .po source files
$(GT_OBJ_RES_DIR)/%.mo : $(GT_SRC_RES_DIR)/%.po
	@echo "generating $@"
	@mkdir -p $(dir $@)
	$(MSGFMT) $^ -o $@

## Rule to link the executable
#$(PROJECT_NAME): $(OBJ_FILES) $(GT_OBJ_FILES)
$(PROJECT_NAME): $(OBJ_FILES)
	@mkdir -p $(BIN_DIR)
ifeq ($(DEBUG_MODE), true)
	@echo "Building with debug flags..."
	$(CC) $(HEADERS) -o $(BIN_DIR)/$@ $(OBJ_FILES) $(LIBS) $(CC_OPTS) $(CC_STANDARD) $(DEBUG)
else
	@echo "Building without debug flags..."
	$(CC) $(HEADERS) -o $(BIN_DIR)/$@ $(OBJ_FILES) $(LIBS) $(CC_OPTS) $(CC_STANDARD)
endif
	xgettext -d $(GT_OBJ_RES_NAME) -o $(GT_URL_RES_NAME).pot -k_ $(GT_SRC_RES_NAME).c
	sed "s|=CHARSET|=UTF-8|gI" $(GT_URL_RES_NAME).pot > $(GT_URL_RES_NAME).po
	msgfmt $(GT_URL_RES_NAME).po -o $(GT_URL_RES_NAME).mo
	mkdir -pv $(GT_SRC_RES_DIR)/root/LC_MESSAGES
	cp -rfv $(GT_URL_RES_NAME).po $(GT_SRC_RES_DIR)/root/LC_MESSAGES
	$(MAKE) $(GT_OBJ_RESOURCES)
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
## IF WANT TO USE IT, MUST INCLUDE THE RULE LIKE
## $(PROJECT_NAME): $(OBJ_FILES) $(GT_OBJ_FILES)
## Rule to compile PO source files into MO object files
## Generate .mo file names from .po source files
$(GT_OBJ_DIR)/%.mo: %.po
	@mkdir -p $(dir $@)
	$(MSGFMT) $< -o $@
## Clean up build artifacts
clean:
	rm -rfv $(OBJ_DIR) $(BIN_DIR) $(PROJECT_NAME) $(GT_OBJ_RES_DIR) $(GT_URL_RES_NAME).mo $(GT_URL_RES_NAME).po $(GT_URL_RES_NAME).pot
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
#./bin/gt_main "$@" ## line 5
#exit 0 ## line 6

## DO NOT FORGET TO UNCOMMENT 
## lines 3, 5 & 6
