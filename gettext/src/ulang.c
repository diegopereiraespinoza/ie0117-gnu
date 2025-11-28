#include "ulang.h"            // utils for processing language available

/*
// function definitions
*/

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
