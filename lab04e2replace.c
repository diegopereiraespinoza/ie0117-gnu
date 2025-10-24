#define _GNU_SOURCE // Required by some systems for strsep, asprintf
#include <locale.h> // For setlocale to use a thousands separator in numbers by printf
//#include <libio.h>  // For fwide
#include <stdio.h>  // For fprintf, printf, scanf, ...
#include <string.h> // For memmove, strlen, though getline returns length
#include <stdlib.h> // For rand, srand, system, strtok, strtol, malloc, realloc, free, if dynamic allocation is used
//#include <time.h>   // For seeding rand/random with time
//#include <limits.h> // For limits of: char / short / int (INT_MIN/INT_MAX) / long / long long (LLONG_MIN/LLONG_MAX)
//#include <float.h>  // For limits of: float (FLT_MIN/FLT_MAX) / double / long double (LDBL_MIN/LDBL_MAX)
#include <errno.h>  // For errno == ERANGE, clear errno=0 before call strlen, strcpy, strstr, strtok, strtol, ...
#include <ctype.h>  // For toupper, tolower, isalpha, isdigit, isspace (trim spaces from words)
//#include <iconv.h>  // For decimal ASCII to UTF-8
#include <stdbool.h>// For bool types
//#include <uchar.h>  // For uchar_t on estandar UTF-8
//#include <wchar.h>  // For wchar_t and wide character functions
/*
// global constants defined by macros
*/
#ifndef FORWARD
	#define FORWARD 0 // forward search for utf8 symbols
#endif
#ifndef REVERSE
	#define REVERSE 1 // reverse search for utf8 symbols
#endif
#ifndef ALLOWED
	// ANSI/ASCII 192 <= symbol <= 255
	// they are 50 symbols, but each one occupies 2 bytes, so strlen is 100
	#define ALLOWED "ÀÁÂÃÄÅÈÉÊËÌÍÎÏÑÒÓÔÕÖÙÚÛÜàáâãäåèéêëìíîïñòóôõöùúûüýÿ";
	//#define ALLOWED "";
