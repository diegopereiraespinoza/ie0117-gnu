/*
// This script compares different types of factorial ranges
// Tested on: Arch Linux | Linux Mint
// Tested by: Diego Pereira Espinoza
// Date: 2025-09-22
*/
/*
// Include of some dependencies/libraries
*/
#define _GNU_SOURCE // Required by some systems for strsep, asprintf
#include <locale.h> // For use a thousands separator in numbers by printf
#include <stdio.h>  // For printf, scanf
#include <string.h> // For memmove
#include <stdlib.h> // For rand, srand, system, strtok, strtol, malloc, realloc, free, if dynamic allocation is used
#include <limits.h> // For limits of: char / short / int / long / long long
#include <float.h> // For limits of: float / double / long double
#include <errno.h>  // For errno
#include <ctype.h> // Required by trim for isspace
/*
// trim is used to delete spaces from start/end of a string
*/
void trim(char *s) {
	if (s == NULL || *s == '\0') {
		return; // Handle null or empty strings
	}
	// Trim leading whitespace
	char *start = s;
	while (*start != '\0' && isspace((unsigned char)*start)) {
		start++;
	}
	// If the entire string was whitespace, set it to an empty string
	if (*start == '\0') {
		*s = '\0';
		return;
	}
	// Trim trailing whitespace
	char *end = s + strlen(s) - 1;
	while (end >= start && isspace((unsigned char)*end)) {
		end--;
	}
	// Shift characters to the beginning if leading whitespace was removed
	if (start != s) {
		char *dest = s;
		while (start <= end) {
			*dest++ = *start++;
		}
		*dest = '\0'; // Null-terminate the new string
	} else {
		// No leading whitespace removed, just null-terminate at the new end
		*(end + 1) = '\0';
	}
}
/*
// Calculate factorial, one function for each type of number
*/
long double factorial_long_double(int n) {
	if (n < 0) {
		// Factorial no definido para negativos
		return 0;
	}
	long double i = 1;
	while (n > 1) {
		i = i * n;
		n = n - 1;
		if ((errno == ERANGE || (i <= LDBL_MIN || LDBL_MAX <= i))) return -1;
	}
	return i;
}
double factorial_double(int n) {
	if (n < 0) {
		// Factorial no definido para negativos
		return 0;
	}
	double i = 1;
	while (n > 1) {
		i = i * n;
		n = n - 1;
	}
	return i;
}
float factorial_float(int n) {
	if (n < 0) {
		// Factorial no definido para negativos
		return 0;
	}
	float i = 1;
	while (n > 1) {
		i = i * n;
		n = n - 1;
	}
	return i;
}
long long factorial_long_long(int n) {
	if (n < 0) {
		// Factorial no definido para negativos
		return 0;
	}
	long long i = 1;
	while (n > 1) {
		i = i * n;
		n = n - 1;
	}
	return i;
}
long factorial_long(int n) {
	if (n < 0) {
		// Factorial no definido para negativos
		return 0;
	}
	long i = 1;
	while (n > 1) {
		i = i * n;
		n = n - 1;
	}
	return i;
}
int factorial_int(int n) {
	if (n < 0) {
		// Factorial no definido para negativos
		return 0;
	}
	int i = 1;
	while (n > 1) {
		i = i * n;
		n = n - 1;
	}
	return i;
}
short factorial_short(int n) {
	if (n < 0) {
		// Factorial no definido para negativos
		return 0;
	}
	short i = 1;
	while (n > 1) {
		i = i * n;
		n = n - 1;
	}
	return i;
}
/*
// Print monochromatic factorial, one function for each type of number
*/
void print_factorial_ld_theoric(int first, int last) {
	char *notation_mode = "%'.Lf\n";
	for (int n = first; n <= last; n++) {
		long double fact_t = factorial_long_double(n);
		printf("     Teorico %'d! : ", n);
		if (fact_t != 0) {
			if (fact_t > FLT_MAX) notation_mode = "%LE\n";
			if (fact_t != -1) printf(notation_mode, fact_t); else printf("excede el rango\n");
		} else {
			printf("no definido\n");
		}
	}
}
void print_factorial_long_double(int first, int last) {
	char *notation_mode = "%'.Lf\n";
	printf("------------ Factorial (long double) ------------\n");
	for (int n = first; n <= last; n++) {
		//print_factorial_ld_theoric(n, n);
		long double fact_e = factorial_long_double(n);
		printf("Experimental %'d! : ", n);
		if (fact_e != 0) {
			if (fact_e > FLT_MAX) notation_mode = "%LE\n";
			if (fact_e != -1) printf(notation_mode, fact_e); else printf("excede el rango\n");
		} else {
			printf("no definido\n");
		}
	}
}
void print_factorial_double(int first, int last) {
	printf("-------------- Factorial  (double) --------------\n");
	for (int n = first; n <= last; n++) {
		print_factorial_ld_theoric(n, n);
		double fact_e = factorial_double(n);
		printf("Experimental %'d! : ", n);
		if (fact_e != 0) {
			printf("%'.lf\n", fact_e);
		} else {
			printf("no definido\n");
		}
	}
}
void print_factorial_float(int first, int last) {
	printf("--------------- Factorial (float) ---------------\n");
	for (int n = first; n <= last; n++) {
		print_factorial_ld_theoric(n, n);
		float fact_e = factorial_float(n);
		printf("Experimental %'d! : ", n);
		if (fact_e != 0) {
			printf("%'.f\n", fact_e);
		} else {
			printf("no definido\n");
		}
	}
}
void print_factorial_long_long(int first, int last) {
	printf("------------- Factorial (long long) -------------\n");
	for (int n = first; n <= last; n++) {
		print_factorial_ld_theoric(n, n);
		long long fact_e = factorial_long_long(n);
		printf("Experimental %'d! : ", n);
		if (fact_e != 0) {
			printf("%'lld\n", fact_e);
		} else {
			printf("no definido\n");
		}
	}
}
void print_factorial_long(int first, int last) {
	printf("--------------- Factorial  (long) ---------------\n");
	for (int n = first; n <= last; n++) {
		print_factorial_ld_theoric(n, n);
		long fact_e = factorial_long(n);
		printf("Experimental %'d! : ", n);
		if (fact_e != 0) {
			printf("%'ld\n", fact_e);
		} else {
			printf("no definido\n");
		}
	}
}
void print_factorial_int(int first, int last) {
	printf("---------------- Factorial (int) ----------------\n");
	for (int n = first; n <= last; n++) {
		print_factorial_ld_theoric(n, n);
		int fact_e = factorial_int(n);
		printf("Experimental %'d! : ", n);
		if (fact_e != 0) {
			printf("%'d\n", fact_e);
		} else {
			printf("no definido\n");
		}
	}
}
void print_factorial_short(int first, int last) {
	printf("--------------- Factorial (short) ---------------\n");
	for (int n = first; n <= last; n++) {
		print_factorial_ld_theoric(n, n);
		short fact_e = factorial_short(n);
		printf("Experimental %'d! : ", n);
		if (fact_e != 0) {
			printf("%'d\n", fact_e);
		} else {
			printf("no definido\n");
		}
	}
}
/*
// Print monochromatic limits for each type of number (exponential mode)
*/
void print_limits_exponential() {
	printf("--------------- Rango Exponencial ---------------\n");
    printf("---------------- Default: signed ----------------\n");
    printf("Min       (short) : %E\n", (float)SHRT_MIN);
    printf("Max       (short) : +%E\n", (float)SHRT_MAX);
    printf("Max     (u short) : +%E\n", (float)USHRT_MAX);
    printf("Min         (int) : %E\n", (float)INT_MIN);
    printf("Max         (int) : +%E\n", (float)INT_MAX);
    printf("Max       (u int) : +%E\n", (float)UINT_MAX);
    printf("Min        (long) : %E\n", (float)LONG_MIN);
    printf("Max        (long) : +%E\n", (float)LONG_MAX);
    printf("Max      (u long) : +%E\n", (float)ULONG_MAX);
    printf("Min   (long long) : %E\n", (float)LLONG_MIN);
    printf("Max   (long long) : +%E\n", (float)LLONG_MAX);
    printf("Max (u long long) : +%E\n", (float)ULLONG_MAX);
	printf("--------------- Rango Exponencial ---------------\n");
    printf("--------------- Default: unsigned ---------------\n");
    printf("Min       (float) : +%E\n", FLT_MIN);
    printf("Max       (float) : +%E\n", FLT_MAX);
    printf("Min      (double) : +%E\n", DBL_MIN);
    printf("Max      (double) : +%E\n", DBL_MAX);
    printf("Min (long double) : +%LE\n", LDBL_MIN);
    printf("Max (long double) : +%LE\n", LDBL_MAX);
}
/*
// Print monochromatic limits for each type of number (decimal mode)
*/
void print_limits_decimal() {
	printf("----------------- Rango Decimal -----------------\n");
    printf("---------------- Default: signed ----------------\n");
    printf("Min       (short) : %'d\n", SHRT_MIN);
    printf("Max       (short) : +%'d\n", SHRT_MAX);
    printf("Max     (u short) : +%'d\n", USHRT_MAX);
    printf("Min         (int) : %'d\n", INT_MIN);
    printf("Max         (int) : +%'d\n", INT_MAX);
    printf("Max       (u int) : +%'u\n", UINT_MAX);
    printf("Min        (long) : %'ld\n", LONG_MIN);
    printf("Max        (long) : +%'ld\n", LONG_MAX);
    printf("Max      (u long) : +%'lu\n", ULONG_MAX);
    printf("Min   (long long) : %'lld\n", LLONG_MIN);
    printf("Max   (long long) : +%'lld\n", LLONG_MAX);
    printf("Max (u long long) : +%'llu\n", ULLONG_MAX);
	printf("----------------- Rango Decimal -----------------\n");
    printf("--------------- Default: unsigned ---------------\n");
    printf("Min       (float) : +%'.f\n", FLT_MIN);
    printf("Max       (float) : +%'.f\n", FLT_MAX);/*
    printf("Min      (double) : +%'.lf\n", DBL_MIN);
    printf("Max      (double) : +%'.lf\n", DBL_MAX);
    printf("Min (long double) : +%'.Lf\n", LDBL_MIN);
    printf("Max (long double) : +%'.Lf\n", LDBL_MAX);*/
}
void print_monochrome() {
	print_limits_decimal();
	print_limits_exponential();
	print_factorial_short(7, 8);
	print_factorial_int(12, 13);
	print_factorial_long(20, 21);
	print_factorial_long_long(20, 21);
	print_factorial_float(13, 14);
	print_factorial_double(22, 23);
	// after float max value will be printed in exponential mode
	// after factorial 35 exceeds float max value of 39 digits
	print_factorial_long_double(34, 35);
	// after factorial 1755 exceeds long double max vlue
	print_factorial_long_double(1754, 1755);
}
int get_int_greater_than_zero(int n_size, char *arguments[n_size]) {
	int num = 0;
	for (int i = 0; i < n_size; i++) {
		num = 0;
		char *str_mixed = arguments[i];
		trim(str_mixed); // Should handle null gracefully
		//printf("Índice %d : '%s'\n", i, str_mixed);
		// Example with a string containing non-numeric characters
		errno = 0; // Clear errno before the call
		char *endptr;
		long val = strtol(str_mixed, &endptr, 10); // Base 10 for decimal conversion
		// Check for errors
		//if (errno == ERANGE) {
		if ((errno == ERANGE || (val <= INT_MIN || INT_MAX <= val))) {
			//printf("\nError: valor excede el rango del tipo int\n");
			continue;
		} else if (endptr == str_mixed) {
			//printf("Error: texto '%s' no inicia con un número\n", str_mixed);
			continue;
		} else if (*endptr != '\0') {
			//printf("Aviso: conversión parcial de '%ld', pendientes '%s'\n", val, endptr);
			num = (int)val;
		} else {
			//printf("Éxito: texto convertido al número '%ld'\n", val);
			num = (int)val;
		}
		if (0 < num) return num;
	}
	return num;
}
int read_int_from_input() {
	//char input_string[] = "Name: John Age: 30 Score: 95.5";
	//int items_read = sscanf(input_string, "Name: %s Age: %d Score: %f", name, &age, &score); // items_read expecting 3 to be successful
	int input_size = 20;
	char originalString[input_size]; // String to be split
	//scanf("%19s", originalString); // avoid buffer overflows if longer reserving last space for the null terminator
	fgets(originalString, sizeof(originalString), stdin); // more robust for entire lines of text, last one is null terminator
	const char *delimiter = " "; // Delimiter character
	char *string_array[input_size]; // Array to store pointers to string_array
	int tokenCount = 0;
	char *token;
	// strsep() is similar to strtok() but is generally considered safer and more robust in some scenarios
	// particularly when dealing with empty tokens or consecutive delimiters.
	// It also modifies the original string.
	/*
	while ((token = strsep(&originalString, delimiter)) != NULL && tokenCount < input_size) {
		string_array[tokenCount] = token;// strsep gets also NULL/EMPTY tokens, while strtok DOES NOT
		tokenCount++;
	}*/
	// Get the first token
	token = strtok(originalString, delimiter);
	// Loop through the rest of the string_array
	while (token != NULL && tokenCount < input_size) {
		string_array[tokenCount] = token; // Store the token
		tokenCount++;
		token = strtok(NULL, delimiter); // Get the next token (NOT NULL/EMPTY)
	}
	return get_int_greater_than_zero(tokenCount, string_array);
}
/*
// Main entry from where different functions are called
*/
int main(int argc, char *argv[]) {
	//setlocale(LC_ALL, ""); // Set to the user's default locale
	// Set locale to one that uses a thousands separator (e.g., en_US.UTF-8)
	//setlocale(LC_ALL, "en_US.UTF-8");
	setlocale(LC_NUMERIC, "en_US.UTF-8");
	//printf("\e[2J\e[1;1H"); // clear the terminal NOT WORKING
	// \e[2J\e[1;1H is an embeded ANSI sequence
	// \033[2J\033[1;1H is an embeded ANSI sequence
	// \033 is the ASCII escape character of \e (ASCII 27 or HEX \x1b)
	// [2J indicates to clear the entire screen
	// [1;1H moves the cursor to the top-left position (row 1, column 1)
	//printf("\033[2J\033[1;1H"); // clear the terminal NOT WORKING
	system("clear"); // clear the terminal WORKING
	print_monochrome();
	int num = get_int_greater_than_zero(argc, argv);
	while (num <= 0) {
		printf("Ingrese un número para calcular el factorial : ");
		num = read_int_from_input();
		if (num < 0) printf("Se requiere un entero igual/mayor que cero\n");
		else break;
	}
	// after factorial 1755 exceeds long double max vlue
	print_factorial_long_double(num, num);
	return 0;
}
