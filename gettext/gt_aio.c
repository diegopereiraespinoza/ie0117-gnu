#define _GNU_SOURCE           // For some systems, getenv, strndup, random, realpath, PATH_MAX, strsep, asprintf, getline, exp10, ...
#include <locale.h>           // For setlocale, printf("%'d", number) with thousands separator
#include <stdio.h>            // For scanf, fprintf, printf, perror, getline, ...
#include <stdlib.h>           // For setenv, getenv, rand, random, realpath, system, strtol, malloc, realloc, free, ...
#include <stdbool.h>          // For bool types
#include <stdint.h>           // For int32_t
#include <string.h>           // For memmove, strlen, strdup, strcpy, strstr, strtok, ...
//#include <time.h>             // For time to seed srand/srandom
#include <limits.h>           // For char, short, int (INT_MIN/INT_MAX), long | long long (LLONG_MIN/LLONG_MAX), PATH_MAX, ...
//#include <float.h>            // For float (FLT_MIN/FLT_MAX), double | long double (LDBL_MIN/LDBL_MAX)
//#include <errno.h>            // For errno == ERANGE, clear errno=0 before call strlen, strdup, strcpy, strstr, strtok, strtol, ...
//#include <ctype.h>            // For toupper, tolower, isalpha, isdigit, isspace (trim spaces from words)
//#include <iconv.h>            // For decimal ASCII to UTF-8
//#include <uchar.h>            // For uchar_t on estandar UTF-8
//#include <wchar.h>            // For wchar_t on wide character
//#include <libio.h>            // For fwide
//#include <libgen.h>           // For dirname, basename, ...
#ifdef WIN32
#include <direct.h>           // For _getcwd, ...
#else // WIN32
#include <unistd.h>           // For getcwd, getpid, readlink, ...
#endif // WIN32
#include <libintl.h>          // For i18n/i10n resources in gettext
#include <gtk/gtk.h>          // For graphics and widgets in GTK-3, ICU is not supported on GTK-4

/*
// global constants defined by macros
*/

#define _(String) gettext(String)
#define _t(String1, String2, n) ngettext(String1, String2, n)

// realpath and PATH_MAX requires _GNU_SOURCE
#ifndef PATH_MAX
	// _POSIX_PATH_MAX = 256 // maximum in POSIX
	// _XOPEN_PATH_MAX = 1024 // maximum in XSI
	#define PATH_MAX 1024 // maximum storable as a pathname in buffer
#endif // PATH_MAX

// total quantity of languages available to select from combo box in gtk
#ifndef LANGS_AVAILABLE
	#define LANGS_AVAILABLE 12
#endif // LANGS_AVAILABLE

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

// data type for GTK user data
// typedef struct GtkUserData GtkUserData;
typedef struct GtkUserData {
	L10nKeys *gtkeys; // used by gettext to get localized messages by key
	
	FILE *out; // used by fprintf to print localized messages to stdout
	FILE *outerr; // used by fprintf to print localized messages to stderr
	
	int *langs_total; // total quantity of languages available to select from combo box in gtk
	char **langs_avail; // array to store codes of languages available to select from combo box in gtk
	char **langs_usrt; // array to update combo box list in gtk according to selected language
	int *cmb_lang_active; // global variable to store the index to last selected language from combo box in gtk
	int cmb_lang_default; // constant to default index in case lang is not enable for combo box in gtk
	
	char *locale_avail; // stores the last available locale, as locales may not be enable in /etc/locale.gen
	//char *locale = "en_US.UTF-8"; // this will store the current locale or last selected -> NOT WORKING ON ARCH
	//char *locale = "en_US.utf8"; // this will store the current locale or last selected -> WORKING ON ARCH LINUX
	char *locale; // this will store the current locale or last selected -> BUT BETTER WORK WITH DEFAULT LANG
	char *locale_default; // constant to default locale in case a locale is not enable in /etc/locale.gen
	char *encoding; // for now, will stay as NULL always
	char *packageName; // if not packageName.mak available, needs an absolute path
	
	GtkWidget *gtk_wdw; // global access to gtk widget to update in runtime the gtk window
	GtkWidget *gtk_box; // global access to gtk widget to update in runtime the gtk box layout
	GtkWidget *gtk_lbl; // global access to gtk widget to update in runtime the gtk label
	GtkWidget *gtk_btn; // global access to gtk widget to update in runtime the gtk button
	GtkWidget *gtk_cmb; // global access to gtk widget to update in runtime the gtk combo box text
	
	//struct GtkUserData* next;
} GtkUserData;

/*
// function definitions
*/

// return: path
// get the current working directory
char* current_dir() {
	size_t size = PATH_MAX; // initial buffer size
	char *buffer = (NULL);
	// dynamically allocate memory for the path
	buffer = (char*)malloc((size + 1) * sizeof(char)); // +1 for null terminator (\0)
	if (!(buffer)) { // buffer == NULL
		//perror("No se pudo reservar memoria para 'ruta'\n");
		//printf("No se pudo reservar memoria para 'ruta'\n");
		printf("No memory could be allocated for 'path'\n");
		return (NULL);
	}
	// using a dynamically allocated buffer
	// get the current working directory
	#ifdef WIN32
		buffer = _getcwd(buffer, size);
	#else
		// can let getcwd dynamically allocate memory
		// buffer = getcwd(NULL, 0);
		// but in some systems
		// if buffer is NULL, can lead
		// getcwd to undefined behaviour
		buffer = getcwd(buffer, size);
	#endif
	if (!(buffer)) { // buffer == NULL
		// free the dynamically allocated memory
		free(buffer);
		buffer = (NULL);
		return (NULL);
	}
	return (buffer);
}

// allocates a string variable dynamically
// willing to free the memory by yourself
char* str_init(char **str_head, char *str) {//, FILE *outerr) {
	char *str_ptr = (NULL);
	int32_t length = strlen(str);
	//ssize_t length = strlen(str);
	//printf("'%s' %zd\n", str, length);
	str_ptr = (char*)malloc((length + 1) * sizeof(char)); // +1 for null terminator (\0)
	if (!(str_ptr)) { // str_ptr == NULL
		//u_fprintf(outerr, "No se pudo reservar memoria para '%s'\n", (str));
		//printf("No se pudo reservar memoria para '%s'\n", (str));
		printf("No memory could be allocated for '%s'\n", (str));
		free(str_ptr); // free allocated memory
		str_ptr = (NULL);
		// terminate and return control to where is called from
		return (NULL);
	}
	strcpy(&str_ptr[0], str);
	(*str_head) = str_ptr;
	return (*str_head);
}
// replaces a string variable dynamically
// willing to free the memory by yourself
char* str_replace(char **str_head, char *str) {//, FILE *outerr) {
	char *str_ptr = (NULL);
	int32_t length = strlen(str);
	str_ptr = (char*)realloc((*str_head), (length + 1) * sizeof(char)); // +1 for null terminator (\0)
	if (!(str_ptr)) { // str_ptr == NULL
		//u_fprintf(outerr, "No se pudo reemplazar '%s' por '%s'\n", (*str_head), (str));
		//printf("No se pudo reemplazar '%s' por '%s'\n", (*str_head), (str));
		printf("'%s' could not be replaced with '%s'\n", (*str_head), (str));
		free(str_ptr); // free allocated memory
		str_ptr = (NULL);
		// terminate and return control to where is called from
		return (NULL);
	}
	strcpy(&str_ptr[0], str);
	(*str_head) = str_ptr;
	//strcpy(&(*str_head)[0], str); // NOT NECESSARY
	return (*str_head);
}
// concatenates a string to another string variable
// willing to free the dynamic memory by yourself
char* str_concat(char **str_head, char *str_ext) {//, FILE *outerr) {
	char *str_ptr = (NULL);
	int32_t base_len = strlen(*str_head);
	int32_t ext_len = strlen(str_ext);
	int32_t length = (base_len + ext_len + 1); // +1 for null terminator (\0)
	char str_aux[length];
	strcpy(&str_aux[0], (*str_head));
	for (int32_t i = 0; i < ext_len; i++) {
		strncat(str_aux, &str_ext[i], 1);
	}
	str_ptr = str_replace(str_head, str_aux);//, outerr);
	if (!(str_ptr)) { // str_ptr == NULL
		// terminate and return control to where is called from
		return (NULL);
	}
	return (*str_head);
}

