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
// Define macros for global constants
*/
#ifndef MATRIX_SIZE
	#define MATRIX_SIZE 3 // defined once only
#endif
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
// Function to check if the matrix is a magic square to return the magic constant
int get_magic_const(int n_size, int magic_square[n_size][n_size]) {
	int magic_constant = -1;
	if (0 < n_size) {
		magic_constant = (n_size != 2) ? ((n_size * (n_size * n_size + 1)) / 2) : 2;
		int sum_of_line = 0;
		// check diagonal 1
		sum_of_line = 0;
		for (int i = 0; i < n_size; i++) {
			sum_of_line += magic_square[i][i];
		}
		if (sum_of_line != magic_constant) return -1;
		// check diagonal 2
		sum_of_line = 0;
		for (int i = (n_size - 1); 0 <= i; i--) {
			sum_of_line += magic_square[i][n_size-i-1];
		}
		if (sum_of_line != magic_constant) return -1;
		// check rows
		for (int i = 0; i < n_size; i++) {
			sum_of_line = 0;
			for (int j = 0; j < n_size; j++) {
				sum_of_line += magic_square[i][j];
			}
			if (sum_of_line != magic_constant) return -1;
		}
		// check cols
		for (int j = 0; j < n_size; j++) {
			sum_of_line = 0;
			for (int i = 0; i < n_size; i++) {
				sum_of_line += magic_square[i][j];
			}
			if (sum_of_line != magic_constant) return -1;
		}
	}
	return magic_constant;
}
// Function to print the magic constant
void print_magic_const(int n_size, int magic_square[n_size][n_size]) {
	// magic_doubly_v3 para multiplos de 4
	int magic_constant = get_magic_const(n_size, magic_square);
	printf("Constante Mágica  : ");
	if (0 < magic_constant) {
		printf("%d\n", magic_constant);
	} else {
		printf("%s\n", "no es un cuadrado mágico");
	}
}
// Function to print monochromatic the magic square
void print_magic_square(int n_size, int magic_square[n_size][n_size]) {
	char *notation_mode = "%6d";
	printf("---------------- Matriz Cuadrada ----------------\n");
	printf("De Tamaño / Orden : ");
	if (0 < n_size) {
		printf("%d\n", n_size);
		for (int i = 0; i < n_size; i++) {
			for (int j = 0; j < n_size; j++) {
				printf(notation_mode, magic_square[i][j]);
			}
			printf("\n");
		}
	} else {
		printf("%s\n", "fuera de rango");
	}
}
int get_int_random(int min, int max) {
	return (min + (rand() % (max - min + 1)));
}
// Function to generate an odd-ordered magic square
void fill_magic_odd_v1(int n_size, int magic_square[n_size][n_size], int randomly) {
	// magic_odd_v1 version v1 used in lab example
	// Initialize all cells to 0
	for (int i = 0; i < n_size; i++) {
		for (int j = 0; j < n_size; j++) {
			magic_square[i][j] = 0;
		}
	}
	// Start position for 1
	int i = (n_size / 2);
	int j = (n_size - 1);
	// Fill the square
	for (int num = 1; num <= n_size * n_size; ) {
		if (i == -1 && j == n_size) { // 3rd condition: out of bounds on both axes
			i = 0;
			j = (n_size - 2);
		} else {
			if (i < 0) i = (n_size - 1); // Wrap around row
			if (j == n_size) j = 0; // Wrap around column
		}
		if (magic_square[i][j] != 0) { // 2nd condition: cell already filled
			i++;
			j -= 2;
			continue;
		} else {
			if (randomly != 1) magic_square[i][j] = num++; // Place number
			else magic_square[i][j] = get_int_random(1, (n_size * n_size)); // Place random
		}
		i--;
		j++; // Move next: up-right
	}
}
// Function to generate an odd-ordered magic square
// Implement Siamese ((De la Loubère/De la Laubera) method here to fill a (n_size) x (n_size) square
// starting from num = 1, (1*(n_size/2)*(n_size/2)), (2*(n_size/2)*(n_size/2)), (3*(n_size/2)*(n_size/2))...
void fill_magic_odd_v2(int n_size, int magic_square[n_size][n_size], int randomly) {
	// magic_odd_v2 version v2 may works better than v1
	for (int i = 0; i < n_size; i++) {
		for (int j = 0; j < n_size; j++) {
			magic_square[i][j] = 0; // Initialize with 0
		}
	}
	int i = 0; // Current row
	int j = (n_size /2); // Current column (middle of top row)
	for (int num = 1; num <= n_size * n_size; num++) {
		if (randomly != 1) magic_square[i][j] = num;
		else magic_square[i][j] = get_int_random(1, (n_size * n_size)); // Place random
		int next_i = (i - 1 + n_size) % n_size; // Move up, wrap around
		int next_j = (j + 1) % n_size;     // Move right, wrap around
		if (magic_square[next_i][next_j] != 0) { // If next cell is occupied
			i = (i + 1) % n_size; // Move one cell down
		} else {
			i = next_i;
			j = next_j;
		}
	}
}
// Function to generate a doubly even magic square
void fill_magic_even_doubly(int n_size, int magic_square[n_size][n_size], int randomly) {
	// magic_doubly_v2 para multiplos de 4
	// Fill the square with numbers 1 to n_size*n_size
	// and then mark elements to be swapped
	for (int i = 0; i < n_size; i++) {
		for (int j = 0; j < n_size; j++) {
			if (randomly != 1) magic_square[i][j] = (n_size * i) + j + 1;
			else magic_square[i][j] = get_int_random(1, (n_size * n_size)); // Place random
		}
	}
	// Swap elements in sub-squares of size (n_size/4) x (n_size/4)
	int quart_size = (n_size / 4);
	// Top-left quart
	for (int i = 0; i < quart_size; i++) {
		for (int j = 0; j < quart_size; j++) {
			magic_square[i][j] = (n_size * n_size + 1) - magic_square[i][j];
		}
	}
	// Top-right quart
	for (int i = 0; i < quart_size; i++) {
		for (int j = n_size - quart_size; j < n_size; j++) {
			magic_square[i][j] = (n_size * n_size + 1) - magic_square[i][j];
		}
	}
	// Bottom-left quart
	for (int i = n_size - quart_size; i < n_size; i++) {
		for (int j = 0; j < quart_size; j++) {
			magic_square[i][j] = (n_size * n_size + 1) - magic_square[i][j];
		}
	}
	// Bottom-right quart
	for (int i = n_size - quart_size; i < n_size; i++) {
		for (int j = n_size - quart_size; j < n_size; j++) {
			magic_square[i][j] = (n_size * n_size + 1) - magic_square[i][j];
		}
	}
	// Center 2x2 block (for size = 4, this is the entire inner 2x2)
	// For larger size, this applies to the central (n_size / 2) x (n_size / 2) block,
	// swapping the inner (n_size / 4) x (n_size / 4) sub-blocks
	for (int i = quart_size; i < n_size - quart_size; i++) {
		for (int j = quart_size; j < n_size - quart_size; j++) {
			magic_square[i][j] = (n_size * n_size + 1) - magic_square[i][j];
		}
	}
}
// Function to generate a doubly even magic square
// The LUX method was devised by J.H. Conway
// for m = 1,2,3,4,5,6,7,8,9... n_size = (4m + 2) = 6, 10, 14, 18, 22, 26, 30, 34, 38...
void fill_magic_even_singly(int n_size, int magic_square[n_size][n_size], int randomly) {
	int temp_num, half_size = (n_size / 2), magic_odd[half_size][half_size];
	int m_index = ((n_size - 2) / 4), swapped_cols = 0, magic_swap[n_size];
	// Initialize squares to 0
	for (int i = 0; i < n_size; i++) {
		for (int j = 0; j < n_size; j++) {
			magic_square[i][j] = 0;
		}
	}
	for (int i = 0; i < half_size; i++) {
		for (int j = 0; j < half_size; j++) {
			magic_odd[i][j] = 0;
		}
	}
	// Create the odd-order magic square
	fill_magic_odd_v2(half_size, magic_odd, randomly);
	// Fill the four sub-squares (L, U, X, and the fourth)
	for (int i = 0; i < half_size; i++) {
		for (int j = 0; j < half_size; j++) {
			magic_square[i][j] = magic_odd[i][j]; // A=L
			magic_square[i+half_size][j+half_size] = magic_odd[i][j] + (1*half_size*half_size); // D=1*X
			magic_square[i][j+half_size] = magic_odd[i][j] + (2*half_size*half_size); // B=3*Fourth region
			magic_square[i+half_size][j] = magic_odd[i][j] + (3*half_size*half_size); // C=2*U
		}
	}
	// Perform swaps according to the LUX method
	// left half of the square are swapped with corresponding elements in the right half
	for (int i = 1; i <= m_index; i++) {
		magic_swap[swapped_cols++] = i;
	}
	for (int i = (n_size - m_index + 2); i <= n_size; i++) {
		magic_swap[swapped_cols++] = i;
	}
	for (int i = 1; i <= half_size; i++) {
		for (int j = 1; j <= swapped_cols; j++) {
			temp_num = magic_square[i-1][magic_swap[j-1]-1];
			magic_square[i-1][magic_swap[j-1]-1] = magic_square[i+half_size-1][magic_swap[j-1]-1];
			magic_square[i+half_size-1][magic_swap[j-1]-1] = temp_num;
		}
	}
	// Swap middle elements of middle rows
	int i = m_index; 
	int j = 0;
	temp_num = magic_square[i][j]; 
	magic_square[i][j] = magic_square[i+half_size][j]; 
	magic_square[i+half_size][j] = temp_num;
	j = i;
	temp_num = magic_square[i+half_size][j]; 
	magic_square[i+half_size][j] = magic_square[i][j]; 
	magic_square[i][j] = temp_num;
}
// Function to fill a matrix according to given size
void fill_magic_square(int n_size, int magic_square[n_size][n_size], int randomly) {
	if (n_size == 1) {
		//magic_square[n_size][n_size] = {1};
		magic_square[0][0] = 1;
	} else if (n_size == 2) {
		//magic_square[n_size][n_size] = {1};
		if (randomly != 1) {
			magic_square[0][0] = 1;
			magic_square[0][1] = 1;
			magic_square[1][0] = 1;
			magic_square[1][1] = 1;
		} else {
			magic_square[0][0] = get_int_random(1, (n_size * n_size)); // Place random
			magic_square[0][1] = get_int_random(1, (n_size * n_size)); // Place random
			magic_square[1][0] = get_int_random(1, (n_size * n_size)); // Place random
			magic_square[1][1] = get_int_random(1, (n_size * n_size)); // Place random
		}
	} else if ((n_size % 2) == 1) {
		fill_magic_odd_v2(n_size, magic_square, randomly);
	} else if ((n_size % 4) == 0) {
		fill_magic_even_doubly(n_size, magic_square, randomly);
	} else {
		fill_magic_even_singly(n_size, magic_square, randomly);
	}
}
int get_int_greater_than_zero(int n_size, char *arguments[n_size]) {
	for (int i = 0; i < n_size; i++) {
		int num = 0;
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
	return -1;
}
int read_size_from_input() {
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
	int n_size = 3;
	/*
	printf("Arguments '%d'\n", argc);
	for (int j = 0; j < argc; j++) {
		printf("argv[%d] = ", j);
		printf("'%s'\n", argv[j]);
	}
	//int matrix_eg[n_size][n_size] = {{2, 7, 6},{9, 5, 1},{4, 3, 8}}; // initializer not allowed
	//int matrix_eg[n_size][n_size] = {}; // except with an empty initializer of zeros
	int matrix_eg[MATRIX_SIZE][MATRIX_SIZE] = {{2, 7, 6},{9, 5, 1},{4, 3, 8}}; // except with macros
	print_magic_square(n_size, matrix_eg);
	print_magic_const(n_size, matrix_eg);
	*/
	n_size = get_int_greater_than_zero(argc, argv);
	while (n_size <= 0) {
		printf("Ingrese el orden/tamaño de la matriz : ");
		n_size = read_size_from_input();
		if (n_size <= 0) printf("Se requiere un entero mayor que cero\n");
	}
	// Initialice is not mandatory
	int magic_square[n_size][n_size];
	// Fill a magic square
	int randomly = 0;
	fill_magic_square(n_size, magic_square, randomly);
	// Print the magic square
	print_magic_square(n_size, magic_square);
	print_magic_const(n_size, magic_square);
	// Fill a random matrix
	randomly = 1;
	fill_magic_square(n_size, magic_square, randomly);
	// Print the random matrix
	print_magic_square(n_size, magic_square);
	print_magic_const(n_size, magic_square);
	/*
	for (int i = -2; i < 7; i++) {
		int magic_square[i][i];
		// Print the magic square
		fill_magic_square(i, magic_square, 0);
		print_magic_square(i, magic_square);
		print_magic_const(i, magic_square);
		// Print the random matrix
		fill_magic_square(i, magic_square, 1);
		print_magic_square(i, magic_square);
		print_magic_const(i, magic_square);
	}*/
	return 0;
}
