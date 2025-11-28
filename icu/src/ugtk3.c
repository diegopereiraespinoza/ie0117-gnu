#include "ugtk3.h"            // utils for graphics and widgets in GTK-3, ICU is not supported on GTK-4

/*
// global variables
*/

/*
// function definitions
*/

// return: gudata
// allocate memory to create the new gudata
GtkUserData* gudata_create(L10nKeys **gtkeys, UFILE **out, UFILE **outerr, int *nlangs, char *langs_avail[*nlangs], char *langs_usrt[*nlangs], int *cmb_lang_active, int cmb_lang_default, char **locale_avail, char **locale, char *locale_default, char **encoding, char **packageName, GtkWidget **gtk_wdw, GtkWidget **gtk_box, GtkWidget **gtk_lbl, GtkWidget **gtk_btn, GtkWidget **gtk_cmb) {
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
	
	(gudata)->out = (*out); // used by u_fprintf to print localized messages to stdout
	(gudata)->outerr = (*outerr); // used by u_fprintf to print localized messages to stderr
	
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
	char *key_str = str_by_key(gudata->packageName, gudata->locale, gudata->gtkeys->greeting, gudata->outerr);
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
	char *key_str = str_by_key(gudata->packageName, gudata->locale, gudata->gtkeys->farewell, gudata->outerr);
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
	char *key_str = str_by_key(gudata->packageName, gudata->locale, gudata->gtkeys->farewell, gudata->outerr);
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
	char *wdw_title = str_by_key(gudata->packageName, gudata->locale, gudata->gtkeys->wdw_title, gudata->outerr);
	gtk_window_set_title(GTK_WINDOW(gudata->gtk_wdw), wdw_title);
	if (wdw_title != NULL) free(wdw_title);
	
	// Update the label's text
	GtkLabel *lbl = GTK_LABEL(gudata->gtk_lbl);
	//char *lbl_head = str_by_key(gudata->packageName, gudata->locale, gudata->gtkeys->welcome, gudata->out, gudata->outerr);
	char *lbl_head = str_by_key(gudata->packageName, gudata->locale, gudata->gtkeys->welcome, gudata->outerr);
	gtk_label_set_text(lbl, lbl_head);
	if (lbl_head != NULL) free(lbl_head);
	
	// Update the button's label
	// Get the current label of the button
	// Cast user_data to GtkButton* to access the button
	//GtkButton *btn = GTK_BUTTON(user_data);
	//GtkButton *btn = GTK_BUTTON(gudata->gtk_btn);
	// Change the button label based on its current state
	//char *lbl_lang = str_by_key(gudata->packageName, gudata->locale, gudata->gtkeys->lbl_lang, gudata->out, gudata->outerr);
	char *lbl_lang = str_by_key(gudata->packageName, gudata->locale, gudata->gtkeys->lbl_lang, gudata->outerr);
	gtk_button_set_label(GTK_BUTTON(gudata->gtk_btn), lbl_lang);
	if (lbl_lang != NULL) free(lbl_lang);
	
	// Update the combo's elements
	//update_langs_avail(gudata->gtkeys, (*gudata->langs_total), gudata->langs_usrt, gudata->locale, gudata->packageName, gudata->out, gudata->outerr);
	update_langs_avail(gudata->gtkeys, (*gudata->langs_total), gudata->langs_usrt, gudata->packageName, gudata->locale, gudata->outerr);//, gudata->packageName, gudata->out, gudata->outerr);
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
	/*
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
	*/
	// ICU always loads the resource even if the locale is not enable in /etc/locale.gen
	// Look if locale is a language available to set as the active one in combo box
	if (is_lang_avail((*gudata->langs_total), gudata->langs_avail, gudata->locale, (gudata->cmb_lang_active))) {
		gtk_combo_box_set_active(GTK_COMBO_BOX(gudata->gtk_cmb), (*gudata->cmb_lang_active));
	}
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
	char *wdw_title = str_by_key(gudata->packageName, gudata->locale, gudata->gtkeys->wdw_title, gudata->outerr);
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
	char *lbl_head = str_by_key(gudata->packageName, gudata->locale, gudata->gtkeys->welcome, gudata->outerr);
	gudata->gtk_lbl = gtk_label_new(lbl_head);
	if (lbl_head != NULL) free(lbl_head);
	
	// Create a new button
	//gudata->gtk_btn = gtk_button_new_with_label("Language:");
	//char *lbl_lang = str_by_key(gudata->packageName, gudata->locale, gudata->gtkeys->lbl_lang, gudata->out, gudata->outerr);
	char *lbl_lang = str_by_key(gudata->packageName, gudata->locale, gudata->gtkeys->lbl_lang, gudata->outerr);
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
	/*
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
	*/
	// ICU always loads the resource even if the locale is not enable in /etc/locale.gen
	// Look if locale is a language available to set as the active one in combo box
	if (is_lang_avail((*gudata->langs_total), gudata->langs_avail, gudata->locale, (gudata->cmb_lang_active))) {
		gtk_combo_box_set_active(GTK_COMBO_BOX(gudata->gtk_cmb), (*gudata->cmb_lang_active));
	}
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