// convert UChar to char* (for ASCII-only example)
char* str_from_usrt(char **str, const char *ustr, FILE *outerr) {
	if (!(ustr)) { // ustr == NULL
		return (NULL);
	}
	const char *key_ustr = (ustr);
	//UChar *key_ustr = quotedString(ustr);
	int32_t len = strlen(key_ustr);
	//u_fprintf(outerr, "UNICODE has length '%ld' for '%s'\n", strlen(key_ustr), key_ustr);
	//char key_str[(len+1)];
	char *key_str = (NULL);
	// (len * 4) because each symbol in UNICODE can be longer in UTF-8
	key_str = (char*)malloc(((len + 1) * 4) * sizeof(char)); // +1 for null terminator (\0)
	if (!(key_str)) { // key_str == NULL
		//printf("No se pudo reservar memoria para '%s'\n", (ustr));
		//u_fprintf(outerr, "No se pudo reservar memoria para '%S'\n", (ustr));
		//u_fprintf(outerr, "No memory could be allocated for '%S'\n", (ustr));
		fprintf(outerr, "No memory could be allocated for '%s'\n", (ustr));
		free(key_str); // free allocated memory
		key_str = (NULL);
		return (NULL);
	}
	// destiny can be static or dynamically allocated
	// but strcpy does not dynamically allocate memory
	//char key_str[((len + 1) * 4)];
	//char *key_str = (char*)malloc(((len + 1) * 4) * sizeof(char)); // +1 for null terminator (\0)
	// needs to know the length/size and have enough memory to hold the source
	// if destiny buffer is too small, can lead strcpy to undefined behavior
	//strcpy(&key_str[0], key_ustr);
	// strdup dynamically allocate memory
	// willing to free the memory by yourself
	//char *key_str = strdup(key_ustr);
	// copy ustring (UChar) to a string encoded in the default codepage (usually UTF-8)
	key_str = strcpy(&key_str[0], key_ustr);
	//free(key_ustr); // only if using key_ustr = quotedString(ustr);
	//printf("  UTF-8 has length '%ld' for '%s'\n", strlen(key_str), key_str);
	(*str) = key_str;
	return (*str);
}

// get a ustring in UNICODE (UChar) from a locale resource given a key
const char* ustr_by_key(char *key) {//char *packageName, char *locale, char *key, FILE *outerr) {
	const char *key_val = (NULL); // ustring in UNICODE codepage (UChar)
	key_val = _(key);
	//fprintf(outerr, "Locale '%s' is localized in path '%s/%s/LC_MESSAGES'\n", locale, packageName, locale);
	return (key_val);
}

// get a string in default codepage (usually UTF-8) from a locale resource given a key
char* str_by_key(char *locale, char *key, FILE *outerr) {//char *packageName, char *locale, char *key, FILE *out, FILE *outerr) {
	const char *key_ustr = (NULL);
	key_ustr = ustr_by_key(key);//packageName, locale, key, outerr);
	if (!(key_ustr)) { // key_ustr == NULL
		return (NULL);
	}
	//u_fprintf(out, "UNICODE in locale '%s' has length '%ld' for '%S'\n", locale, u_strlen(key_ustr), key_ustr);
	char *key_str = (NULL);
	key_str = str_from_usrt((&key_str), (key_ustr), (outerr));
	if (!(key_str)) { // key_str == NULL
		return (NULL);
	}
	fprintf(outerr, "  UTF-8 in locale '%s' has length '%ld' for '%s'\n", locale, strlen(key_str), key_str);
	return (key_str);
}

// return: gtkeys
// allocate memory to create the new gtkeys
L10nKeys* gtkeys_create() {
	L10nKeys *gtkeys = (NULL);
	gtkeys = (L10nKeys*)malloc(sizeof(L10nKeys));
	if (!(gtkeys)) { // gtkeys == NULL
		//perror("No se pudo reservar memoria para 'L10nKeys'\n");
		//printf("No se pudo reservar memoria para 'L10nKeys'\n");
		printf("No memory could be allocated for 'L10nKeys'\n");
		free(gtkeys);
		gtkeys = (NULL);
		return (NULL);
	}
	(gtkeys)->wdw_title = "Internationalization";
	(gtkeys)->greeting = "Hello!";
	(gtkeys)->farewell = "Goodbye!";
	(gtkeys)->welcome = "Welcome!";
	(gtkeys)->lbl_lang = "Language";
	(gtkeys)->cmd_lang_root = "Select";
	(gtkeys)->cmd_lang_ar = "Arabic";
	(gtkeys)->cmd_lang_de = "German";
	(gtkeys)->cmd_lang_en = "English";
	(gtkeys)->cmd_lang_es = "Spanish";
	(gtkeys)->cmd_lang_fr = "French";
	(gtkeys)->cmd_lang_hi = "Hindi";
	(gtkeys)->cmd_lang_ja = "Japanese";
	(gtkeys)->cmd_lang_ru = "Russian";
	(gtkeys)->cmd_lang_sr = "Serbian";
	(gtkeys)->cmd_lang_zh = "Chinese CN";
	(gtkeys)->cmd_lang_zh_TW = "Chinese TW";
	(gtkeys)->nlangs = "%d language is available.";
	(gtkeys)->nlangs_plural = "%d languages are available.";
	return (gtkeys);
}

void init_langs_avail(int nlangs, char *langs_usrt[nlangs]) {
	for (int i = 0; i < nlangs; i++) {
		langs_usrt[i] = (NULL); // Initialize the pointer to NULL before any operation
	}
}

void free_langs_avail(int nlangs, char *langs_usrt[nlangs]) {
	for (int i = 0; i < nlangs; i++) {
		if(langs_usrt[i] != NULL) {
			free(langs_usrt[i]); // Free the memory pointed to by each element
		}
		langs_usrt[i] = (NULL); // Set the pointer to NULL after freeing for safety
	}
	// Free the array of char pointers itself
	// only when the array is initialized as double pointer
	// char **langs_usrt = (char**)malloc(nlangs * sizeof(char*));
	//free(langs_usrt);
	//langs_usrt = (NULL); // Set the main array pointer to NULL after freeing
}