#endif
// it is a must to call this function after each scanf(" %c", &choice);
// in order to clear the input buffer of any leftover newline (\n)
// if remaining characters in the buffer are not consume, including newline (\n)
// fgets will read the \n as a hit of enter, reading always 1 byte with no input
// to prevent this issue, use a getchar() call in a loop
// WARNING: use only after scanf(" %c", &choice);
void clear_input() {
	int c;
	//while ((c = getchar()) != '\n' && c != EOF);
	while ((c = getchar()) != '\n' && c != '\0');
}
char* malloc_mem(char *str_input) {
	int length = strlen(str_input);
	//ssize_t length = strlen(str_input);
	//printf("'%s' %zd\n", str_input, length);
	char *str_buffer = NULL;
	str_buffer = (char*)malloc(length * sizeof(char) + 1); // +1 for null terminator (\0)
	if (str_buffer == NULL) {
		//perror("No se pudo reservar memoria.");
		free(str_buffer);
		return (NULL); // terminate and return control to where is called from
	}
	strcpy(&str_buffer[0], str_input);
	return (str_buffer);
}
char* ralloc_mem(char **str_old, char *str_new) {
	//ssize_t length_old = (strlen(*str_old) + 1); // +1 for null terminator (\0)
	int length_new = strlen(str_new);
	char *str_buffer = NULL;
	str_buffer = (char*)realloc((*str_old), length_new * sizeof(char) + 1); // +1 for null terminator (\0)
	if (str_buffer == NULL) {
		//perror("No se pudo reservar memoria.");
		free(*str_old); // free original memory
		return (NULL);
	}
	(*str_old) = str_buffer;
	strcpy(&(*str_old)[0], str_new);
	return (str_buffer);
}
void free_all(char **word_old, char **word_new, char **file_name_input, char **file_name_temp) {
	free(*word_old);
	(*word_old) = NULL;
	free(*word_new);
	(*word_new) = NULL;
	free(*file_name_input);
	(*file_name_input) = NULL;
	free(*file_name_temp);
	(*file_name_temp) = NULL;
}
// function to remove a portion from array between indexes start and end
void remove_from_array(int *length, int array[(*length)], size_t start_index, size_t end_index) {
	if ((*length) <= 0) return;
	// validate indices
	if (start_index < (*length) && end_index < (*length) && start_index <= end_index) {
		// calculate the number of characters to remove
		size_t num_to_remove = end_index - start_index + 1;
		// fixed-size arrays: 
		// this method modifies the content of a fixed-size char array.
		// it does not reallocate memory or change the array's overall size.
		// if you need dynamic resizing, you would typically use malloc/realloc
		// memmove shift to the left the remaining characters.
		// memmove is used instead of memcpy because the source and destination memory regions might overlap.
		// &str[start_index]: destination address where the shifted characters will be placed.
		// &str[end_index + 1]: source address, which is the first character after the range to be removed.
		// length - (end_index + 1) + 1: number of bytes to left move
		// moves from end_index + 1 until end plus 1 to include the null terminator (\0)
		memmove((&array[start_index]), (&array[end_index + 1]), ((*length) - (end_index + 1) + 1) * sizeof(char));
		// ensure the output buffer is null-terminated (\0)
		array[(*length) - num_to_remove] = '\0';
		(*length)--;
	}
	//printf(" new length %d\n", (*length));
	//for (int i = 0; i < (*length); i++) printf(" %d", (array[i]));
	//printf("\n");
}
// function to check the space a symbol occupies in bytes
int get_utf8_char_len(unsigned char byte) {
	if (byte == '\x00' || byte == '\x0D' || byte == '\x0A' || byte == '\0') {
		return 0; // like '' = "\x00", '\r' = "\x0D", '\n' = "\x0A"
	} else if ((byte & 0x80) == 0x00) { // 0xxxxxxx
		return 1; // like 'e' = "\x65"
	} else if ((byte & 0xE0) == 0xC0) { // 110xxxxx
		return 2; // like 'é' = "\xC3\xA9"
	} else if ((byte & 0xF0) == 0xE0) { // 1110xxxx
		return 3; // like '€' = "\xE2\x82\xAC"
	} else if ((byte & 0xF8) == 0xF0) { // 11110xxx
		return 4;
	}
	return 0; // invalid UTF-8 start byte, like '\xFF'
}
// count by single symbol, no matters how many bytes occupies each one
long double get_line_symbols(char *str_buffer, int str_length) {
	//ssize_t str_length = strlen(str_buffer);
	long double line_symbols = 0;
	int i = 0, bytes = 0;
	for (i = 0; i < str_length; i++) {
		bytes = get_utf8_char_len(str_buffer[i]);
		if (0 < bytes) line_symbols++;
	}
	return (line_symbols);
}
// check if has carriage return type like endings in Windows CR LF \r\n
// length-1 to subtract only the newline (\n) from Unix LF endings
// length-2 to subtract carriage return (\r) + newline (\n) from Windows CR LF endings
long double get_line_bytes(char *str_buffer, int str_length) {
	//ssize_t str_length = strlen(str_buffer);
	long double line_bytes = 0;
	if (0 < str_length) {
		line_bytes = (str_length); // default
		if (str_buffer[str_length - 1] == '\n') {
			line_bytes = (str_length - 1); // UTF-8 Unix (LF)
			if (1 < str_length && str_buffer[str_length - 2] == '\r') {
				line_bytes = (str_length - 2); // UTF-8 Windows (CR LF)
			}
		}
	}
	return (line_bytes);
}
// function to check if a symbol is an accepted letter/vowel with accent
bool alphas(char char_1, char char_2, char char_3, char char_4, int order) {
	bool is_accent = false;
	// ANSI/ASCII 192 <= symbol <= 255
	// they are 50 symbols, but each one occupies 2 bytes, so strlen is 100
	char *haystack = ALLOWED;
	// append each symbol to the needle that represents a letter/vowel with accent
    // &char_N converts the char to a char pointer array of size 1 (null-terminated)
    int i, j, length_sym = 4;
	char symbol[length_sym] = {};
	char frw[] = {char_1, char_2, char_3, char_4, '\0'};
	//char frw[length_sym] = {};
	//strncat(frw, &char_1, 1);
	//strncat(frw, &char_2, 1);
	//strncat(frw, &char_3, 1);
	//strncat(frw, &char_4, 1);
	char rev[] = {char_4, char_3, char_2, char_1, '\0'};
	//char rev[length_sym] = {};
	//strncat(rev, &char_4, 1);
	//strncat(rev, &char_3, 1);
	//strncat(rev, &char_2, 1);
	//strncat(rev, &char_1, 1);
	int bytes = 0;
	for (i = 0; bytes == 0 && i < length_sym; i++) {
		bytes = get_utf8_char_len(frw[i]);
	}
	int length_frw = strlen(frw);
	//printf("length_frw %zu ", length_frw);
	if (order == FORWARD) {
		for (j = 0; j < bytes; j++) {
			strncat(symbol, &frw[j], 1);
		}
		// validate indices
		if (bytes < length_frw) {
			size_t start_index = (bytes);
			size_t end_index = (length_frw - 1);
			// validate indices
			if (start_index < length_frw && end_index < length_frw && start_index <= end_index) {
				// calculate the number of characters to remove
				size_t num_to_remove = end_index - start_index + 1;
				// fixed-size arrays: 
				// this method modifies the content of a fixed-size char array.
				// it does not reallocate memory or change the array's overall size.
				// if you need dynamic resizing, you would typically use malloc/realloc
				// memmove shift to the left the remaining characters.
				// memmove is used instead of memcpy because the source and destination memory regions might overlap.
				// &str[start_index]: destination address where the shifted characters will be placed.
				// &str[end_index + 1]: source address, which is the first character after the range to be removed.
				// length - (end_index + 1) + 1: number of bytes to left move
				// moves from end_index + 1 until end plus 1 to include the null terminator (\0)
				memmove(&frw[start_index], &frw[end_index + 1], (length_frw - (end_index + 1) + 1) * sizeof(char));
				// ensure the output buffer is null-terminated (\0)
				frw[length_frw - num_to_remove] = '\0';
			}
		}
	}
	int length_rev = strlen(rev);
	//printf("length_rev %zu ", length_rev);
	if (order == REVERSE) {
		for (j = length_sym - bytes; j < length_sym; j++) {
			strncat(symbol, &rev[j], 1);
		}
		// validate indices
		if (bytes < length_rev) {
			size_t start_index = 0;
			size_t end_index = (length_rev - bytes - 1);
			// validate indices
			if (start_index < length_rev && end_index < length_rev && start_index <= end_index) {
				// calculate the number of characters to remove
				size_t num_to_remove = end_index - start_index + 1;
				// fixed-size arrays: 
				// this method modifies the content of a fixed-size char array.
				// it does not reallocate memory or change the array's overall size.
				// if you need dynamic resizing, you would typically use malloc/realloc
				// memmove shift to the left the remaining characters.
				// memmove is used instead of memcpy because the source and destination memory regions might overlap.
				// &str[start_index]: destination address where the shifted characters will be placed.
				// &str[end_index + 1]: source address, which is the first character after the range to be removed.
				// length - (end_index + 1) + 1: number of bytes to left move
				// moves from end_index + 1 until end plus 1 to include the null terminator (\0)
				memmove(&rev[start_index], &rev[end_index + 1], (length_rev - (end_index + 1) + 1) * sizeof(char));
				// ensure the output buffer is null-terminated (\0)
				rev[length_rev - num_to_remove] = '\0';
			}
		}
	}
	/*char empty = '\0';
	for (j = bytes; j < length_sym; j++) {
		strncat(symbol, &empty, 1);
	}*/
	//printf("length_frw %zu length_rev %zu\n", strlen(frw), strlen(rev));
	//printf("'%s' -> ['0x%X','0x%X','0x%X','0x%X']\n", frw, char_1, char_2, char_3, char_4);
	//for (j = 0; j < length_sym; j++) {printf("%d ", get_utf8_char_len(frw[j]));}
	//printf("i %d bytes %d order %d forward %d reverse %d '%s' '%s'\n", i, bytes, order, FORWARD, REVERSE, frw, rev);
	int size_symbol = strlen(symbol);
	if (size_symbol == 0 || bytes < 1) {
		return (is_accent); // terminate and return control to where is called from
	}
	//long double symbols_count = get_line_symbols(haystack, strlen(haystack));
	//printf("buscando símbolo '%s' de %d %s en %'.0Lf acentos permitidos\n", symbol, size_symbol, ((bytes == 1) ? "byte" : "bytes"), symbols_count);
	//char *needle = symbol;
	char *needle = NULL;
	needle = (char*)malloc(bytes + 1); // +1 for null terminator (\0)
	if (needle == NULL) {
		//perror("No se pudo reservar memoria.");
		//exit (EXIT_FAILURE); // terminate immediately, regardless where is called from
		free(needle);
		return (is_accent); // terminate and return control to where is called from
	}
	strcpy(&needle[0], symbol);
	i = 0;
	// iterate through haystack
	while (*haystack) {
		if (strstr(haystack, needle) == haystack) {
			//printf("%2d ANSI %3d ASCII '%c' %p match '%s' size %lu\n", i, haystack[i], haystack[i], &haystack[i], needle, strlen(needle));
			haystack += bytes;
			i += bytes;
			is_accent = true;
			break;
		} else {
			//printf("%2d ANSI %3d ASCII '%c' %p\n", i, haystack[i], haystack[i], &haystack[i]);
			haystack++;
			i++;
		}
	}
	free(needle);
	//printf("símbolo '%s' de %d %s %s una letra/vocal con acento\n", symbol, bytes, ((bytes == 1) ? "byte" : "bytes"), ((is_accent) ? "es" : "no es"));
	return (is_accent);
}
bool last_next(const char *p, int b, int n) {
	char c = '\x00';
	/*char frw[4] = {};
	strncat(frw, &p[b+1], 1);
	strncat(frw, (b+2)<n?&p[b+2]:&c, 1);
	strncat(frw, (b+3)<n?&p[b+3]:&c, 1);
	strncat(frw, (b+4)<n?&p[b+4]:&c, 1);
	printf("last_next '%s'\n", frw);*/
	return (isalpha(p[b+1]) || isdigit(p[b+1]) || p[b+1] == '_' || alphas(p[b+1],(b+2)<n?p[b+2]:c,(b+3)<n?p[b+3]:c,(b+4)<n?p[b+4]:c,FORWARD));
}
bool last(const char *p, int b) {
	char c = '\x00';
	/*char rev[4] = {};
	strncat(rev, (b-3)>=0?&p[b-3]:&c, 1);
	strncat(rev, (b-2)>=0?&p[b-2]:&c, 1);
	strncat(rev, (b-1)>=0?&p[b-1]:&c, 1);
	strncat(rev, &p[b], 1);
	printf("last '%s'\n", rev);*/
	return (isalpha(p[b]) || isdigit(p[b]) || p[b] == '_' || alphas(p[b],(b-1)>=0?p[b-1]:c,(b-2)>=0?p[b-2]:c,(b-3)>=0?p[b-3]:c,REVERSE));
}
bool first_prev(const char *p, int a) {
	char c = '\x00';
	/*char rev[4] = {};
	strncat(rev, (a-4)>=0?&p[a-4]:&c, 1);
	strncat(rev, (a-3)>=0?&p[a-3]:&c, 1);
	strncat(rev, (a-2)>=0?&p[a-2]:&c, 1);
	strncat(rev, (a-1)>=0?&p[a-1]:&c, 1);
	printf("first_prev '%s'\n", rev);*/
	return (isalpha(p[a-1]) || isdigit(p[a-1]) || p[a-1] == '_' || alphas(p[a-1],(a-2)>=0?p[a-2]:c,(a-3)>=0?p[a-3]:c,(a-4)>=0?p[a-4]:c,REVERSE));
}
bool first(const char *p, int a, int n) {
	char c = '\x00';
	/*char frw[4] = {};
	strncat(frw, &p[a], 1);
	strncat(frw, (a+1)<n?&p[a+1]:&c, 1);
	strncat(frw, (a+2)<n?&p[a+2]:&c, 1);
	strncat(frw, (a+3)<n?&p[a+3]:&c, 1);
	printf("first '%s'\n", frw);*/
	return (isalpha(p[a]) || isdigit(p[a]) || p[a] == '_' || alphas(p[a],(a+1)<n?p[a+1]:c,(a+2)<n?p[a+2]:c,(a+3)<n?p[a+3]:c,FORWARD));
}
// function to replace only complete words in file content
long double replace_word(const char *word_old, const char *word_new, const char *p, char **buffer_output, long double *matches) {
	//char *buffer_head = strdup(p); // if copy, must be free at the end
	//char *buffer_copy = buffer_head; // keep a pointer to the start for freeing later
	const char *buffer_input = p;
	char *buffer_new;
	//char c = '\x00';
	bool is_word = false;
	int i, j, a, b;
	long double words = 0;
	int size_old = strlen(word_old);
	int size_new = strlen(word_new);
	int n = strlen(buffer_input);
	//printf("'%s' - '%s' = %d - %d = %d diff, line size %d\n", word_new, word_old, size_new, size_old, (size_new - size_old), n);
	// count matches (words or not) and words (only complete words):
	// iterate through buffer_input to count how many times word_old appears
	for (i = 0; p[i] != '\0'; i++) {
		//printf("%2d ANSI %3d ASCII '%c' %p\n", i, p[i], p[i], &p[i]);
		// strstr function is used to find matches of word_old
		if (strstr(&p[i], word_old) == &p[i]) {
			// at this point i is pointing to the first character of the found match
			// adding (size_old - 1) jumps to the last character of the found match
			// to determine the output buffer length
			// must count only if the match is a complete word
			// this is crucial for allocating the correct amount of memory
			a = i; // first starting index
			b = (i + size_old - 1); // last ending index
			is_word = true;
			//is_word = ((isalpha(p[b+1]) || isdigit(p[b+1]) || p[b+1] == '_') && (isalpha(p[b]) || isdigit(p[b]) || p[b] == '_')) ? false : !(0 < i && (isalpha(p[a-1]) || isdigit(p[a-1]) || p[a-1] == '_') && (isalpha(p[a]) || isdigit(p[a]) || p[a] == '_')); // without accent check
			is_word = (last_next(p, b, n) && last(p, b)) ? false : !(0 < i && first_prev(p, a) && first(p, a, n)); // with accent check
			if (is_word) words++;
			(*matches)++;
			i += (size_old - 1);
			//printf("first %2d ANSI %3d ASCII '%c' %p match %'.0Lf\n", a, p[a], p[a], &p[a], (*matches));
			//printf("last %2d ANSI %3d ASCII '%c' %p match %'.0Lf\n", b, p[b], p[b], &p[b], (*matches));
			//printf("%2d ANSI %3d ASCII '%c' %p match %'.0Lf\n", i, p[i], p[i], &p[i], (*matches));
			//printf("is_word=%s ", is_word ? "true" : "false");
			//printf("p[%d] '%c' p[%d] '%c'\n", (a-1), p[a-1], (b+1), p[b+1]);
		}
	}
	// allocate memory (dynamically)
	// size for the buffer_new is calculated based on the buffer_input's length
	// matches of word_old, and the difference in length between word_new and word_old
	// buffer_new is dynamic allocated using malloc, so can persist after the function exits
	// but needs to correctly update the pointer to that memory living on the heap
	buffer_new = (char*)malloc(i + words * (size_new - size_old) + 1); // +1 for null terminator (\0)
	if (buffer_new == NULL) {
		//perror("No se pudo reservar memoria.");
		//exit (EXIT_FAILURE); // terminate immediately, regardless where is called from
		// update the pointer to the output buffer
		*buffer_output = NULL;
		return -1;
	}
	// perform replacement
	i = 0, j = 0;
	// iterate through buffer_input to replace
	while (*buffer_input) {
		if (strstr(buffer_input, word_old) == buffer_input) {
			//a = 0; // first starting index
			//b = (size_old - 1); // last ending index
			a = j; // first starting index
			b = (j + size_old - 1); // last ending index
			is_word = true;
			//is_word = ((isalpha(p[b+1]) || isdigit(p[b+1]) || p[b+1] == '_') && (isalpha(p[b]) || isdigit(p[b]) || p[b] == '_')) ? false : !(0 < i && (isalpha(p[a-1]) || isdigit(p[a-1]) || p[a-1] == '_') && (isalpha(p[a]) || isdigit(p[a]) || p[a] == '_')); // without accent check
			is_word = (last_next(p, b, n) && last(p, b)) ? false : !(0 < j && first_prev(p, a) && first(p, a, n)); // with accent check
			//printf("first %2d ANSI %3d ASCII '%c' %p match %'.0Lf\n", a, p[a], p[a], &p[a], (*matches));
			//printf("last %2d ANSI %3d ASCII '%c' %p match %'.0Lf\n", b, p[b], p[b], &p[b], (*matches));
			//printf("%2d ANSI %3d ASCII '%c' %p\n", i, buffer_new[i], buffer_new[i], &buffer_new[i]);
			//printf("is_word=%s ", is_word ? "true" : "false");
			//printf("p[%d] '%c' p[%d] '%c'\n", (a-1), p[a-1], (b+1), p[b+1]);
			// when an instance of word_old is found, copy word_new into buffer_new
			// strcpy or strcat functions can be helpful here
			// must replace only if the match is a complete word
			if (is_word) {
				strcpy(&buffer_new[i], word_new);
				i += (size_new);
			} else {
				strcpy(&buffer_new[i], word_old);
				i += (size_old);
			}
			j += (size_old);
			buffer_input += size_old;
			//printf("%2d buffer '%s'\n", i, buffer_new);
		} else {
			// otherwise, manual character copying from the buffer_input
			buffer_new[i++] = *buffer_input++;
			j++;
			//printf("%2d ANSI %3d ASCII '%c' %p\n", i-1, buffer_new[i-1], buffer_new[i-1], &buffer_new[i-1]);
		}
	}
	// null terminator (\0)
	// ensure the output buffer is null-terminated
	buffer_new[i] = '\0';
	// update the pointer to the output buffer
	*buffer_output = buffer_new;
	// return and free
	// remember to free to prevent memory leaks
	// free the dynamically allocated memory by the replace_word function
	//free(*buffer_output);
	//free(buffer_copy);
	return (words);
}
long double get_by_line(const char *word_old, const char *word_new, long double *matches, const char *file_name_input, const char *file_name_temp) {
	FILE *file_pointer_txt_input = NULL, *file_pointer_txt_temp = NULL;
	char *buffer_input = NULL, *buffer_output = NULL;
	size_t buffer_size = 0; // use %zu for unsigned size_t, ssize_t is a signed %zd version of size_t
	ssize_t bytes_read = 0; // use %zd ssize_t for the return value of getline
	ssize_t bytes_new = 0; // use %zd ssize_t for the return value of replace_word
	long double line_bytes = 0, bytes_r = 0, bytes_w = 0;
	long double line_number = 0, line_words = 0, words = 0;
	//printf("'%s' '%s' '%s' '%s'\n", word_old, word_new, file_name_input, file_name_temp);
	file_pointer_txt_input = fopen(file_name_input, "r"); // open the file for reading
	if (file_pointer_txt_input == NULL) {
		//perror("No se pudo abrir el archivo fuente.");
		return -1;
	}
	//printf("'%s' '%s' '%s' '%s'\n", word_old, word_new, file_name_input, file_name_temp);
	file_pointer_txt_temp = fopen(file_name_temp, "w"); // open the file for writing
	if (file_pointer_txt_temp == NULL) {
		//perror("No se pudo abrir el archivo temporal.");
		fclose(file_pointer_txt_input);
		return -1;
	}
	printf("--------------- Procesando Archivo ---------------\n");
	//printf("%s : \"%s\"\n", "Nombre del archivo", file_name_input);
	//printf("%s : \"%s\"\n", "Coincidencia vieja", word_old);
	//printf("%s : \"%s\"\n", "Coincidencia nueva", word_new);
	printf("%s : \"%s\"\n", "  Nombre del leído", file_name_input);
	printf("%s : \"%s\"\n", "  Nombre del nuevo", file_name_temp);
	printf("%s : \"%s\"\n", "Texto que se busca", word_old);
	printf("%s : \"%s\"\n", "El reemplazo nuevo", word_new);
	// read lines one by one until end-of-file (EOF)
	// getline returns the number of characters
	// including the newline (\n) and the null terminator (\0)
	// if you need the length without the newline, you can subtract 1
	// and check if (buffer_input[bytes_read - 1] == '\n')
	// on error or end-of-file (EOF), it returns -1
	// getline handles memory allocation and reallocation as needed
	// making it suitable for reading lines of unknown or varying lengths
	// You are responsible to free the buffer_input when you are done
	// if buffer_input is initially NULL, getline will allocate memory
	// &buffer_input is the address of pointer char* to store allocated content
	// &buffer_size is the address of size_t to update with size of buffer_input
	// file_pointer_txt_input is a file pointer from which to read the content
	while ((bytes_read = getline(&buffer_input, &buffer_size, file_pointer_txt_input)) != -1) {
		// the returning value by getline can be store in a variable to hold the number of characters
		// including the newline (\n) character if present, and the null terminator (\0)
		// if you want the length without the newline, you can check for it and adjust
		// if (buffer_input[bytes_read - 1] == '\n')
		line_number++;
		printf("----------------- Línea  %'7.0Lf -----------------\n", line_number);
		if (0 < bytes_read) {
			line_bytes = get_line_bytes(buffer_input, bytes_read);
			// if you need the length *without* the trailing newline
			if (buffer_input[bytes_read - 1] == '\n') {
				printf("%s", buffer_input); // already contains the newline
				if (1 < bytes_read && buffer_input[bytes_read - 2] == '\r') {
					printf("Fin de Línea tipo Windows (CR LF)\n");
				} else {
					printf("Fin de Línea tipo Unix (LF)\n");
				}
			} else {
				printf("%s\n", buffer_input); // adding the newline (\n)
				printf("Fin de Línea sin CR (Carro de Retorno)\n");
			}
			printf("Leídos       %'7.0Lf Bytes en Línea %'7.0Lf\n", line_bytes, line_number);
			bytes_r += line_bytes;
		}
		// This simple replace_word function might replace substrings
		// For complete word replacement, you'd need more sophisticated logic
		// (e.g., checking for word boundaries like spaces or punctuation)
		line_words = (long double)replace_word(word_old, word_new, buffer_input, &buffer_output, matches);
		if (buffer_output != NULL && 0 <= line_words) {
			words += line_words;
			bytes_new = strlen(buffer_output);
			if (0 < bytes_new) {
				line_bytes = get_line_bytes(buffer_output, bytes_new);
				// if you need the length *without* the trailing newline
				if (buffer_output[bytes_new - 1] == '\n') {
					printf("%s", buffer_output); // already contains the newline
					if (1 < bytes_new && buffer_output[bytes_new - 2] == '\r') {
						printf("Fin de Línea tipo Windows (CR LF)\n");
					} else {
						printf("Fin de Línea tipo Unix (LF)\n");
					}
					// write the buffer_output to the temporary file
					fprintf(file_pointer_txt_temp, "%s", buffer_output);
				} else {
					printf("%s\n", buffer_output); // adding the newline (\n)
					printf("Fin de Línea sin CR (Carro de Retorno)\n");
					// write the buffer_output to the temporary file
					fprintf(file_pointer_txt_temp, "%s\n", buffer_output);
				}
				printf("Reemplazados %'7.0Lf Bytes en Línea %'7.0Lf\n", line_bytes, line_number);
				bytes_w += line_bytes;
			}
			// free the dynamically allocated memory by the replace_word function
			free(buffer_output);
			buffer_output = NULL;
			bytes_new = 0;
		}
		// free the dynamically allocated memory by the getline function
		free(buffer_input);
		buffer_input = NULL;
		bytes_read = 0;
	}
	printf("----------------- Línea  %'7.0Lf -----------------\n", ++line_number);
	printf("Coincidencias  Encontradas  %'7.0Lf\n", (*matches));
	printf("Coincidencias Reemplazadas  %'7.0Lf\n", words);
	fclose(file_pointer_txt_input); // close the original source file
	fclose(file_pointer_txt_temp); // close the temporary replaced file
	// free the dynamically allocated memory by the getline function
	if (buffer_input != NULL) {
		free(buffer_input);
		buffer_input = NULL;
		bytes_read = 0;
	}
	return (words);
}
long double replace_file(const char *file_name_input, const char *file_name_temp) {
	FILE *file_pointer_txt_input = NULL, *file_pointer_txt_temp = NULL;
	char *buffer_input = NULL;
	size_t buffer_size = 0; // use %zu for unsigned size_t, ssize_t is a signed %zd version of size_t
	ssize_t bytes_read = 0; // use %zd ssize_t for the return value of getline
	long double line_bytes = 0, bytes = 0;
	long double line_number = 0;
	file_pointer_txt_temp = fopen(file_name_temp, "r"); // open the file for reading
	if (file_pointer_txt_temp == NULL) {
		//perror("No se pudo abrir el archivo temporal.");
		fclose(file_pointer_txt_input);
		return -1;
	}
	file_pointer_txt_input = fopen(file_name_input, "w"); // open the file for writing
	if (file_pointer_txt_input == NULL) {
		//perror("No se pudo abrir el archivo fuente.");
		return -1;
	}
	printf("-------------- Actualizando Archivo --------------\n");
	while ((bytes_read = getline(&buffer_input, &buffer_size, file_pointer_txt_temp)) != -1) {
		line_number++;
		if (0 < bytes_read) {
			line_bytes = get_line_bytes(buffer_input, bytes_read);
			if (buffer_input[bytes_read - 1] == '\n') {
				printf("%s", buffer_input); // already contains the newline
				if (1 < bytes_read && buffer_input[bytes_read - 2] == '\r') {
					printf("Fin de Línea tipo Windows (CR LF)\n");
				} else {
					printf("Fin de Línea tipo Unix (LF)\n");
				}
				fprintf(file_pointer_txt_input, "%s", buffer_input);
			} else {
				printf("%s\n", buffer_input); // adding the newline (\n)
				printf("Fin de Línea sin CR (Carro de Retorno)\n");
				fprintf(file_pointer_txt_input, "%s\n", buffer_input);
			}
			printf("Actualizados %'7.0Lf Bytes en Línea %'7.0Lf\n", line_bytes, line_number);
			bytes += line_bytes;
		}
		// free the dynamically allocated memory by the getline function
		free(buffer_input);
		buffer_input = NULL;
		bytes_read = 0;
	}
	printf("---------------- Final de Archivo ----------------\n");
	fclose(file_pointer_txt_input); // close the original source file
	fclose(file_pointer_txt_temp); // close the temporary replaced file
	// replace the original source file with the temporary replaced file
	/*if (remove(file_name_input) != 0) {
		//perror("No se pudo eliminar el archivo fuente.");
		return -1;
	}
	if (rename(file_name_temp, file_name_input) != 0) {
		//perror("No se pudo renombrar el archivo temporal.");
		return -1;
	}*/
	// delete the temporary replaced file after update the source file
	if (remove(file_name_temp) != 0) {
		//perror("No se pudo eliminar el archivo temporal.");
		return -1;
	}
	return (bytes);
}
long double create_file(const char *file_name_input) {
	FILE *file_pointer_txt_input = NULL;
	ssize_t bytes_read = 0; // use %zd ssize_t for the return value of getline
	long double line_bytes = 0, bytes = 0;
	long double line_number = 0;
	file_pointer_txt_input = fopen(file_name_input, "w"); // open the file for writing
	if (file_pointer_txt_input == NULL) {
		//perror("No se pudo abrir el archivo fuente.");
		return -1;
	}
	printf("---------------- Creando  Archivo ----------------\n");
	char line1[] = "";
	char line2[] = "índice i★borrado del inicio.";
	char line3[] = "_índice i no es borrado.";
	char line4[] = "★borrado del final.índice i";
	char *buffer_input[] = {line1, line2, line3, line4};
	int buffer_lines = sizeof(buffer_input) / sizeof(buffer_input[0]);
	int i = 0;
	for (i = 0; i < buffer_lines; i++) {
		line_number++;
		bytes_read = strlen(buffer_input[i]);
		line_bytes = get_line_bytes(buffer_input[i], bytes_read);
		if (0 < bytes_read && buffer_input[i][bytes_read - 1] == '\n') {
			printf("%s", buffer_input[i]); // already contains the newline
			if (1 < bytes_read && buffer_input[i][bytes_read - 2] == '\r') {
				printf("Fin de Línea tipo Windows (CR LF)\n");
			} else {
				printf("Fin de Línea tipo Unix (LF)\n");
			}
			fprintf(file_pointer_txt_input, "%s", buffer_input[i]);
		} else {
			printf("%s\n", buffer_input[i]); // adding the newline (\n)
			printf("Fin de Línea sin CR (Carro de Retorno)\n");
			fprintf(file_pointer_txt_input, "%s\r\n", buffer_input[i]); // Windows (CR LF)
			//fprintf(file_pointer_txt_input, "%s\n", buffer_input[i]); // Unix (LF)
		}
		printf("Creados      %'7.0Lf Bytes en Línea %'7.0Lf\n", line_bytes, line_number);
		bytes += line_bytes;
	}
	printf("---------------- Final de Archivo ----------------\n");
	fclose(file_pointer_txt_input); // close the original source file
	return (bytes);
}
void print_usage(char *command) {
	printf("%s\n", "Uso del comando, ejemplo:");
	printf("\t%s %s", command, "[/ruta/]archivo palabra_vieja palabra_nueva");
	printf("\n");
}
void print_help(char *command) {
	printf("%s", "AYUDA");
	printf("\n");
	print_usage(command);
	printf("%s", "NOMBRE:");
	printf("\n");
	printf("\t%s", command);
	printf(" %s", "Reemplaza una palabra por otra en un archivo.");
	printf("\n");
	printf("%s", "OPCIONES:");
	printf("\n");
	printf("%s", "archivo");
	printf("\n");
	printf("\t%s", "Archivo que se quiere actualizar (ruta es opcional).");
	printf("\n");
	printf("%s", "palabra_vieja");
	printf("\n");
	printf("\t%s", "Palabra buscada para reemplazar en un archivo.");
	printf("\n");
	printf("%s", "palabra_nueva");
	printf("\n");
	printf("\t%s", "Palabra que reemplaza las coincidencias (puede estar en blanco).");
	printf("\n");
	printf("%s", "PARÁMETROS CON ESPACIOS:");
	printf("\n");
	printf("\t%s", "Se ingresa entre comillas dobles cada uno que sea necesario.");
	printf("\n");
	printf("%s\n", "Uso del comando, ejemplo:");
	printf("\t%s %s", command, "\"archivo con espacio\" \"palabras con espacios\" \"\"");
	printf("\n");
}
bool confirm_option() {
	bool accepted = false;
	char choice;
	scanf(" %c", &choice); // space before %c to consume any leftover newline (\n)
	clear_input(); // to prevent an issue, always remember to consume any leftover newline (\n)
	choice = toupper(choice); // convert to uppercase for case-insensitive comparison
	switch (choice) {
		case 'S':
		case 'Y':
			// Add code for 'yes' action here
			//printf("%s\n", "Opción aceptada. Continuando...");
			printf("%s\n", "Opción aceptada...");
			accepted = true;
		break;
		default:
			// Handle invalid input, e.g., re-prompt or exit
			// Add code for 'no' action here
			//printf("%s\n", "Opción no válida para (si/no)");
			//printf("%s\n", "Opción rechazada. Saliendo...");
			printf("%s\n", "Opción rechazada...");
		break;
	}
	return (accepted);
}
bool autogen_file(const char *file_name_input) {
	bool accepted = false;
	printf("%s", "Se usará un archivo de prueba... Autogenerar ? (si/no) : ");
	accepted = confirm_option();
	if (accepted) {
		long double bytes = create_file(file_name_input);
		if (bytes < 0) accepted = false;
	}
	return (accepted);
}
bool file_exists(const char *file_name_input) {
	bool exists = false;
	FILE *file_pointer_txt_input = NULL;
	file_pointer_txt_input = fopen(file_name_input, "r"); // open the file for reading
	if (file_pointer_txt_input == NULL) {
		//perror("No se pudo abrir el archivo fuente.");
		return (exists);
	}
	exists = true;
	fclose(file_pointer_txt_input); // close the original source file
	return (exists);
}
int read_input(char **word) {
	//char input_string[] = "Name: John Age: 30 Score: 95.5";
	//int items_read = sscanf(input_string, "Name: %s Age: %d Score: %f", name, &age, &score); // items_read expecting 3 to be successful
	int length = 1024;
	char str_input[length] = {};
	//scanf("%1023s", str_input); // avoid buffer overflows if longer reserving last space for the null terminator (\0)
	fgets(str_input, sizeof(str_input), stdin); // more robust for entire lines of text, last one is null terminator
	str_input[strcspn(str_input, "\n")] = 0; // to prevent an issue, always remember to the trailing newline (\n)
	int bytes_read = strlen(str_input);
	if (0 <= bytes_read) {
		str_input[bytes_read] = '\0';
		char str[bytes_read] = {};
		for (int i = 0; i < bytes_read; i++) {
			strncat(str, &str_input[i], 1);
		}
		//printf("'%s' '%zd' '%s'\n", (*word), bytes_read, str);
		//strcpy(&(*word)[0], str); // DO NOT WORK
		//(*word) = str; // ASSIGN GARBAGE
		//(*word) = "otra vieja"; // WORKING
		// next 3 are working, replaced by realloc
		//free(*word);
		//(*word) = NULL;
		//(*word) = malloc_mem(str);
		(*word) = ralloc_mem(word, str);
	}
	return (bytes_read);
}
int read_word_old(char **word_old) {
	int bytes_read = 0;
	while (bytes_read <= 0) {
		printf("%s", "Favor ingresar la palabra para buscar coincidencias : ");
		bytes_read = read_input(word_old);
		if (bytes_read <= 0) printf("%s\n", "Palabra a buscar no puede estar en blanco.");
	}
	return (bytes_read);
}
int read_word_new(char **word_new) {
	int bytes_read = 0;
	do {
		printf("%s", "Favor ingresar la palabra que reemplaza (opcional) : ");
		bytes_read = read_input(word_new);
		if (bytes_read < 0) printf("%s\n", "Palabra que reemplaza no puede estar en blanco.");
	} while (bytes_read < 0);
	return (bytes_read);
}
bool values_from_args(int n_args, char *arguments[n_args], char **word_old, char **word_new, char **file_name_input) {
	bool accepted = false;
	//printf("args %d\n", n_args);
	//for (int i = 0; i < n_args; i++) printf("argv[%d] %s\n", i, arguments[i]);
	if (n_args <= 1) {
		printf("%s\n", "Faltan datos en los argumentos del programa.");
		accepted = autogen_file(*file_name_input);
		if (!accepted) {
			print_help(arguments[0]);
		}
		return (accepted);
	}
	if (!file_exists(arguments[1])) {
		printf("%s%s%s\n", "No se pudo abrir el archivo \"",arguments[1],"\".");
		accepted = autogen_file(arguments[1]);
		if (!accepted) {
			print_help(arguments[0]);
		} else {
			//(*file_name_input) = arguments[1]; // WORKING
			// next 3 are working, replaced by realloc
			//free(*file_name_input);
			//(*file_name_input) = NULL;
			//(*file_name_input) = malloc_mem(arguments[1]);
			(*file_name_input) = ralloc_mem(file_name_input, arguments[1]);
		}
		return (accepted);
	} else {
		//(*file_name_input) = arguments[1]; // WORKING
		// next 3 are working, replaced by realloc
		//free(*file_name_input);
		//(*file_name_input) = NULL;
		//(*file_name_input) = malloc_mem(arguments[1]);
		(*file_name_input) = ralloc_mem(file_name_input, arguments[1]);
	}
	if (n_args <= 2) {
		printf("%s\n", "Falta la palabra para buscar coincidencias.");
		printf("%s", "Quiere ingresar una palabra... Continuar ? (si/no) : ");
		accepted = confirm_option();
		if (!accepted) {
			print_help(arguments[0]);
			return (accepted);
		} else {
			read_word_old(word_old);
		}
	} else {
		//(*word_old) = arguments[2]; // WORKING
		// next 3 are working, replaced by realloc
		//free(*word_old);
		//(*word_old) = NULL;
		//(*word_old) = malloc_mem(arguments[2]);
		(*word_old) = ralloc_mem(word_old, arguments[2]);
	}
	if (n_args <= 3) {
		printf("%s\n", "La palabra que reemplaza se encuentra en blanco.");
		printf("%s", "Las coincidencias se borrarán... Continuar ? (si/no) : ");
		accepted = confirm_option();
		if (!accepted) {
			printf("%s", "Quiere ingresar una palabra... Continuar ? (si/no) : ");
			accepted = confirm_option();
			if (!accepted) {
				print_help(arguments[0]);
				return (accepted);
			} else {
				read_word_new(word_new);
				accepted = true;
			}
		} else {
			//(*word_new) = ""; // WORKING
			// next 3 are working, replaced by realloc
			//free(*word_new);
			//(*word_new) = NULL;
			//(*word_new) = malloc_mem("");
			(*word_new) = ralloc_mem(word_new, "");
			accepted = true;
		}
	} else {
		//(*word_new) = arguments[3]; // WORKING
		// next 3 are working, replaced by realloc
		//free(*word_new);
		//(*word_new) = NULL;
		//(*word_new) = malloc_mem(arguments[3]);
		(*word_new) = ralloc_mem(word_new, arguments[3]);
		accepted = true;
	}
	return (accepted);
}
int main(int argc, char *argv[]) {
	// Set the locale to support Unicode UTF-8 (or your desired locale)
	//setlocale(LC_NUMERIC, "en_US.UTF-8"); // for using a thousands separator
	setlocale(LC_ALL, "en_US.UTF-8"); // for using a thousands separator
	//setlocale(LC_CTYPE, ""); // for character handling to the user's default
	//setlocale(LC_ALL, ""); // Set to the user environment's default locale
	// \e escape character for embedding an ANSI sequence
	// \033[38;2 is the ASCII 27 escape character of \e
	// \x1b[38;2 is the HEX escape character of \e
	// [2J indicates to clear the entire screen
	// [1;1H moves the cursor to the top-left position (row 1, column 1)
	//printf("\e[2J\e[1;1H"); // clear the terminal NOT WORKING
	//printf("\033[2J\033[1;1H"); // clear the terminal NOT WORKING
	//printf("\x1b[2J\x1b[1;1H"); // clear the terminal NOT WORKING
	//system("clear"); // clear the terminal WORKING
	char *word_old = malloc_mem("índice i");
	char *word_new = malloc_mem("");
	char *file_name_input = malloc_mem("");
	char *file_name_temp = malloc_mem("lab04e2replace.c.txt");
	//printf("args %d\n", argc);
	//for (int i = 0; i < argc; i++) printf("argv[%d] %s\n", i, argv[i]);
	if (!values_from_args(argc, argv, &word_old, &word_new, &file_name_input)) {
		free_all(&word_old, &word_new, &file_name_input, &file_name_temp);
		return (EXIT_FAILURE); // terminate and return control to where is called from
	}
	int length = 1024;
	char file_name[length] = {};
	strcpy(&file_name[0], argv[0]);
	//int file_len = strlen(file_name);
	char ext_str[] = {'.', 't', 'x', 't', '\0'};
	int ext_len = strlen(ext_str);
	for (int i = 0; i < ext_len; i++) {
		strncat(file_name, &ext_str[i], 1);
	}
	//printf("name_len %d ext_len %d new_len %zd '%s'\n", file_len, ext_len, strlen(file_name), file_name);
	file_name_temp = ralloc_mem(&file_name_temp, file_name);
	//printf("name_len %d ext_len %d new_len %zd '%s'\n", file_len, ext_len, strlen(file_name_temp), file_name_temp);
	//printf("'%s' '%s' '%s' '%s'\n", word_old, word_new, file_name_input, file_name_temp);
	long double matches = 0;
	long double words = get_by_line(word_old, word_new, &matches, file_name_input, file_name_temp);
	if (words < 0) {
		//printf("No se pudo abrir el archivo para reemplazar palabras\n");
		printf("%s%s%s\n", "No se pudo abrir el archivo \"",file_name_input,"\".");
		free_all(&word_old, &word_new, &file_name_input, &file_name_temp);
		return (EXIT_FAILURE); // terminate and return control to where is called from
	}
	//printf("Coincidencias  Encontradas  %'7.0Lf\n", (matches));
	//printf("Coincidencias Reemplazadas  %'7.0Lf\n", words);
	//replace_file(file_name_input, file_name_temp);
	free_all(&word_old, &word_new, &file_name_input, &file_name_temp);
	/*
	free(word_old);
	word_old = NULL;
	free(word_new);
	word_new = NULL;
	free(file_name_input);
	file_name_input = NULL;
	free(file_name_temp);
	file_name_temp = NULL;
	*/
	/*
	int bytes = 0;
	char *symbol = "", *haystack = ALLOWED;
	long double symbols_count = get_line_symbols(haystack, strlen(haystack));
	// e occupies 1 byte, so needs 3 to complete 4 bytes \x65\x00\x00\x00
	symbol = "\x65";
	symbol = "e";
	bytes = get_utf8_char_len(symbol[0]);
	printf("buscando símbolo '%s' de %d %s en %'.0Lf acentos permitidos\n", symbol, bytes, ((bytes == 1) ? "byte" : "bytes"), symbols_count);
	printf("símbolo '%s' de %d %s %s una letra/vocal con acento\n", symbol, bytes, ((bytes == 1) ? "byte" : "bytes"), (alphas(symbol[0],'\x00','\x00','\x00',FORWARD) ? "es" : "no es"));
	// é occupies 2 bytes, so needs 2 to complete 4 bytes \xC3\xA9\x00\x00
	symbol = "\xC3\xA9";
	symbol = "é";
	bytes = get_utf8_char_len(symbol[0]);
	printf("buscando símbolo '%s' de %d %s en %'.0Lf acentos permitidos\n", symbol, bytes, ((bytes == 1) ? "byte" : "bytes"), symbols_count);
	printf("símbolo '%s' de %d %s %s una letra/vocal con acento\n", symbol, bytes, ((bytes == 1) ? "byte" : "bytes"), (alphas(symbol[0],symbol[1],'\x00','\x00',FORWARD) ? "es" : "no es"));
	// € occupies 3 bytes, so needs 1 to complete 4 bytes \xE2\x82\xAC\x00
	symbol = "\xE2\x82\xAC";
	symbol = "€";
	bytes = get_utf8_char_len(symbol[0]);
	printf("buscando símbolo '%s' de %d %s en %'.0Lf acentos permitidos\n", symbol, bytes, ((bytes == 1) ? "byte" : "bytes"), symbols_count);
	printf("símbolo '%s' de %d %s %s una letra/vocal con acento\n", symbol, bytes, ((bytes == 1) ? "byte" : "bytes"), (alphas(symbol[0],symbol[1],symbol[2],'\x00',FORWARD) ? "es" : "no es"));
	*/
	// exit () performs standard program termination cleanup
	// return generally does not involve program-wide cleanup
	//exit (EXIT_FAILURE); // terminate immediately, regardless where is called from
	return (EXIT_SUCCESS); // terminate and return control to where is called from
}
