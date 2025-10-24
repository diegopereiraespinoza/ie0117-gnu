#define _GNU_SOURCE // Required by some systems for strsep, asprintf
#include <locale.h> // For setlocale to use a thousands separator in numbers by printf
//#include <libio.h>  // For fwide
#include <stdio.h>  // For fprintf, printf, scanf, ...
#include <string.h> // For memmove, strlen, though getline returns length
#include <stdlib.h> // For rand, srand, system, strtok, strtol, malloc, realloc, free, if dynamic allocation is used
#include <time.h>   // For seeding rand/random with time
//#include <limits.h> // For limits of: char / short / int (INT_MIN/INT_MAX) / long / long long (LLONG_MIN/LLONG_MAX)
//#include <float.h>  // For limits of: float (FLT_MIN/FLT_MAX) / double / long double (LDBL_MIN/LDBL_MAX)
#include <errno.h>  // For errno == ERANGE, clear errno=0 before call strlen, strcpy, strstr, strtok, strtol, ...
#include <ctype.h>  // For toupper, tolower, isalpha, isdigit, isspace (trim spaces from words)
//#include <iconv.h>  // For decimal ASCII to UTF-8
#include <stdbool.h>// For bool types
//#include <uchar.h>  // For uchar_t on estandar UTF-8
//#include <wchar.h>  // For wchar_t and wide character functions
/*
// To sort an array in C without using an extra array, you need to employ an in-place sorting algorithm. 
// Several algorithms achieve this by manipulating elements directly within the original array's memory. 
// Common examples include:
// Bubble Sort:
// This algorithm repeatedly steps through the list, compares adjacent elements, and swaps them 
// if they are in the wrong order. The pass through the list is repeated until no swaps are needed, 
// indicating that the list is sorted. It has a time complexity of O(n^2) in the worst and average cases.
*/
/*
// global constants defined by macros
*/
#ifndef ZEROED
	#define ZEROED 0 // fill a matrix by index or with zeros
#endif
#ifndef BYINDEX
	#define BYINDEX 1 // fill a matrix by index or with zeros
#endif
#ifndef RANDOM1
	#define RANDOM1 2 // fill a matrix with random numbers
#endif
#ifndef RANDOM2
	#define RANDOM2 3 // fill a matrix with random numbers (more efficient approach)
#endif
#ifndef REPEAT_AVOID
	#define REPEAT_AVOID 0 // repeated numbers are not allowed
#endif
#ifndef REPEAT_ALLOW
	#define REPEAT_ALLOW 1 // repeated numbers can occur
#endif
#ifndef LENGTH_MIN
	#define LENGTH_MIN 2 // minimum rows/cols for a matrix
#endif
#ifndef LENGTH_MAX
	#define LENGTH_MAX 15 // maximum rows/cols for a matrix
#endif
#ifndef DESCENDING
	#define DESCENDING 0 // flag to order an array high to low
#endif
#ifndef ASCENDING
	#define ASCENDING 1 // flag to order an array low to high