void update_langs_avail(L10nKeys *gtkeys, int nlangs, char *langs_usrt[nlangs], char *locale, FILE *outerr) {//, char *packageName, FILE *out, FILE *outerr) {
	//free_langs_avail(nlangs, langs_usrt);
	langs_usrt[0] = ""; // can not be static, needs to be dynamic to not fail free
	//char *blank = (NULL);
	//while(!str_init((&blank), ""));
	//langs_usrt[0] = blank;
	/*
	langs_usrt[0] = str_by_key(packageName, locale, gtkeys->cmd_lang_root, out, outerr);
	langs_usrt[1] = str_by_key(packageName, locale, gtkeys->cmd_lang_ar, out, outerr);
	langs_usrt[2] = str_by_key(packageName, locale, gtkeys->cmd_lang_de, out, outerr);
	langs_usrt[3] = str_by_key(packageName, locale, gtkeys->cmd_lang_en, out, outerr);
	langs_usrt[4] = str_by_key(packageName, locale, gtkeys->cmd_lang_es, out, outerr);
	langs_usrt[5] = str_by_key(packageName, locale, gtkeys->cmd_lang_fr, out, outerr);
	langs_usrt[6] = str_by_key(packageName, locale, gtkeys->cmd_lang_hi, out, outerr);
	langs_usrt[7] = str_by_key(packageName, locale, gtkeys->cmd_lang_ja, out, outerr);
	langs_usrt[8] = str_by_key(packageName, locale, gtkeys->cmd_lang_ru, out, outerr);
	langs_usrt[9] = str_by_key(packageName, locale, gtkeys->cmd_lang_sr, out, outerr);
	langs_usrt[10] = str_by_key(packageName, locale, gtkeys->cmd_lang_zh, out, outerr);
	langs_usrt[11] = str_by_key(packageName, locale, gtkeys->cmd_lang_zh_TW, out, outerr);
	*/
	langs_usrt[0] = str_by_key(locale, gtkeys->cmd_lang_root, outerr);
	langs_usrt[1] = str_by_key(locale, gtkeys->cmd_lang_ar, outerr);
	langs_usrt[2] = str_by_key(locale, gtkeys->cmd_lang_de, outerr);
	langs_usrt[3] = str_by_key(locale, gtkeys->cmd_lang_en, outerr);
	langs_usrt[4] = str_by_key(locale, gtkeys->cmd_lang_es, outerr);
	langs_usrt[5] = str_by_key(locale, gtkeys->cmd_lang_fr, outerr);
	langs_usrt[6] = str_by_key(locale, gtkeys->cmd_lang_hi, outerr);
	langs_usrt[7] = str_by_key(locale, gtkeys->cmd_lang_ja, outerr);
	langs_usrt[8] = str_by_key(locale, gtkeys->cmd_lang_ru, outerr);
	langs_usrt[9] = str_by_key(locale, gtkeys->cmd_lang_sr, outerr);
	langs_usrt[10] = str_by_key(locale, gtkeys->cmd_lang_zh, outerr);
	langs_usrt[11] = str_by_key(locale, gtkeys->cmd_lang_zh_TW, outerr);
}

void print_langs_avail(int nlangs, char *langs_usrt[nlangs]) {
	//printf("%s\n", "--------------- IDIOMAS DISPONIBLES ---------------");
	printf("%s\n", "-------------- LANGUAGES - AVAILABLE --------------");
	for (int i = 0; i < nlangs; i++) {
		if(langs_usrt[i] != NULL) printf("%2d %s\n", i, langs_usrt[i]);
	}
	//printf("%s\n", "------------- ÚLTIMA LÍNEA DE IDIOMAS -------------");
	printf("%s\n", "-------------- LANGUAGES - LAST LINE --------------");
}

// len_lang = 2 for 'en' and len_lang = 5 for 'en_US'
char* lang_from_loc(char **str_lang, char *str_loc, int32_t len_lang) {
	if (!(str_loc)) { // str_ptr == NULL
		// can not be static, needs to be dynamic to not fail free
		char *blank = (NULL);
		while(!str_init((&blank), ""));
		(*str_lang) = blank;
		//return (""); // if static, then free fails
		return (*str_lang);
	}
	//int32_t len_loc = strlen(str_loc);
	//int32_t len_lang = 2;
	char *str_ptr = (NULL);
	// destiny can be static or dynamically allocated
	// but strcpy does not dynamically allocate memory
	//char str_ptr[len_lang];
	//char *str_ptr = (char*)malloc((len_lang + 1) * sizeof(char)); // +1 for null terminator (\0)
	// needs to know the length/size and have enough memory to hold the source
	// if destiny buffer is too small, can lead strcpy to undefined behavior
	//strcpy(&str_ptr[0], str_loc);
	// strdup dynamically allocate memory
	// willing to free the memory by yourself
	//char *str_ptr = strdup(str_loc);
	str_ptr = strndup(str_loc, len_lang);
	//str_ptr = (char*)malloc((len_lang + 1) * sizeof(char)); // +1 for null terminator (\0)
	if (!(str_ptr)) { // str_ptr == NULL
		//u_fprintf(outerr, "No se pudo reservar memoria para '%s'\n", (str_loc));
		//printf("No se pudo reservar memoria para '%s'\n", (str_loc));
		printf("No memory could be allocated for '%s'\n", (str_loc));
		free(str_ptr); // free allocated memory
		str_ptr = (NULL);
		// terminate and return control to where is called from
		// can not be static, needs to be dynamic to not fail free
		char *blank = (NULL);
		while(!str_init((&blank), ""));
		(*str_lang) = blank;
		//return (""); // if static, then free fails
		return (*str_lang);
	}
	// usgin strndup because strncpy is copying garbage
	//strncpy(&str_ptr[0], str_loc, (len_lang));
	//printf("lang_from_loc '%s'\n", str_ptr);
	//str_ptr[len_lang] = '\0'; // explicitly null terminated (\0)
	//printf("lang_from_loc '%s'\n", str_ptr);
	(*str_lang) = str_ptr;
	return (*str_lang);
}

char* loc_from_lang(int nlangs, char *langs_avail[nlangs], char *locale) {
	char *locale_avail = ""; // stores the last available locale, as locales may not be enable in /etc/locale.gen
	char *locale_temp = "";
	char* lang_curr_2 = (NULL);
	char* lang_curr_5 = (NULL);
	lang_curr_2 = lang_from_loc(&lang_curr_2, locale, 2);
	lang_curr_5 = lang_from_loc(&lang_curr_5, locale, 5);
	// Look if locale is a language available to set as the active one
	for (int i = 0; i < nlangs; i++) {
		//printf("locale '%s' language available %2d %s\n", locale, i, langs_avail[i]);
		char* lang_temp_2 = (NULL);
		char* lang_temp_5 = (NULL);
		lang_temp_2 = lang_from_loc(&lang_temp_2, langs_avail[i], 2);
		lang_temp_5 = lang_from_loc(&lang_temp_5, langs_avail[i], 5);
		if(strcmp(lang_curr_5, lang_temp_5) == 0) {
			locale_avail = langs_avail[i];
			if (lang_temp_2 != NULL) {
				free(lang_temp_2);
				lang_temp_2 = (NULL);
			}
			if (lang_temp_5 != NULL) {
				free(lang_temp_5);
				lang_temp_5 = (NULL);
			}
			break;
		} else if(strcmp(locale_temp, "") == 0 && strcmp(lang_curr_2, lang_temp_2) == 0) {
			locale_temp = langs_avail[i];
		}
		if (lang_temp_2 != NULL) {
			free(lang_temp_2);
			lang_temp_2 = (NULL);
		}
		if (lang_temp_5 != NULL) {
			free(lang_temp_5);
			lang_temp_5 = (NULL);
		}
	}
	if(strcmp(locale_avail, "") == 0) {
		locale_avail = locale_temp;
	}
	if (lang_curr_2 != NULL) {
		free(lang_curr_2);
		lang_curr_2 = (NULL);
	}
	if (lang_curr_5 != NULL) {
		free(lang_curr_5);
		lang_curr_5 = (NULL);
	}
	//printf("loc_from_lang '%s'\n", locale_avail);
	return (locale_avail);
}

