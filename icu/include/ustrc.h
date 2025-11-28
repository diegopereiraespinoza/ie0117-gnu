#ifndef UTILS_STRC_H
#define UTILS_STRC_H

#include <stdio.h>            // For scanf, fprintf, printf, perror, getline, ...
#include <stdlib.h>           // For setenv, getenv, rand, random, realpath, system, strtol, malloc, realloc, free, ...
#include <string.h>           // For memmove, strlen, strdup, strcpy, strstr, strtok, ...
#include <stdint.h>           // For int32_t
#include "uresb.h"            // utils for i18n/i10n resources in icu

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
char* str_init(char **str_head, char *str);//, UFILE *outerr);
// replaces a string variable dynamically
// willing to free the memory by yourself
char* str_replace(char **str_head, char *str);//, UFILE *outerr);
// concatenates a string to another string variable
// willing to free the dynamic memory by yourself
char* str_concat(char **str_head, char *str_ext);//, UFILE *outerr);

// convert UChar to char* (for ASCII-only example)
char* str_from_usrt(char **str, const UChar *ustr, UFILE *outerr);
// convert char to UChar* (for ASCII-only example)
UChar* ustr_from_srt(UChar **ustr, char *str, UFILE *outerr);
// get a ustring in UNICODE (UChar) from a locale resource given a key
const UChar* ustr_by_key(char *packageName, char *locale, char *key, UFILE *outerr);
// get a string in default codepage (usually UTF-8) from a locale resource given a key
char* str_by_key(char *packageName, char *locale, char *key, UFILE *outerr);

#endif // UTILS_STRC_H
