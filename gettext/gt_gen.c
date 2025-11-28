#define _GNU_SOURCE           // For some systems, random, realpath, PATH_MAX, strsep, asprintf, getline, exp10, ...
#include <locale.h>           // For setlocale, printf("%'d", number) with thousands separator
#include <stdio.h>            // For scanf, fprintf, printf, perror, getline, ...
#include <stdlib.h>           // For rand, random, realpath, system, strtol, malloc, realloc, free, ...
#include <stdbool.h>          // For bool types
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

//#include "uresb.h"            // utils for i18n/i10n resources in icu
//#include "unicode/ustdio.h"
//#include "unicode/ustring.h"
//#include "unicode/ures.h"
//#include <gtk/gtk.h>          // For graphics and widgets in GTK-3, ICU is not supported on GTK-4

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
	int length = strlen(str);
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
	int length = strlen(str);
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
	int base_len = strlen(*str_head);
	int ext_len = strlen(str_ext);
	int length = (base_len + ext_len + 1); // +1 for null terminator (\0)
	char str_aux[length];
	strcpy(&str_aux[0], (*str_head));
	for (int i = 0; i < ext_len; i++) {
		strncat(str_aux, &str_ext[i], 1);
	}
	str_ptr = str_replace(str_head, str_aux);//, outerr);
	if (!(str_ptr)) { // str_ptr == NULL
		// terminate and return control to where is called from
		return (NULL);
	}
	return (*str_head);
}

int main() {//int argc, char *argv[]) {
	/*
	// global variables for GTK-3, ICU is not supported on GTK-4
	*/
	
	char *locale_avail = ""; // stores the last available locale, as locales may not be enabled in /etc/locale.gen
	//char *locale = "en_US.UTF-8"; // this will store the current locale or last selected -> NOT WORKING ON ARCH
	//char *locale = "en_US.utf8"; // this will store the current locale or last selected -> WORKING ON ARCH LINUX
	char *locale = ""; // this will store the current locale or last selected -> BUT BETTER WORK WITH DEFAULT LANG
	//char *encoding = (NULL); // for now, will stay as NULL always
	char *packageName = (NULL); // if not packageName.mak available, needs an absolute path
	
	// get the current locale for all categories (LC_ALL)
	char *locale_prev = setlocale(LC_ALL, NULL);
	if (locale_prev != NULL) {
		printf(" Current LC_ALL locale is '%s'\n", locale_prev);
		locale_avail = locale_prev;
	} else {
		printf("Failed to get LC_ALL locale\n");
		locale_prev = locale;
	}
	// set the locale to support Unicode UTF-8 (or your desired locale)
	//char *locale_curr = setlocale(LC_NUMERIC, "en_US.UTF-8"); // for using a thousands separator
	//char *locale_curr = setlocale(LC_ALL, "en_US.UTF-8"); // for using a thousands separator
	char *locale_curr = setlocale(LC_ALL, locale); // setting locale through a defined constant
	//char *locale_curr = setlocale(LC_CTYPE, ""); // for character handling to the user's default
	//char *locale_curr = setlocale(LC_ALL, ""); // set to the user environment's default locale
	// \e escape character for embedding an ANSI sequence
	// \033[38;2 is the ASCII 27 escape character of \e
	// \x1b[38;2 is the HEX escape character of \e
	// [2J indicates to clear the entire screen
	// [1;1H moves the cursor to the top-left position (row 1, column 1)
	//printf("\e[2J\e[1;1H"); // clear the terminal NOT WORKING
	//printf("\033[2J\033[1;1H"); // clear the terminal NOT WORKING
	//printf("\x1b[2J\x1b[1;1H"); // clear the terminal WORKING
	//system("clear"); // clear the terminal WORKING
	if (locale_curr != NULL) {
		printf("LC_ALL locale changed from '%s' to '%s'\n", locale_prev, locale_curr);
		locale_avail = locale_curr;
		locale_prev = locale_curr;
	} else {
		locale_curr = setlocale(LC_ALL, NULL);
		printf("Failed to change LC_ALL locale from '%s' to '%s'\n", locale_avail, locale);
		locale_prev = locale;
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
	
	// domain name, package name, or program name, name.po
	// e.g. for the GNU C Library the domain name is libc
	char *poName = "i18n_gt";
	textdomain(poName);
	bindtextdomain(poName, packageName);
	
	//Steps for Translation:
	// 1. Use a tool to extract all _(String) into a .po (Portable Object)
	// Tools : xgettext, libglade-xgettext, xml-i18n-extract, intltool-extract
	// xgettext -d i18n_gt -o resources/L10n-gt/i18n_gt.po -k_ gt_gen.c
	// WARNING:
	// xgettext NOT WORKING to generate the .po file if using a struct
	// xgettext NEEDS to have the String directly into the printf
	// 2. Copy and Translate the .po files to languages available
	// 3. Compile the .po files into .mo (Machine Object) files
	// msgfmt resources/L10n-gt/i18n_gt.po -o resources/i18n-gt/i18n_gt.mo
	/*
	L10nKeys *gtkeys = (NULL);
	gtkeys = gtkeys_create();
	
	printf("%s\n", _(gtkeys->wdw_title));
	printf("%s\n", _(gtkeys->greeting));
	printf("%s\n", _(gtkeys->farewell));
	printf("%s\n", _(gtkeys->welcome));
	printf("%s\n", _(gtkeys->lbl_lang));
	printf("%s\n", _(gtkeys->cmd_lang_root));
	printf("%s\n", _(gtkeys->cmd_lang_ar));
	printf("%s\n", _(gtkeys->cmd_lang_de));
	printf("%s\n", _(gtkeys->cmd_lang_en));
	printf("%s\n", _(gtkeys->cmd_lang_es));
	printf("%s\n", _(gtkeys->cmd_lang_fr));
	printf("%s\n", _(gtkeys->cmd_lang_hi));
	printf("%s\n", _(gtkeys->cmd_lang_ja));
	printf("%s\n", _(gtkeys->cmd_lang_ru));
	printf("%s\n", _(gtkeys->cmd_lang_sr));
	printf("%s\n", _(gtkeys->cmd_lang_zh));
	printf("%s\n", _(gtkeys->cmd_lang_zh_TW));
	printf("%s\n", _(gtkeys->nlangs));
	printf("%s\n", _(gtkeys->nlangs_plural));
	printf(_t("%d language is available.", "%d languages are available.", LANGS_AVAILABLE), LANGS_AVAILABLE);
	printf("\n");
	*/
	printf("%s\n", _("Internationalization"));
	printf("%s\n", _("Hello!"));
	printf("%s\n", _("Goodbye!"));
	printf("%s\n", _("Welcome!"));
	printf("%s\n", _("Language"));
	printf("%s\n", _("Select"));
	printf("%s\n", _("Arabic"));
	printf("%s\n", _("German"));
	printf("%s\n", _("English"));
	printf("%s\n", _("Spanish"));
	printf("%s\n", _("French"));
	printf("%s\n", _("Hindi"));
	printf("%s\n", _("Japanese"));
	printf("%s\n", _("Russian"));
	printf("%s\n", _("Serbian"));
	printf("%s\n", _("Chinese CN"));
	printf("%s\n", _("Chinese TW"));
	printf("%s\n", _("%d language is available."));
	printf("%s\n", _("%d languages are available."));
	
	return (0);
}