bool is_lang_avail(int nlangs, char *langs_avail[nlangs], char *locale, int *cmb_lang_active) {
	bool is_avail = false;
	int active_avail = -1;
	int active_temp = -1;
	char* lang_curr_2 = (NULL);
	char* lang_curr_5 = (NULL);
	lang_curr_2 = lang_from_loc(&lang_curr_2, locale, 2);
	lang_curr_5 = lang_from_loc(&lang_curr_5, locale, 5);
	// Look if locale is a language available to set as the active one
	for (int i = 0; i < nlangs; i++) {
		//printf("locale '%s' language available %2d %s\n", locale, i, langs_avail[i]);
		char* lang_temp_2 = (NULL);
		char* lang_temp_5 = (NULL);
		lang_temp_2 = lang_from_loc(&lang_temp_2, langs_avail[i], 2);
		lang_temp_5 = lang_from_loc(&lang_temp_5, langs_avail[i], 5);
		if(strcmp(lang_curr_5, lang_temp_5) == 0) {
			active_avail = i;
			if (lang_temp_2 != NULL) {
				free(lang_temp_2);
				lang_temp_2 = (NULL);
			}
			if (lang_temp_5 != NULL) {
				free(lang_temp_5);
				lang_temp_5 = (NULL);
			}
			break;
		} else if(active_temp < 0 && strcmp(lang_curr_2, lang_temp_2) == 0) {
			active_temp = i;
		}
		if (lang_temp_2 != NULL) {
			free(lang_temp_2);
			lang_temp_2 = (NULL);
		}
		if (lang_temp_5 != NULL) {
			free(lang_temp_5);
			lang_temp_5 = (NULL);
		}
	}
	if(0 <= active_avail) {
		(*cmb_lang_active) = active_avail;
		is_avail = true;
	} else if (0 <= active_temp) {
		(*cmb_lang_active) = active_temp;
		is_avail = true;
	}
	if (lang_curr_2 != NULL) {
		free(lang_curr_2);
		lang_curr_2 = (NULL);
	}
	if (lang_curr_5 != NULL) {
		free(lang_curr_5);
		lang_curr_5 = (NULL);
	}
	//printf("is_lang_avail '%d' for locale '%s' index '%d'\n", is_avail, locale, (*cmb_lang_active));
	return (is_avail);
}

bool update_lang_env(char *locale, char *locale_default) {
	bool fail_env = false;
	if (!(locale) || strcmp(locale, "") == 0) {
		if (!(locale_default) || strcmp(locale_default, "") == 0) {
			// Get the value of the LANG environment variable
			char *lang_env_var = getenv("LANG");
			// Check if the environment variable was found
			if (lang_env_var != NULL) {
				printf("LANG environment variable stay as '%s' because can not be NULL\n", lang_env_var);
			} else {
				printf("LANG environment variable stay as is because can not be NULL\n");
			}
			return (false);
		} else {
			locale = locale_default;
		}
	}
	// Set the LANG environment is a must when using GTK
	// Set the LANG environment variable to "en_US.UTF-8"
    // The third argument (1) indicates to overwrite if the variable already exists
    //if (setenv("LANG", "en_US.UTF-8", 1) == 0) {
    if (setenv("LANG", locale, 1) == 0) {
        // Variable successfully set
		printf("LANG environment for GTK success updated to '%s'\n", locale);
    } else {
        // Error setting variable
		printf("LANG environment for GTK fail updating to '%s'\n", locale);
		fail_env = true;
    }
	return (fail_env);
}

bool update_locale(char *locale, char *locale_default) {
	// get the current locale for all categories (LC_ALL)
	char *locale_null = setlocale(LC_ALL, NULL);
	if (!(locale) || strcmp(locale, "") == 0) {
		if (!(locale_default)) {
			printf("LC_ALL locale stay as '%s' because can not be NULL\n", locale_null);
			return (false);
		} else {
			locale = locale_default;
		}
	}
	//update_lang_env(locale, locale_default);
	char *locale_original = (NULL); // keep a copy to not get garbage from setlocale
	char *locale_curr = (NULL);
	bool is_avail = true;
	//printf("start update_locale '%d' for locale '%s' current '%s'\n", is_avail, locale, locale_null);
	if (locale_null != NULL) {
		printf("LC_ALL locale is currently '%s'\n", locale_null);
		locale_original = strdup(locale_null); // keep a copy to not get garbage from setlocale
	} else {
		printf("Failed to get LC_ALL locale\n");
		locale_original = strdup(""); // keep a copy to not get garbage from setlocale
	}
	// set the locale to support Unicode UTF-8 (or your desired locale)
	//char *locale_curr = setlocale(LC_NUMERIC, "en_US.UTF-8"); // for using a thousands separator
	//char *locale_curr = setlocale(LC_ALL, "en_US.UTF-8"); // for using a thousands separator
	locale_curr = setlocale(LC_ALL, locale); // setting locale through a defined constant
	//char *locale_curr = setlocale(LC_CTYPE, ""); // for character handling to the user's default
	//char *locale_curr = setlocale(LC_ALL, ""); // set to the user environment's default locale
	if (locale_curr != NULL) {
		printf("LC_ALL locale changed from '%s' to '%s'\n", locale_original, locale_curr);
	} else {
		if(strcmp(locale_original, locale) == 0) {
			printf("LC_ALL locale from '%s' to '%s' stay equals\n", locale_original, locale);
		} else {
			printf("Failed to change LC_ALL locale from '%s' to '%s'\n", locale_original, locale);
			is_avail = false;
		}
		locale_curr = locale_original;
	}
	//printf("end update_locale '%d' for locale '%s' current '%s'\n", is_avail, locale, locale_curr);
	if (!(locale_original)) {
		free(locale_original);
		locale_original = (NULL);
	}
	return (is_avail);
}

bool is_loc_avail(char *locale, char *locale_default) {
	// get the current locale for all categories (LC_ALL)
	char *locale_null = setlocale(LC_ALL, NULL);
	if (!(locale)) {
		printf("LC_ALL locale is not available because can not be NULL\n");
		return (false);
	}
	// keep a copy to not get garbage from setlocale
	char *locale_original = strdup(locale_null);
	char *locale_curr = (NULL);
	//char *locale_prev = (NULL);
	bool is_avail = true;
	//printf("start is_loc_avail '%d' for locale '%s' current '%s'\n", is_avail, locale, locale_null);
	is_avail = update_locale(locale, locale_default);
	if (is_avail) {
		locale_null = setlocale(LC_ALL, NULL);
		//printf("LC_ALL locale previous was '%s'\n", locale_null);
		locale_curr = strdup(locale_null); // keep a copy to not get garbage from setlocale
		setlocale(LC_ALL, locale_original); // set to original locale to leave unchanged
		/*
		locale_prev = setlocale(LC_ALL, locale_original); // set to original locale to leave unchanged
		if (locale_prev != NULL) {
			printf("LC_ALL locale changed from '%s' to '%s'\n", locale_curr, locale_prev);
		} else {
			if(strcmp(locale_original, locale_curr) == 0) {
				printf("LC_ALL locale from '%s' to '%s' stay equals\n", locale_curr, locale_original);
			} else {
				printf("Failed to change LC_ALL locale from '%s' to '%s'\n", locale_curr, locale_original);
			}
			locale_prev = locale_curr;
		}
		*/
	}
	//printf("end is_loc_avail '%d' for locale '%s' current '%s'\n", is_avail, locale, locale_prev);
	if (!(locale_original)) {
		free(locale_original);
		locale_original = (NULL);
	}
	if (!(locale_curr)) {
		free(locale_curr);
		locale_curr = (NULL);
	}
	return (is_avail);
}