#endif
// function to retreive a random number between min and max (inclusive)
int rand_int(int min, int max) {
	// rand() is considered a lower-quality pseudo-random number generator (PRNG)
	// it also requires explicit seeding with srand()
	// it is crucial to seed with time to ensure a different sequence each time
	//srand(time(NULL)); // seed the rand() number generator only once from main
	return (min + (rand() % (max - min + 1)));
}
// function to retreive a random number between min and max (inclusive)
int random_int(int min, int max) {
	// random() typically provides a higher quality of randomness than rand()
	// it also requires explicit seeding with srandom()
	// it is crucial to seed with time to ensure a different sequence each time
	//srandom(time(NULL)); // seed the random() number generator only once from main
	return (min + (random() % (max - min + 1)));
}
// function to check if a number exists in an array
bool contains_array(int length, int avoid[(length)], int num) {
	if ((length) <= 0) return false;
	for (int i = 0; i < (length); i++) {
		if ((avoid[i]) == num) {
			return true;
		}
	}
	return false;
}
// function to check if a number exists in a matrix
bool contains_matrix(int rows, int cols, int avoid[(rows)][(cols)], int num) {
	if ((rows) <= 0 || (cols) <= 0) return false;
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			if ((avoid[i][j]) == num) {
				return true;
			}
		}
	}
	return false;
}
// function to retreive a random index from a given number filled array
int random_unique_matrix_v1(int rows, int cols, int matrix[(rows)][(cols)]) {
	if ((rows) <= 0 || (cols) <= 0) return 0;
	int minimum = LENGTH_MIN;
	int maximum = LENGTH_MAX;
	int num = 0;
	do {
		num = random_int(minimum, maximum);
	} while (contains_matrix(rows, cols, matrix, num));
	return (num);
}
// function to retreive a random index from a given number filled array
int random_unique_array_v1(int length, int array[(length)]) {
	if ((length) <= 0) return 0;
	int minimum = LENGTH_MIN;
	int maximum = LENGTH_MAX;
	int num = 0;
	do {
		num = random_int(minimum, maximum);
	} while (contains_array(length, array, num));
	return (num);
}
// function to retreive a random index from a given number filled array (more efficient approach)
// to avoid repeated occurences, the index is removed from the array
int random_unique_array_v2(int *length, int array[(*length)]) {
	if ((*length) <= 0) return 0;
	int minimum = 0, maximum = ((*length) - 1);
	int index = random_int(minimum, maximum);
	int num = array[index];
	size_t start_index = (index);
	size_t end_index = (index);
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
		memmove((&array[start_index]), (&array[end_index + 1]), ((*length) - (end_index + 1) + 1) * sizeof(int));
		// ensure the output buffer is null-terminated (\0)
		array[(*length) - num_to_remove] = '\0';
		(*length)--;
	}
	//printf(" new length %d\n", (*length));
	//for (int i = 0; i < (*length); i++) printf(" %d", (array[i]));
	//printf("\n");
	return (num);
}
// function to fill an array according to a given type of order
void fill_array(int length, int array[(length)], int order) {
	if ((length) <= 0) return;
	//char *notation_mode = " %d";
	int minimum = 1;
	int maximum = (length);
	int num = 0;
	switch (order) {
		case RANDOM1:
		case RANDOM2:
			// initialize the array radomly
			for (int i = 0; i < (length); i++) {
				num = random_int(minimum, maximum);
				array[i] = num;
				//printf(notation_mode, (array[i]));
			}
			//printf("\n");
		break;
		case BYINDEX:
			// initialize the array by index
			for (int i = 0; i < (length); i++) {
				num = (i + 1);
				array[i] = num;
				//printf(notation_mode, (array[i]));
			}
			//printf("\n");
		break;
		default:
			// initialize the array with zeros
			for (int i = 0; i < (length); i++) {
				num = 0;
				array[i] = num;
				//printf(notation_mode, (array[i]));
			}
			//printf("\n");
		break;
	}
}
// function to fill a matrix according to a given type of order
void fill_matrix(int rows, int cols, int matrix[(rows)][(cols)], int order, int repeat) {
	if ((rows) <= 0 || (cols) <= 0) return;
	//char *notation_mode = "%5d";
	int minimum = 1;
	int maximum = ((rows) * (cols));
	int num = 0;
	int length = maximum;
	int array[(length)] = {};
	switch (order) {
		case RANDOM1:
			// initialize the matrix radomly
			for (int i = 0; i < (rows); i++) {
				for (int j = 0; j < (cols); j++) {
					if (repeat == REPEAT_ALLOW) {
						num = random_int(minimum, maximum);
					} else {
						num = random_unique_matrix_v1(rows, cols, matrix);
					}
					matrix[i][j] = num;
					//printf("matrix[%d][%d] %5d\n", i, j, (matrix[i][j]));
					//printf(notation_mode, matrix[i][j]);
				}
				//printf("\n");
			}
		break;
		case RANDOM2:
			// initialize the matrix radomly
			fill_array((length), (array), BYINDEX);
			for (int i = 0; i < (rows); i++) {
				for (int j = 0; j < (cols); j++) {
					if (repeat == REPEAT_ALLOW) {
						num = random_int(minimum, maximum);
					} else {
						num = random_unique_array_v2((&length), (array));
					}
					matrix[i][j] = num;
					//printf("matrix[%d][%d] %5d\n", i, j, (matrix[i][j]));
					//printf(notation_mode, matrix[i][j]);
				}
				//printf("\n");
			}
		break;
		case BYINDEX:
			// initialize the matrix by index
			for (int i = 0; i < (rows); i++) {
				for (int j = 0; j < (cols); j++) {
					num = ((i * cols) + j + 1);
					matrix[i][j] = num;
					//printf(notation_mode, (matrix[i][j]));
				}
				//printf("\n");
			}
		break;
		default:
			// initialize the matrix with zeros
			for (int i = 0; i < (rows); i++) {
				for (int j = 0; j < (cols); j++) {
					num = 0;
					matrix[i][j] = num;
					//printf(notation_mode, (matrix[i][j]));
				}
				//printf("\n");
			}
		break;
	}
}
// function to fill an array with a matrix
void matrix_to_array(int rows, int cols, int matrix[(rows)][(cols)], int array[(rows)*(cols)]) {
	if ((rows) <= 0 || (cols) <= 0) return;
	int index = 0;
	// fill the array with matrix values
	for (int i = 0; i < (rows); i++) {
		for (int j = 0; j < (cols); j++) {
			index = ((i * cols) + j);
			array[index] = matrix[i][j];
		}
	}
}
// function to fill a matrix with an array
void array_to_matrix(int rows, int cols, int matrix[(rows)][(cols)], int array[(rows)*(cols)]) {
	if ((rows) <= 0 || (cols) <= 0) return;
	int index = 0;
	// fill the matrix with array values
	for (int i = 0; i < (rows); i++) {
		for (int j = 0; j < (cols); j++) {
			index = ((i * cols) + j);
			matrix[i][j] = array[index];
		}
	}
}
// function to swap two integers
void swap_int(int *xp, int *yp) {
	int temp = *xp;
	*xp = *yp;
	*yp = temp;
}
// function to perform bubble sort on an array
void sort_bubble(int length, int array[(length)], int order) {
	int i, j;
	bool swapped;
	for (i = 0; i < (length - 1); i++) {
		swapped = false; // flag to check if any swap occurred in this pass
		// last i elements are already in place (already sorted)
		for (j = 0; j < (length - i - 1); j++) {
			if ((order == ASCENDING && array[j + 1] < array[j]) || (array[j] < array[j + 1] && order == DESCENDING)) {
				//int temp = array[j];
				//array[j] = array[j + 1];
				//array[j + 1] = temp;
				// swap function replaces previous lines using pointers
				swap_int(&array[j], &array[j + 1]);
				swapped = true; // set flag to true if a swap occurred
			}
		}
		// if no elements were swapped, then the array is sorted
		if (swapped == false) break;
	}
}
// function to print a monochromatic array
void print_array(int length, int array[(length)]) {
	if ((length) <= 0) return;
	char *notation_mode = " %d";
	for (int i = 0; i < (length); i++) {
		printf(notation_mode, (array[i]));
	}
	printf("\n");
}
// function to print a monochromatic matrix
void print_matrix(int rows, int cols, int matrix[(rows)][(cols)], int order, int n) {
	if ((rows) <= 0 || (cols) <= 0) return;
	char *notation_mode = "%5d";
	switch (order) {
		case RANDOM1:
		case RANDOM2:
			printf("--------------- Matriz %d Sin Orden ---------------\n", n);
			printf("------------- Filas %2d × Columnas %2d -------------\n", (rows), (cols));
		break;
		default:
			printf("--------------- Matriz %d Con Orden ---------------\n", n);
			printf("------------- Filas %2d × Columnas %2d -------------\n", (rows), (cols));
		break;
	}
	for (int i = 0; i < (rows); i++) {
		for (int j = 0; j < (cols); j++) {
			printf(notation_mode, (matrix[i][j]));
		}
		printf("\n");
	}
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
	srand(time(NULL)); // seed the rand() number generator only once from main
	srandom(time(NULL)); // seed the random() number generator only once from main
	//int minimum = LENGTH_MIN;
	//int maximum = LENGTH_MAX;
	//printf("rand   %d\n", rand_int(minimum, maximum));
	//printf("random %d\n", random_int(minimum, maximum));
	int rows = 0, cols = 0;
	int quantity_i = 3, quantity_j = 2;
	int rows_cols[quantity_i][quantity_j] = {};
	fill_matrix(quantity_i, quantity_j, rows_cols, RANDOM1, REPEAT_AVOID);
	for (int i = 0; i < quantity_i; i++) {
		if (rows_cols[i][1] < rows_cols[i][0]) {
			rows = rows_cols[i][0];
			cols = rows_cols[i][1];
		} else {
			rows = rows_cols[i][1];
			cols = rows_cols[i][0];
		}
		//printf("rows %d cols %d\n", rows, cols);
		int matrix[rows][cols] = {};
		fill_matrix(rows, cols, matrix, RANDOM2, REPEAT_AVOID);
		print_matrix(rows, cols, matrix, RANDOM2, (i + 1));
		int array[(rows*cols)] = {};
		matrix_to_array(rows, cols, matrix, array);
		//print_array((rows*cols), array);
		sort_bubble((rows*cols), array, ASCENDING);
		array_to_matrix(rows, cols, matrix, array);
		print_matrix(rows, cols, matrix, BYINDEX, (i + 1));
	}
	// exit () performs standard program termination cleanup
	// return generally does not involve program-wide cleanup
	//exit (EXIT_FAILURE); // terminate immediately, regardless where is called from
	return (EXIT_SUCCESS); // terminate and return control to where is called from
}
