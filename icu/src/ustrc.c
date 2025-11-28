#include "ustrc.h"            // utils for init, duplicate and concat

/*
// global variables
*/

/*
// function definitions
*/

// allocates a string variable dynamically
// willing to free the memory by yourself
char* str_init(char **str_head, char *str) {//, UFILE *outerr) {
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
char* str_replace(char **str_head, char *str) {//, UFILE *outerr) {
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
char* str_concat(char **str_head, char *str_ext) {//, UFILE *outerr) {
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
char* str_from_usrt(char **str, const UChar *ustr, UFILE *outerr) {
	if (!(ustr)) { // ustr == NULL
		return (NULL);
	}
	const UChar *key_ustr = (ustr);
	//UChar *key_ustr = quotedString(ustr);
	int32_t len = u_strlen(key_ustr);
	//u_fprintf(outerr, "UNICODE has length '%ld' for '%S'\n", u_strlen(key_ustr), key_ustr);
	//char key_str[(len+1)];
	char *key_str = (NULL);
	// (len * 4) because each symbol in UNICODE can be longer in UTF-8
	key_str = (char*)malloc(((len + 1) * 4) * sizeof(char)); // +1 for null terminator (\0)
	if (!(key_str)) { // key_str == NULL
		//printf("No se pudo reservar memoria para '%s'\n", (ustr));
		//u_fprintf(outerr, "No se pudo reservar memoria para '%S'\n", (ustr));
		u_fprintf(outerr, "No memory could be allocated for '%S'\n", (ustr));
		free(key_str); // free allocated memory
		key_str = (NULL);
		return (NULL);
	}
	// copy ustring (UChar) to a string encoded in the default codepage (usually UTF-8)
	key_str = u_austrcpy(key_str, key_ustr);
	//free(key_ustr); // only if using key_ustr = quotedString(ustr);
	//printf("  UTF-8 has length '%ld' for '%s'\n", strlen(key_str), key_str);
	(*str) = key_str;
	return (*str);
}

// convert char to UChar* (for ASCII-only example)
UChar* ustr_from_srt(UChar **ustr, char *str, UFILE *outerr) {
	if (!(str)) { // str == NULL
		return (NULL);
	}
	char *key_str = (str);
	size_t len = strlen(key_str);
	//printf("  UTF-8 has length '%ld' for '%s'\n", strlen(key_str), key_str);
	//char key_ustr[(len+1)];
	UChar *key_ustr = (NULL);
	key_ustr = (UChar*)malloc((len + 1) * sizeof(UChar)); // +1 for null terminator (\0)
	if (!(key_ustr)) { // key_ustr == NULL
		//printf("No se pudo reservar memoria para '%s'\n", (key_str));
		//u_fprintf(outerr, "No se pudo reservar memoria para '%s'\n", (key_str));
		u_fprintf(outerr, "No memory could be allocated for '%s'\n", (key_str));
		free(key_ustr); // free allocated memory
		key_ustr = (NULL);
		return (NULL);
	}
	// the inverse is a string default codepage (usually UTF-8) to a ustring (UChar)
	key_ustr = u_uastrcpy(key_ustr, key_str);
	//u_fprintf(outerr, "UNICODE has length '%ld' for '%S'\n", u_strlen(key_ustr), key_ustr);
	(*ustr) = key_ustr;
	return (*ustr);
}

// get a ustring in UNICODE (UChar) from a locale resource given a key
const UChar* ustr_by_key(char *packageName, char *locale, char *key, UFILE *outerr) {
	UResourceBundle *bundle = (NULL); // bundle for locale resource
	UErrorCode status = U_ZERO_ERROR; // number for error code by status
	const char *errname = ""; // string for error name based on status
	const UChar *key_val = (NULL); // ustring in UNICODE codepage (UChar)
	int32_t len = 0; // fill by ures_getStringByKey with the string length
	status = U_ZERO_ERROR;
	bundle = ures_open(packageName, locale, &status);
	errname = u_errorName(status);
	/*
	if (U_SUCCESS(status)) {
		//fprintf(stdout, "'%s' opening locale '%s'\n", errname, locale);
		u_fprintf(out, "'%s' opening locale '%s'\n", errname, locale);
	} else {
		//fprintf(stderr, "'%s' opening locale '%s'\n", errname, locale);
		u_fprintf(outerr, "'%s' opening locale '%s'\n", errname, locale);
	}
	*/
	if(U_FAILURE(status)) {
		//fprintf(stderr, "'%s' opening locale '%s'\n", errname, locale);
		u_fprintf(outerr, "'%s' opening locale '%s'\n", errname, locale);
	}
	status = U_ZERO_ERROR;
	key_val = ures_getStringByKey(bundle, key, &len, &status);
	errname = u_errorName(status);
	/*
	if(U_SUCCESS(status)) {
		//fprintf(stdout, "'%s' getting key '%s'\n", errname, key);
		u_fprintf(out, "'%s' getting key '%s'\n", errname, key);
		u_fprintf(out, "UNICODE has length '%d' for '%S'\n", len, key_val);
	} else {
		//fprintf(stderr, "'%s' getting key '%s'\n", errname, key);
		u_fprintf(outerr, "'%s' getting key '%s'\n", errname, key);
		key_val = (NULL);
	}
	*/
	if(U_FAILURE(status)) {
		//fprintf(stderr, "'%s' getting key '%s'\n", errname, key);
		u_fprintf(outerr, "'%s' getting key '%s'\n", errname, key);
		key_val = (NULL);
	}
	ures_close(bundle);
	return (key_val);
}

// get a string in default codepage (usually UTF-8) from a locale resource given a key
char* str_by_key(char *packageName, char *locale, char *key, UFILE *outerr) {
	const UChar *key_ustr = (NULL);
	key_ustr = ustr_by_key(packageName, locale, key, outerr);
	if (!(key_ustr)) { // key_ustr == NULL
		return (NULL);
	}
	u_fprintf(outerr, "UNICODE in locale '%s' has length '%ld' for '%S'\n", locale, u_strlen(key_ustr), key_ustr);
	char *key_str = (NULL);
	key_str = str_from_usrt((&key_str), (key_ustr), (outerr));
	if (!(key_str)) { // key_str == NULL
		return (NULL);
	}
	fprintf(stderr, "  UTF-8 in locale '%s' has length '%ld' for '%s'\n", locale, strlen(key_str), key_str);
	/*
	key_ustr = ustr_from_srt((&key_ustr), (key_str), (outerr));
	u_fprintf(outerr, "UNICODE in locale '%s' has length '%ld' for '%S'\n", locale, u_strlen(key_ustr), key_ustr);
	free(key_ustr);
	free(key_str);
	*/
	return (key_str);
}