// return: gudata
// allocate memory to create the new gudata
GtkUserData* gudata_create(L10nKeys **gtkeys, FILE **out, FILE **outerr, int *nlangs, char *langs_avail[*nlangs], char *langs_usrt[*nlangs], int *cmb_lang_active, int cmb_lang_default, char **locale_avail, char **locale, char *locale_default, char **encoding, char **packageName, GtkWidget **gtk_wdw, GtkWidget **gtk_box, GtkWidget **gtk_lbl, GtkWidget **gtk_btn, GtkWidget **gtk_cmb) {
	GtkUserData *gudata = (NULL);
	gudata = (GtkUserData*)malloc(sizeof(GtkUserData));
	if (!(gudata)) { // gudata == NULL
		//perror("No se pudo reservar memoria para 'GtkUserData'\n");
		//printf("No se pudo reservar memoria para 'GtkUserData'\n");
		printf("No memory could be allocated for 'GtkUserData'\n");
		free(gudata);
		gudata = (NULL);
		return (NULL);
	}
	(gudata)->gtkeys = (*gtkeys); // used by gettext to get localized messages by key
	
	(gudata)->out = (*out); // used by fprintf to print localized messages to stdout
	(gudata)->outerr = (*outerr); // used by fprintf to print localized messages to stderr
	
	(gudata)->langs_total = (nlangs); // total quantity of languages available to select from combo box in gtk
	(gudata)->langs_avail = (langs_avail); // array to store codes of languages available to select from combo box in gtk
	(gudata)->langs_usrt = (langs_usrt); // array to update combo box list in gtk according to selected language
	(gudata)->cmb_lang_active = (cmb_lang_active); // global variable to store the index to last selected language from combo box in gtk
	(gudata)->cmb_lang_default = (cmb_lang_default); // constant to default index in case lang is not enable for combo box in gtk
	
	(gudata)->locale_avail = (*locale_avail); // stores the last available locale, as locales may not be enable in /etc/locale.gen
	//(gudata)->locale = "en_US.UTF-8"; // this will store the current locale or last selected -> NOT WORKING ON ARCH
	//(gudata)->locale = "en_US.utf8"; // this will store the current locale or last selected -> WORKING ON ARCH LINUX
	(gudata)->locale = (*locale); // this will store the current locale or last selected -> BUT BETTER WORK WITH DEFAULT LANG
	(gudata)->locale_default = (locale_default); // this will store the default locale in case a locale is not enable in /etc/locale.gen
	(gudata)->encoding = (*encoding); // for now, will stay as NULL always
	(gudata)->packageName = (*packageName); // if not packageName.mak available, needs an absolute path
	
	(gudata)->gtk_wdw = (*gtk_wdw); // global access to gtk widget to update in runtime the gtk window
	(gudata)->gtk_box = (*gtk_box); // global access to gtk widget to update in runtime the gtk box layout
	(gudata)->gtk_lbl = (*gtk_lbl); // global access to gtk widget to update in runtime the gtk label
	(gudata)->gtk_btn = (*gtk_btn); // global access to gtk widget to update in runtime the gtk button
	(gudata)->gtk_cmb = (*gtk_cmb); // global access to gtk widget to update in runtime the gtk combo box text
	return (gudata);
}

void on_button_click(GtkButton *btn, gpointer user_data) {
	GtkUserData *gudata = (GtkUserData*)user_data;
    // Get the current label of the button
    const gchar *btn_glbl = gtk_button_get_label(btn);
	const char *btn_lbl = btn_glbl;
	// Print the button label based on its current state
	if (g_strcmp0(btn_glbl, "") != 0) {
		//g_print("Se ha capturado evento de click sobre el botón '%s'!\n", btn_lbl);
		g_print("'%s' event on button '%s' has been captured!\n", "click", btn_lbl);
	} else {
		//g_print("Se ha capturado evento de click sobre un botón!\n");
		g_print("'%s' event on a button has been captured!\n", "click");
	}
	//char *key_str = str_by_key(gudata->packageName, gudata->locale, gudata->gtkeys->greeting, gudata->out, gudata->outerr);
	char *key_str = str_by_key(gudata->locale, gudata->gtkeys->greeting, gudata->outerr);
	if (key_str != NULL) free(key_str);
}

// Callback function for a button-press-event
gboolean on_button_press(GtkWidget *widget, GdkEvent *event, gpointer user_data) {
	// You can determine the event name based on the event->type
	const char *event_name = (NULL);
	int event_type = event->type;
	switch (event_type) {
		case GDK_BUTTON_PRESS:
			event_name = "button-press-event";
		break;
		case GDK_BUTTON_RELEASE:
			event_name = "button-release-event";
		break;
		case GDK_KEY_PRESS:
			event_name = "key-press-event";
		break;
		case GDK_KEY_RELEASE:
			event_name = "key-release-event";
		break;
		// Add more cases for other event types as needed
		default:
			event_name = "unknown-event";
		break;
	}
	GdkEventButton *btn_event = (GdkEventButton*)event;
	g_print("'%s' event on button '%u' has been captured!\n", event_name, btn_event->button);
	GtkButton *btn = GTK_BUTTON(widget);
	on_button_click(btn, user_data);
	return (false); // return (FALSE) to allow other handlers to be called
}

gboolean on_delete_wdw(GtkWidget *widget, GdkEvent *event, gpointer user_data) {
	GtkUserData *gudata = (GtkUserData*)user_data;
	// Retrieve the title
	GtkWindow *wdw = GTK_WINDOW(widget);
    const gchar *wdw_title = gtk_window_get_title(wdw);
	//g_print("Se ha capturado evento previo a cerrar la ventana!\n");
	//g_print("An event prior to quit the window has been captured!\n");
	g_print("'%d' event prior to quit window '%s' has been captured!\n", event->type, wdw_title);
	//char *key_str = str_by_key(gudata->packageName, gudata->locale, gudata->gtkeys->farewell, gudata->out, gudata->outerr);
	char *key_str = str_by_key(gudata->locale, gudata->gtkeys->farewell, gudata->outerr);
	if (key_str != NULL) free(key_str);
	return (false); // continues the trigger to the destroy event
	//return (true); // stop the destroy event to not quit the window
}

void on_destroy_wdw(GtkWidget *widget, gpointer user_data) {
	GtkUserData *gudata = (GtkUserData*)user_data;
	// Retrieve the title
	GtkWindow *wdw = GTK_WINDOW(widget);
    const gchar *wdw_title = gtk_window_get_title(wdw);
	//g_print("Se ha capturado evento último que cierra la ventana!\n");
	//g_print("Last event that quit the window has been captured!\n");
	g_print("'%s' event that quit window '%s' has been captured!\n", "last", wdw_title);
	//char *key_str = str_by_key(gudata->packageName, gudata->locale, gudata->gtkeys->farewell, gudata->out, gudata->outerr);
	char *key_str = str_by_key(gudata->locale, gudata->gtkeys->farewell, gudata->outerr);
	if (key_str != NULL) free(key_str);
	gtk_main_quit(); // in gtk-3
	//g_main_quit(); // in gtk-4
}

