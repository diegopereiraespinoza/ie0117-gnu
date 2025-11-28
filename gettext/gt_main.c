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
#include "upath.h"            // utils for resolving urls from symlinks
//#include "ustrc.h"            // utils for init, duplicate and concat
//#include "ulang.h"            // utils for processing language available
#include "ugtk3.h"            // utils for graphics and widgets in GTK-3, ICU is not supported on GTK-4

/*
// global constants defined by macros
*/

#define _(String) gettext(String)
#define _t(String1, String2, n) ngettext(String1, String2, n)

// total quantity of languages available to select from combo box in gtk
#ifndef LANGS_AVAILABLE
	#define LANGS_AVAILABLE 12
#endif // LANGS_AVAILABLE

/*
// function definitions
*/

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
