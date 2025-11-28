#ifndef UTILS_STRC_H
#define UTILS_STRC_H

#include <stdio.h>            // For scanf, fprintf, printf, perror, getline, ...
#include <stdlib.h>           // For setenv, getenv, rand, random, realpath, system, strtol, malloc, realloc, free, ...
#include <string.h>           // For memmove, strlen, strdup, strcpy, strstr, strtok, ...
#include <stdint.h>           // For int32_t
#include <libintl.h>          // For i18n/i10n resources in gettext

/*
// global constants defined by macros
*/

#define _(String) gettext(String)
#define _t(String1, String2, n) ngettext(String1, String2, n)

/*
// global variables defined by struct for GTK
*/

/*
// function declarations
*/

// allocates a string variable dynamically
// willing to free the memory by yourself
char* str_init(char **str_head, char *str);//, FILE *outerr);
// replaces a string variable dynamically
// willing to free the memory by yourself
char* str_replace(char **str_head, char *str);//, FILE *outerr);
// concatenates a string to another string variable
// willing to free the dynamic memory by yourself
char* str_concat(char **str_head, char *str_ext);//, FILE *outerr);

// convert UChar to char* (for ASCII-only example)
char* str_from_usrt(char **str, const char *ustr, FILE *outerr);
// get a ustring in UNICODE (UChar) from a locale resource given a key
const char* ustr_by_key(char *key);//char *packageName, char *locale, char *key, FILE *outerr);
// get a string in default codepage (usually UTF-8) from a locale resource given a key
char* str_by_key(char *locale, char *key, FILE *outerr);//char *packageName, char *locale, char *key, FILE *out, FILE *outerr);

#endif // UTILS_STRC_H