void redraw_gtk_widgets(GtkUserData *gudata) {
	// getting locale from languages available
	char *loc_avail = loc_from_lang((*gudata->langs_total), gudata->langs_avail, gudata->locale);
	// setting locale through a defined constant
	bool is_avail = update_locale(loc_avail, gudata->locale_default);
	//printf("available '%d' for locale '%s', default is '%s'\n", is_avail, loc_avail, gudata->locale_default);
	if (is_avail) {
		gudata->locale_avail = loc_avail;
		update_lang_env(loc_avail, gudata->locale_default);
	} else {
		gudata->locale_avail = gudata->locale_default;
		update_lang_env(gudata->locale_default, gudata->locale_default);
	}
	// mark a widget as needing a redraw (this case will be the window)
	// gtk will schedule a redraw for the widget during the next main loop iteration
	//gtk_widget_queue_draw(gudata->gtk_wdw);
	
	// Update the window's title
	//char *wdw_title = str_by_key(gudata->packageName, gudata->locale, gudata->gtkeys->wdw_title, gudata->out, gudata->outerr);
	char *wdw_title = str_by_key(gudata->locale, gudata->gtkeys->wdw_title, gudata->outerr);
	gtk_window_set_title(GTK_WINDOW(gudata->gtk_wdw), wdw_title);
	if (wdw_title != NULL) free(wdw_title);
	
	// Update the label's text
	GtkLabel *lbl = GTK_LABEL(gudata->gtk_lbl);
	//char *lbl_head = str_by_key(gudata->packageName, gudata->locale, gudata->gtkeys->welcome, gudata->out, gudata->outerr);
	char *lbl_head = str_by_key(gudata->locale, gudata->gtkeys->welcome, gudata->outerr);
	gtk_label_set_text(lbl, lbl_head);
	if (lbl_head != NULL) free(lbl_head);
	
	// Update the button's label
	// Get the current label of the button
	// Cast user_data to GtkButton* to access the button
	//GtkButton *btn = GTK_BUTTON(user_data);
	//GtkButton *btn = GTK_BUTTON(gudata->gtk_btn);
	// Change the button label based on its current state
	//char *lbl_lang = str_by_key(gudata->packageName, gudata->locale, gudata->gtkeys->lbl_lang, gudata->out, gudata->outerr);
	char *lbl_lang = str_by_key(gudata->locale, gudata->gtkeys->lbl_lang, gudata->outerr);
	gtk_button_set_label(GTK_BUTTON(gudata->gtk_btn), lbl_lang);
	if (lbl_lang != NULL) free(lbl_lang);
	
	// Update the combo's elements
	//update_langs_avail(gudata->gtkeys, (*gudata->langs_total), gudata->langs_usrt, gudata->locale, gudata->packageName, gudata->out, gudata->outerr);
	update_langs_avail(gudata->gtkeys, (*gudata->langs_total), gudata->langs_usrt, gudata->locale, gudata->outerr);//, gudata->packageName, gudata->out, gudata->outerr);
	print_langs_avail((*gudata->langs_total), gudata->langs_usrt);
	GtkComboBox *cmb = GTK_COMBO_BOX(gudata->gtk_cmb);
	int active = gtk_combo_box_get_active(cmb);
	if (active != -1) {
		g_print("Last active language was %2d, now %2d has been activated!\n", (*gudata->cmb_lang_active), active);
		(*gudata->cmb_lang_active) = active;
	}
	// Clear existing items
	GtkComboBoxText *cmb_txt = GTK_COMBO_BOX_TEXT(gudata->gtk_cmb);
	gtk_combo_box_text_remove_all(cmb_txt);
	// G_N_ELEMENTS is a macro which determines the number of elements in an array
	//for (size_t i = 0; i < G_N_ELEMENTS(gudata->langs_usrt); i++) {
	for (int i = 0; i < (*gudata->langs_total); i++) {
		gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(gudata->gtk_cmb), !((gudata->langs_usrt)[i]) ? (gudata->langs_avail)[i] : (gudata->langs_usrt)[i]);
	}
	// Choose to set the first row as the active one by default, from the beginning
	//gtk_combo_box_set_active(GTK_COMBO_BOX(gudata->gtk_cmb), 0);
	// gettext needs this validation because GTK needs the locale enable in /etc/locale.gen
	if (is_avail) {
		// Look if locale is a language available to set as the active one in combo box
		if (is_lang_avail((*gudata->langs_total), gudata->langs_avail, gudata->locale, (gudata->cmb_lang_active))) {
			gtk_combo_box_set_active(GTK_COMBO_BOX(gudata->gtk_cmb), (*gudata->cmb_lang_active));
		}
	} else {
		// If there is no language available for locale, set the default one in combo box
		gtk_combo_box_set_active(GTK_COMBO_BOX(gudata->gtk_cmb), (gudata->cmb_lang_default));
	}
	/*
	// ICU always loads the resource even if the locale is not enable in /etc/locale.gen
	// Look if locale is a language available to set as the active one in combo box
	if (is_lang_avail((*gudata->langs_total), gudata->langs_avail, gudata->locale, (gudata->cmb_lang_active))) {
		gtk_combo_box_set_active(GTK_COMBO_BOX(gudata->gtk_cmb), (*gudata->cmb_lang_active));
	}
	*/
}

// This is the callback function. It is a handler function which reacts to the signal.
// In this case, if the row selected is not the first one of the ComboBox, 
// we write its value in the terminal for the user.
void on_change_cmb(GtkComboBox *widget, gpointer user_data) {
	GtkUserData *gudata = (GtkUserData*)user_data;
	//GtkComboBox *cmb = widget;
	GtkComboBox *cmb = GTK_COMBO_BOX(widget);
	int active = gtk_combo_box_get_active(cmb);
	//g_print("Last active language was %2d, now %2d is being requested!\n", (*gudata->cmb_lang_active), active);
	if (active != -1 && active != (*gudata->cmb_lang_active)) {
		int i = gtk_combo_box_get_active(cmb);
		//if (i != 0) { // i = 0 = Select root as default
			gudata->locale = (gudata->langs_avail)[i];
			gchar *lang = gtk_combo_box_text_get_active_text(GTK_COMBO_BOX_TEXT(cmb));
			//g_print("Se ha seleccionado el idioma '%s' con código '%s'!\n", lang, gudata->locale);
			g_print("Language '%s' with code '%s' has been selected!\n", lang, gudata->locale);
			g_free(lang);
		//}
		redraw_gtk_widgets(gudata);
	}
}

