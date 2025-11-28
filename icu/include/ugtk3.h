#ifndef UTILS_GTK_H
#define UTILS_GTK_H

#include <stdio.h>            // For scanf, fprintf, printf, perror, getline, ...
#include "ulang.h"            // utils for i18n/i10n resources in icu
#include <gtk/gtk.h>          // For graphics and widgets in GTK-3, ICU is not supported on GTK-4

/*
// global variables defined by struct for GTK
*/

// data type for GTK user data
// typedef struct GtkUserData GtkUserData;
typedef struct GtkUserData {
	L10nKeys *gtkeys; // used by gettext to get localized messages by key
	
	UFILE *out; // used by u_fprintf to print localized messages to stdout
	UFILE *outerr; // used by u_fprintf to print localized messages to stderr
	
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
// function declarations
*/

// return: gudata
// allocate memory to create the new gudata
GtkUserData* gudata_create(L10nKeys **gtkeys, UFILE **out, UFILE **outerr, int *nlangs, char *langs_avail[*nlangs], char *langs_usrt[*nlangs], int *cmb_lang_active, int cmb_lang_default, char **locale_avail, char **locale, char *locale_default, char **encoding, char **packageName, GtkWidget **gtk_wdw, GtkWidget **gtk_box, GtkWidget **gtk_lbl, GtkWidget **gtk_btn, GtkWidget **gtk_cmb);
void on_button_click(GtkButton *btn, gpointer user_data);
// Callback function for a button-press-event
gboolean on_button_press(GtkWidget *widget, GdkEvent *event, gpointer user_data);
gboolean on_delete_wdw(GtkWidget *widget, GdkEvent *event, gpointer user_data);
void on_destroy_wdw(GtkWidget *widget, gpointer user_data);
void redraw_gtk_widgets(GtkUserData *gudata);
// This is the callback function. It is a handler function which reacts to the signal.
// In this case, if the row selected is not the first one of the ComboBox, 
// we write its value in the terminal for the user.
void on_change_cmb(GtkComboBox *widget, gpointer user_data);
void on_activate_wdw(GtkUserData *gudata);//GtkApplication *app, gpointer user_data);

#endif // UTILS_GTK_H
