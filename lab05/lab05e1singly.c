#define _GNU_SOURCE           // Required by some systems for random(), strsep, asprintf, getline, exp10, ...
#include <locale.h>           // For setlocale to use a thousands separator in numbers by printf
//#include <libio.h>            // For fwide
//#include <stdio.h>            // For fprintf, printf, scanf, ...
//#include <stdlib.h>           // For rand(), random(), system, strtok, strtol, malloc, realloc, free, if dynamic allocation is used
//#include <stdbool.h>          // For bool types
//#include <string.h>           // For memmove, strlen, though getline returns length
#include <time.h>             // For seeding srand()/srandom() with time
//#include <limits.h>           // For limits of: char / short / int (INT_MIN/INT_MAX) / long / long long (LLONG_MIN/LLONG_MAX)
//#include <float.h>            // For limits of: float (FLT_MIN/FLT_MAX) / double / long double (LDBL_MIN/LDBL_MAX)
//#include <errno.h>            // For errno == ERANGE, clear errno=0 before call strlen, strcpy, strstr, strtok, strtol, ...
//#include <ctype.h>            // For toupper, tolower, isalpha, isdigit, isspace (trim spaces from words)
//#include <iconv.h>            // For decimal ASCII to UTF-8
//#include <uchar.h>            // For uchar_t on estandar UTF-8
//#include <wchar.h>            // For wchar_t and wide character functions

#include "umath.h"            // math utils for random numbers
#include "sstack.h"           // stack with a singly list

// script to work with singly lists
// all functions return the head
// except those to find the tail
// s_find_tail | s_peek | s_top
// or those to find a specific node
// s_find_prev_to_ref | s_find_ref
// s_find_prev_greater | s_find_prev_smaller
/*
// global constants defined by macros
*/
#ifndef RANDOM_MAX
	#define RANDOM_MAX 99 // maximum for a random number
#endif
#ifndef S_LIST_LENGTH
	#define S_LIST_LENGTH 16 // items quantity for the linked list
#endif
/*
// program entry main function
*/
int main() {//int argc, char *argv[]) {
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
	//printf("\x1b[2J\x1b[1;1H"); // clear the terminal WORKING
	//system("clear"); // clear the terminal WORKING
	srand(time(NULL)); // seed the rand() number generator only once from main
	srandom(time(NULL)); // seed the random() number generator only once from main
	int rand_max = RANDOM_MAX;
	int array[(rand_max)];
	//SNode node;
	//node.index = 0;
	//strcpy(node.locale, "en_US");
	SNode* head = (NULL);
	SNode* node = (NULL);
	SNode* ref = (NULL);
	/*
	node = s_create(33);
	head = s_insert_at_head(head, node);
	s_print_list(head);
	node = s_create(1);
	ref = s_create(33);
	head = s_insert_before(head, node, ref);
	s_print_list(head);
	node = s_create(66);
	head = s_insert_at_tail(head, node);
	s_print_list(head);
	node = s_create(99);
	ref = s_create(66);
	head = s_insert_after(head, node, ref);
	s_print_list(head);
	ref = s_create(1);
	head = s_delete(head, ref);
	s_print_list(head);
	s_free_list(head); // free allocated memory
	head = (NULL);
	*/
	printf("%s\n", "-------------- Lista enlazada SIMPLE --------------");
	// check if the singly linked list is empty
	printf("La lista se encuentra vacía? %s\n", (s_isempty(head) ? "Sí" : "No"));
	// create a RANDOM singly linked list
	rand_max = RANDOM_MAX;
	fill_array((rand_max), (array), BYINDEX);
	printf("%s\n", "---------------- Orden : ALEATORIO ----------------");
	for (int i = 0; i < (S_LIST_LENGTH); i++) {
		int index = random_unique_array_v2((&rand_max), (array));
		node = s_create(index);
		//head = s_insert_sorted(head, node, S_DESCENDING);
		//head = s_insert_sorted(head, node, S_ASCENDING);
		head = s_insert_at_tail(head, node);
		//s_print_list(head);
	}
	s_print_list(head);
	// sort S_DESCENDING singly linked list
	printf("%s\n", "--------------- Orden : DESCENDENTE ---------------");
	//head = s_bubble_sort(head, S_DESCENDING); // only ASC/DESC order, other will stay as is
	// merge sort for linked lists (more efficient approach than bubble sort)
	head = s_merge_sort(&head, S_DESCENDING); // only ASC/DESC order, other will stay as is
	s_print_list(head);
	// sort S_ASCENDING singly linked list
	printf("%s\n", "--------------- Orden :  ASCENDENTE ---------------");
	head = s_bubble_sort(head, S_ASCENDING); // only ASC/DESC order, other will stay as is
	// merge sort for linked lists (more efficient approach than bubble sort)
	//head = s_merge_sort(&head, S_ASCENDING); // only ASC/DESC order, other will stay as is
	s_print_list(head);
	printf("%s\n", "-------------- Lista enlazada SIMPLE --------------");
	// check if the singly linked list is empty
	printf("La lista se encuentra vacía? %s\n", (s_isempty(head) ? "Sí" : "No"));
	printf("%s\n", "------------ Implementaciones de STACK ------------");
	printf("%s", "Push:");
	node = s_create(RANDOM_MAX);
	printf("%3d\n", (!(node) ? 0 : ((node)->index)));
	head = s_push(head, node);
	s_print_list(head);
	printf("%s", "Peek:");
	ref = s_peek(head);
	printf("%3d\n", (!(ref) ? 0 : ((ref)->index)));
	printf("%s", "Top :");
	ref = s_top(head);
	printf("%3d\n", (!(ref) ? 0 : ((ref)->index)));
	printf("%s", "Pop :");
	printf("%3d\n", (!(node) ? 0 : ((node)->index)));
	head = s_pop(head);
	s_print_list(head);
	s_free_list(head); // free allocated memory
	head = (NULL);
	return (0);
}