void on_activate_wdw(GtkUserData *gudata) {//GtkApplication *app, gpointer user_data) {
	// getting locale from languages available
	char *loc_avail = loc_from_lang((*gudata->langs_total), gudata->langs_avail, gudata->locale);
	// setting locale through a defined constant
	bool is_avail = update_locale(loc_avail, gudata->locale_default);
	//printf("available '%d' for locale '%s', default is '%s'\n", is_avail, loc_avail, gudata->locale_default);
	if (is_avail) {
		gudata->locale_avail = loc_avail;
		update_lang_env(loc_avail, gudata->locale_default);
	} else {
		gudata->locale_avail = gudata->locale_default;
		update_lang_env(gudata->locale_default, gudata->locale_default);
	}
	//GtkWidget *gtk_wdw;
	//GtkWidget *gtk_box;
	//GtkWidget *gtk_lbl;
	//GtkWidget *gtk_btn;
	//GtkWidget *gtk_cmb;
	// Create a new window
	//gudata->gtk_wdw = gtk_application_window_new(app); // gtk-4
	gudata->gtk_wdw = gtk_window_new(GTK_WINDOW_TOPLEVEL); // gtk-3
	//gtk_window_set_title(GTK_WINDOW(gudata->gtk_wdw), "Internationalization");
	//char *wdw_title = str_by_key(gudata->packageName, gudata->locale, gudata->gtkeys->wdw_title, gudata->out, gudata->outerr);
	char *wdw_title = str_by_key(gudata->locale, gudata->gtkeys->wdw_title, gudata->outerr);
	gtk_window_set_title(GTK_WINDOW(gudata->gtk_wdw), wdw_title);
	if (wdw_title != NULL) free(wdw_title);
	//gtk_window_set_default_size(GTK_WINDOW(gudata->gtk_wdw), -1, -1);
	//gtk_window_set_default_size(GTK_WINDOW(gudata->gtk_wdw), 320, 180);
	gtk_window_set_default_size(GTK_WINDOW(gudata->gtk_wdw), 320, -1);
	gtk_container_set_border_width(GTK_CONTAINER(gudata->gtk_wdw), 10);
	g_signal_connect(G_OBJECT(gudata->gtk_wdw), "delete_event", G_CALLBACK(on_delete_wdw), gudata);
	g_signal_connect(G_OBJECT(gudata->gtk_wdw), "destroy", G_CALLBACK(on_destroy_wdw), gudata);
	
	gudata->gtk_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 20);
	gtk_widget_set_halign(gudata->gtk_box, GTK_ALIGN_CENTER);
	gtk_widget_set_valign(gudata->gtk_box, GTK_ALIGN_CENTER);
	//gtk_window_set_child(GTK_WINDOW(gudata->gtk_wdw), gudata->gtk_box); // gtk-4
	gtk_container_add(GTK_CONTAINER(gudata->gtk_wdw), gudata->gtk_box); // gtk-3
	
	// Create a label widget
	//char *lbl_head = str_by_key(gudata->packageName, gudata->locale, gudata->gtkeys->welcome, gudata->out, gudata->outerr);
	char *lbl_head = str_by_key(gudata->locale, gudata->gtkeys->welcome, gudata->outerr);
	gudata->gtk_lbl = gtk_label_new(lbl_head);
	if (lbl_head != NULL) free(lbl_head);
	
	// Create a new button
	//gudata->gtk_btn = gtk_button_new_with_label("Language:");
	//char *lbl_lang = str_by_key(gudata->packageName, gudata->locale, gudata->gtkeys->lbl_lang, gudata->out, gudata->outerr);
	char *lbl_lang = str_by_key(gudata->locale, gudata->gtkeys->lbl_lang, gudata->outerr);
	gudata->gtk_btn = gtk_button_new_with_label(lbl_lang);
	if (lbl_lang != NULL) free(lbl_lang);
    // Set a minimum size request for the button
    gtk_widget_set_size_request(gudata->gtk_btn, -1, -1); // 160px width, 30px height
	//g_signal_connect(gudata->gtk_btn, "clicked", G_CALLBACK(on_button_click), gudata);
	// Connect the button-press-event signal to the handler
	g_signal_connect(gudata->gtk_btn, "button-press-event", G_CALLBACK(on_button_press), gudata);
	//g_signal_connect_swapped(gudata->gtk_btn, "clicked", G_CALLBACK(gtk_window_destroy), gudata->gtk_wdw);
	
	// Create a combo box and append your string values to it
	gudata->gtk_cmb = gtk_combo_box_text_new();
	//char *langs_usrt[] = {"Seleccionar", "Árabe", "Alemán", "Inglés", "Español", "Francés", "Indú", "Japonés", "Ruso", "Serbio", "Chino CN", "Chino TW"};
	// G_N_ELEMENTS is a macro which determines the number of elements in an array
	//for (size_t i = 0; i < G_N_ELEMENTS(gudata->langs_usrt); i++) {
	for (int i = 0; i < (*gudata->langs_total); i++) {
		gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(gudata->gtk_cmb), !((gudata->langs_usrt)[i]) ? (gudata->langs_avail)[i] : (gudata->langs_usrt)[i]);
	}
	// Choose to set the first row as the active one by default, from the beginning
	//gtk_combo_box_set_active(GTK_COMBO_BOX(gudata->gtk_cmb), 0);
	// gettext needs this validation because GTK needs the locale enable in /etc/locale.gen
	if (is_avail) {
		// Look if locale is a language available to set as the active one in combo box
		if (is_lang_avail((*gudata->langs_total), gudata->langs_avail, gudata->locale, (gudata->cmb_lang_active))) {
			gtk_combo_box_set_active(GTK_COMBO_BOX(gudata->gtk_cmb), (*gudata->cmb_lang_active));
		}
	} else {
		// If there is no language available for locale, set the default one in combo box
		gtk_combo_box_set_active(GTK_COMBO_BOX(gudata->gtk_cmb), (gudata->cmb_lang_default));
	}
	/*
	// ICU always loads the resource even if the locale is not enable in /etc/locale.gen
	// Look if locale is a language available to set as the active one in combo box
	if (is_lang_avail((*gudata->langs_total), gudata->langs_avail, gudata->locale, (gudata->cmb_lang_active))) {
		gtk_combo_box_set_active(GTK_COMBO_BOX(gudata->gtk_cmb), (*gudata->cmb_lang_active));
	}
	*/
	// Connect the signal emitted when a row is selected to the appropriate callback function
	g_signal_connect(gudata->gtk_cmb, "changed", G_CALLBACK(on_change_cmb), gudata);
	
	// Present the window
	//gtk_box_append(GTK_BOX(gudata->gtk_box), gudata->gtk_btn); // gtk-4
	//gtk_box_append(GTK_BOX(gudata->gtk_box), gudata->gtk_cmb); // gtk-4
	gtk_container_add(GTK_CONTAINER(gudata->gtk_box), gudata->gtk_lbl); // gtk-3
	gtk_container_add(GTK_CONTAINER(gudata->gtk_box), gudata->gtk_btn); // gtk-3
	gtk_container_add(GTK_CONTAINER(gudata->gtk_box), gudata->gtk_cmb); // gtk-3
	//gtk_window_present(GTK_WINDOW(gudata->gtk_wdw)); // gtk-4
	// Show the window (is deprecated)
	//gtk_widget_show(gudata->gtk_btn); // gtk-3
	//gtk_widget_show(gudata->gtk_wdw); // gtk-3
	// Show all widgets in the window
	gtk_widget_show_all(gudata->gtk_wdw); // gtk-3
}

/*
// program entry main function
*/

