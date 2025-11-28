#ifndef UTILS_LANG_H
#define UTILS_LANG_H

#define _GNU_SOURCE           // For some systems, getenv, strndup, random, realpath, PATH_MAX, strsep, asprintf, getline, exp10, ...
#include <locale.h>           // For setlocale, printf("%'d", number) with thousands separator
#include <stdio.h>            // For scanf, fprintf, printf, perror, getline, ...
#include <stdlib.h>           // For setenv, getenv, rand, random, realpath, system, strtol, malloc, realloc, free, ...
#include <string.h>           // For memmove, strlen, strdup, strcpy, strstr, strtok, ...
#include <stdbool.h>          // For bool types
#include <stdint.h>           // For int32_t
#include "ustrc.h"            // utils for init, duplicate and concat

/*
// global constants defined by macros
*/

/*
// global variables defined by struct for GTK
*/

// data type for gettext user data
// typedef struct L10nKeys L10nKeys;
typedef struct L10nKeys {
	char *wdw_title; // "Internationalization"
	char *greeting; // "Hello!"
	char *farewell; // "Goodbye!"
	char *welcome; // "Welcome!"
	char *lbl_lang; // "Language"
	char *cmd_lang_root; // "Select"
	char *cmd_lang_ar; // "Arabic"
	char *cmd_lang_de; // "German"
	char *cmd_lang_en; // "English"
	char *cmd_lang_es; // "Spanish"
	char *cmd_lang_fr; // "French"
	char *cmd_lang_hi; // "Hindi"
	char *cmd_lang_ja; // "Japanese"
	char *cmd_lang_ru; // "Russian"
	char *cmd_lang_sr; // "Serbian"
	char *cmd_lang_zh; // "Chinese CN"
	char *cmd_lang_zh_TW; // "Chinese TW"
	char *nlangs; // "%d language is available."
	char *nlangs_plural; // "%d languages are available."
	//struct L10nKeys* next;
} L10nKeys;

/*
// function declarations
*/

// return: gtkeys
// allocate memory to create the new gtkeys
L10nKeys* gtkeys_create();

void init_langs_avail(int nlangs, char *langs_usrt[nlangs]);
void free_langs_avail(int nlangs, char *langs_usrt[nlangs]);
void update_langs_avail(L10nKeys *gtkeys, int nlangs, char *langs_usrt[nlangs], char *packageName, char *locale, UFILE *outerr);//, char *packageName, UFILE *out, UFILE *outerr);
void print_langs_avail(int nlangs, char *langs_usrt[nlangs]);
// len_lang = 2 for 'en' and len_lang = 5 for 'en_US'
char* lang_from_loc(char **str_lang, char *str_loc, int32_t len_lang);
char* loc_from_lang(int nlangs, char *langs_avail[nlangs], char *locale);
bool is_lang_avail(int nlangs, char *langs_avail[nlangs], char *locale, int *cmb_lang_active);
bool update_lang_env(char *locale, char *locale_default);
bool update_locale(char *locale, char *locale_default);
bool is_loc_avail(char *locale, char *locale_default);

#endif // UTILS_LANG_H