int main(int argc, char *argv[]) {
	// \e escape character for embedding an ANSI sequence
	// \033[38;2 is the ASCII 27 escape character of \e
	// \x1b[38;2 is the HEX escape character of \e
	// [2J indicates to clear the entire screen
	// [1;1H moves the cursor to the top-left position (row 1, column 1)
	//printf("\e[2J\e[1;1H"); // clear the terminal NOT WORKING
	//printf("\033[2J\033[1;1H"); // clear the terminal NOT WORKING
	//printf("\x1b[2J\x1b[1;1H"); // clear the terminal WORKING
	//system("clear"); // clear the terminal WORKING
	
	/*
	// global variables for GTK-3, ICU is not supported on GTK-4
	*/
	
	FILE *out = (NULL); // used by u_fprintf to print localized messages to stdout
	FILE *outerr = (NULL); // used by u_fprintf to print localized messages to stderr
	
	int langs_total = LANGS_AVAILABLE; // total quantity of languages available to select from combo box in gtk
	// array to store codes of languages available to select from combo box in gtk
	char *langs_avail[LANGS_AVAILABLE] = {"", "ar_AE.UTF-8", "de_DE.UTF-8", "en_US.UTF-8", "es_ES.UTF-8", "fr_FR.UTF-8", "hi_IN.UTF-8", "ja_JP.UTF-8", "ru_RU.UTF-8", "sr_RS.UTF-8", "zh_CN.UTF-8", "zh_TW.UTF-8"};
	char *langs_usrt[LANGS_AVAILABLE]; // array to update combo box list in gtk according to selected language
	init_langs_avail(langs_total, langs_usrt);
	int cmb_lang_active = 0; // global variable to store the index to last selected language from combo box in gtk
	int cmb_lang_default = 0; // constant to default index in case lang is not enable for combo box in gtk
	
	char *locale_avail = ""; // stores the last available locale, as locales may not be enable in /etc/locale.gen
	//char *locale = "en_US.UTF-8"; // this will store the current locale or last selected -> WORKING ON ARCH LINUX
	//char *locale = "en_US.utf8"; // this will store the current locale or last selected -> WORKING ON ARCH LINUX
	char *locale = ""; // this will store the current locale or last selected -> BUT BETTER WORK WITH DEFAULT LANG
	char *locale_null = setlocale(LC_ALL, ""); // get the default locale for all categories (LC_ALL)
	char *locale_default = strdup(locale_null); // keep a copy to not get garbage from setlocale
	char *encoding = (NULL); // for now, will stay as NULL always
	char *packageName = (NULL); // if not packageName.mak available, needs an absolute path
	
	/*
	// GTK global pointers to update, or pass it as user_data to a callback
	*/
	
	GtkWidget *gtk_wdw; // global access to gtk widget to update in runtime the gtk window
	GtkWidget *gtk_box; // global access to gtk widget to update in runtime the gtk box layout
	GtkWidget *gtk_lbl; // global access to gtk widget to update in runtime the gtk label
	GtkWidget *gtk_btn; // global access to gtk widget to update in runtime the gtk button
	GtkWidget *gtk_cmb; // global access to gtk widget to update in runtime the gtk combo box text
	
	// set constant to default index in case lang is not enable for combo box in gtk
	is_lang_avail(langs_total, langs_avail, locale_default, &cmb_lang_active);
	cmb_lang_default = cmb_lang_active;
	// Get the value of the LANG environment variable
    char *lang_env_var = getenv("LANG");
    // Check if the environment variable was found
    if (lang_env_var != NULL) {
        printf("LANG environment variable is '%s'\n", lang_env_var);
    } else {
        printf("LANG environment variable is not set\n");
    }
	if (!(lang_env_var) || strcmp(lang_env_var, "") == 0) {
		lang_env_var = locale_default;
	}
	
	// variables to current locale for all categories (LC_ALL)
	char *loc_avail = (NULL);
	bool is_avail = false;
	// getting locale from languages available
	loc_avail = loc_from_lang(langs_total, langs_avail, locale);
	// setting locale through a defined constant
	is_avail = false;
	is_avail = update_locale(loc_avail, locale_default);
	//printf("available '%d' for locale '%s'\n", is_avail, loc_avail);
	if (is_avail) {
		locale_avail = loc_avail;
		update_lang_env(loc_avail, locale_default);
	} else {
		update_lang_env(locale_default, locale_default);
	}
	
	char *currdir = (NULL);
	currdir = current_dir();
	// destiny can be static or dynamically allocated
	// but strcpy does not dynamically allocate memory
	//char currcpy[PATH_MAX];
	//char *currcpy = (char*)malloc((PATH_MAX + 1) * sizeof(char)); // +1 for null terminator (\0)
	// needs to know the length/size and have enough memory to hold the source
	// if destiny buffer is too small, can lead strcpy to undefined behavior
	//strcpy(&currcpy[0], currdir);
	// strdup dynamically allocate memory
	// willing to free the memory by yourself
	//char *currcpy = strdup(currdir);
	char *currcpy = (NULL);
	while(!str_init((&currcpy), (currdir)));
	char *gt_obj_resources = "/resources/i18n-gt";
	while(!str_concat((&currcpy), (gt_obj_resources)));
	packageName = currcpy;
	//printf("%s\n", currdir);
	//printf("%s\n", packageName);
	
	out = (stdout); // used by fprintf to print localized messages to stdout
	outerr = (stderr); // used by fprintf to print localized messages to stderr
	
	L10nKeys *gtkeys = (NULL);
	gtkeys = gtkeys_create();
	
	// domain name, package name, or program name, name.po
	// e.g. for the GNU C Library the domain name is libc
	char *poName = "i18n_gt";
	textdomain(poName);
	bindtextdomain(poName, packageName);
	
	printf("%s\n", _(gtkeys->welcome));
	printf(_t((gtkeys)->nlangs, (gtkeys)->nlangs_plural, langs_total), langs_total);
	printf("\n");
	
	for(int i = 1; i < argc; ++i) {
		if (is_lang_avail(langs_total, langs_avail, argv[i], &cmb_lang_active)) {
			locale = argv[i];
			// getting locale from languages available
			loc_avail = loc_from_lang(langs_total, langs_avail, locale);
			// setting locale through a defined constant
			is_avail = false;
			is_avail = update_locale(loc_avail, locale_default);
			//printf("available '%d' for locale '%s', default is '%s'\n", is_avail, loc_avail, locale_default);
			if (is_avail) {
				locale_avail = loc_avail;
				update_lang_env(loc_avail, locale_default);
			} else {
				locale_avail = locale_default;
				update_lang_env(locale_default, locale_default);
			}
			char *key = gtkeys->greeting; // key string to get the wanted localized value
			//char *key_str = str_by_key(packageName, locale, key, out, outerr);
			char *key_str = str_by_key(locale, key, outerr);
			if (!(key_str)) free(key_str);
		}
	}
	printf(" Current locale is '%s'\n", locale_avail);
	printf("Temporal locale is '%s'\n", locale);
	
	GtkUserData *gudata = (NULL);
	// one way is to initialize data using the gtk creator
	//gudata = g_new0(GtkUserData, 1);
	// and then set each argument one by one
	//gudata->locale = locale;
	// etc etc ...
	// i prefer this way using malloc
	gudata = gudata_create(&gtkeys, &out, &outerr, &langs_total, langs_avail, langs_usrt, &cmb_lang_active, cmb_lang_default, &locale_avail, &locale, locale_default, &encoding, &packageName, &gtk_wdw, &gtk_box, &gtk_lbl, &gtk_btn, &gtk_cmb);
	//update_langs_avail(gtkeys, langs_total, langs_usrt, locale, packageName, out, outerr);
	update_langs_avail(gtkeys, langs_total, langs_usrt, locale, outerr);//, packageName, out, outerr);
	print_langs_avail(langs_total, langs_usrt);
	
	int status_gtk = 0;
	//GtkApplication *app = gtk_application_new(NULL, G_APPLICATION_FLAGS_NONE); // deprecated in gtk-3
	//GtkApplication *app = gtk_application_new(NULL, G_APPLICATION_DEFAULT_FLAGS); // gtk-4
	//g_signal_connect(app, "activate", G_CALLBACK(on_activate_wdw), gudata); // gtk-4
	//status_gtk = g_application_run(G_APPLICATION(app), argc, argv); // gtk-4
    //g_object_unref(app); // gtk-4
	gtk_init(&argc, &argv); // gtk-3
	on_activate_wdw(gudata); // gtk-3
	gtk_main(); // gtk-3
	
	// reset ENVIRONMENT VARIABLES to its defaults
	update_locale(locale_default, locale_default);
	update_lang_env(lang_env_var, locale_default);
	// free allocated memory
	free(currdir);
	currdir = (NULL);
	free(currcpy);
	currcpy = (NULL);
	free(locale_default);
	locale_default = (NULL);
	//free_langs_avail(langs_total, langs_usrt);
	return (status_gtk);
}
