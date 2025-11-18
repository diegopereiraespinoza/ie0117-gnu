#define _GNU_SOURCE           // Required by some systems for random(), strsep, asprintf, getline, exp10, ...
#include <locale.h>           // For setlocale to use a thousands separator in numbers by printf
//#include <libio.h>            // For fwide
#include <stdio.h>            // For fprintf, printf, scanf, ...
#include <stdlib.h>           // For rand(), random(), system, strtok, strtol, malloc, realloc, free, if dynamic allocation is used
#include <stdbool.h>          // For bool types
#include <string.h>           // For memmove, strlen, though getline returns length
#include <time.h>             // For seeding srand()/srandom() with time
//#include <limits.h>           // For limits of: char / short / int (INT_MIN/INT_MAX) / long / long long (LLONG_MIN/LLONG_MAX)
//#include <float.h>            // For limits of: float (FLT_MIN/FLT_MAX) / double / long double (LDBL_MIN/LDBL_MAX)
//#include <errno.h>            // For errno == ERANGE, clear errno=0 before call strlen, strcpy, strstr, strtok, strtol, ...
//#include <ctype.h>            // For toupper, tolower, isalpha, isdigit, isspace (trim spaces from words)
//#include <iconv.h>            // For decimal ASCII to UTF-8
//#include <uchar.h>            // For uchar_t on estandar UTF-8
//#include <wchar.h>            // For wchar_t and wide character functions

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
#ifndef RANDOM_MAX
	#define RANDOM_MAX 99 // maximum for a random number
#endif
#ifndef S_LIST_LENGTH
	#define S_LIST_LENGTH 16 // items quantity for the linked list
#endif
#ifndef S_DESCENDING
	#define S_DESCENDING 0 // flag to order an array high to low
#endif
#ifndef S_ASCENDING
	#define S_ASCENDING 1 // flag to order an array low to high
#endif
// data type for a singly node
// typedef struct SNode SNode;
typedef struct SNode {
	int index;
	//char locale[5];
	struct SNode* next;
} SNode;
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
// function to retreive a random index from a given number filled array (more efficient approach)
// to avoid repeated occurences, the index is removed from the array
int random_unique_array_v2(int *length, int array[(*length)]) {
	if ((*length) <= 0) return 0;
	int minimum = 0, maximum = ((*length) - 1);
	int index = random_int(minimum, maximum);
	int num = array[index];
	int start_index = (index);
	int end_index = (index);
	// validate indices
	if (start_index < (*length) && end_index < (*length) && start_index <= end_index) {
		// calculate the number of characters to remove
		int num_to_remove = end_index - start_index + 1;
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
// return: node
// allocate memory to create the new node
SNode* s_create(int index) {
	SNode* node = (NULL);
	node = (SNode*)malloc(sizeof(SNode));
	if (!(node)) {
		//perror("No se pudo reservar memoria para el nodo.");
		printf("No se pudo reservar memoria para el nodo.");
		free(node);
		return (NULL);
	}
	(node)->index = index;
	(node)->next = (NULL);
	return (node);
}
// return: head
// insertion at the beginning (head)
SNode* s_insert_at_head(SNode *head, SNode *node) {
	if (!(node)) {
		printf("%s\n", "Nodo es NULL (no se encuentra inicializado).");
		return (NULL);
	}
	//printf("insert_at_head head '%p' node '%p'\n", (head), (node));
	//printf("insert_at_head head '%p' node '%p' <- address\n", (&head), (&node));
	(node)->next = (head);
	(head) = (node);
	return (head);
}
// return: tail
// search for the last (tail)
SNode* s_find_tail(SNode *head) {
	if (!(head)) {
		return (NULL);
	}
	SNode* tail = (head);
	while ((tail)->next != (NULL)) {
		(tail) = (tail)->next;
	}
	return (tail);
}
// return: head
// insertion at the end (tail)
SNode* s_insert_at_tail(SNode *head, SNode *node) {
	if (!(node)) {
		printf("%s\n", "Nodo es NULL (no se encuentra inicializado).");
		return (NULL);
	}
	//printf("insert_at_tail tail '%p' node '%p'\n", (tail), (node));
	//printf("insert_at_tail tail '%p' node '%p' <- address\n", (&tail), (&node));
	SNode* tail = (NULL);
	tail = s_find_tail((head));
	if (!(tail)) {
		(head) = (node);
	} else {
		(tail)->next = (node);
	}
	return (head);
}
// return: node
// search for the previous to ref node
SNode* s_find_prev_to_ref(SNode *head, SNode *node) {
	if (!(head) || !(node) || ((node)->index == (head)->index)) {
		return (NULL);
	}
	SNode* prev = (head);
	while ((prev)->next != (NULL) && ((prev)->next->index != (node)->index)) {
		(prev) = (prev)->next;
	}
	// if reached the end and did not find the ref node
	if (!((prev)->next) && ((prev)->index != (node)->index)) {
		return (NULL);
	}
	return (prev);
}
// return: node
// search for the ref node
SNode* s_find_ref(SNode *head, SNode *node) {
	if (!(head) || !(node)) {
		return (NULL);
	}
	SNode* ref = (head);
	//SNode* prev = (NULL);
	while ((ref) != (NULL) && ((ref)->index != (node)->index)) {
		//(prev) = (ref);
		(ref) = (ref)->next;
	}
	//if (!(ref)) (ref) = (prev);
	return (ref);
}
// return: head
// insertion before the ref
SNode* s_insert_before(SNode *head, SNode *node, SNode *temp) {
	if (!(node)) {
		printf("%s\n", "Nodo es NULL (no se encuentra inicializado).");
		return (NULL);
	}
	//printf("insert_before head '%p' node '%p' ref '%p'\n", (head), (node), (temp));
	//printf("insert_before head '%p' node '%p' ref '%p' <- address\n", (&head), (&node), (&temp));
	SNode* prev = (NULL);
	prev = s_find_prev_to_ref((head), (temp));
	if (!(prev)) {
		(node)->next = (head);
		(head) = (node);
	} else {
		(node)->next = (prev)->next;
		(prev)->next = (node);
	}
	return (head);
}
// return: head
// insertion after the ref
SNode* s_insert_after(SNode *head, SNode *node, SNode *temp) {
	if (!(node)) {
		printf("%s\n", "Nodo es NULL (no se encuentra inicializado).");
		return (NULL);
	}
	//printf("insert_after head '%p' node '%p' ref '%p'\n", (head), (node), (temp));
	//printf("insert_after head '%p' node '%p' ref '%p' <- address\n", (&head), (&node), (&temp));
	SNode* ref = (NULL);
	ref = s_find_ref((head), (temp));
	if (!(ref)) {
		SNode* tail = (NULL);
		tail = s_find_tail((head));
		if (!(tail)) {
			(head) = (node);
		} else {
			(tail)->next = (node);
		}
	} else {
		(node)->next = (ref)->next;
		(ref)->next = (node);
	}
	return (head);
}
// return: node
// search for the greater than new node
SNode* s_find_prev_greater(SNode *head, SNode *node) {
	if (!(head) || !(node) || ((head)->index <= (node)->index)) {
		return (NULL);
	}
	SNode* prev = (head);
	while ((prev)->next != (NULL) && ((node)->index) < (prev)->next->index) {
		(prev) = (prev)->next;
	}
	return (prev);
}
// return: node
// search for the smaller than new node
SNode* s_find_prev_smaller(SNode *head, SNode *node) {
	if (!(head) || !(node) || ((node)->index <= (head)->index)) {
		return (NULL);
	}
	SNode* prev = (head);
	while ((prev)->next != (NULL) && ((prev)->next->index < (node)->index)) {
		(prev) = (prev)->next;
	}
	return (prev);
}
// return: head
// insertion into a sorted linked list, according to order (ASC/DESC)
SNode* s_insert_sorted(SNode *head, SNode *node, int order) {
	if (!(node)) {
		printf("%s\n", "Nodo es NULL (no se encuentra inicializado).");
		return (NULL);
	}
	//printf("insert_sorted head '%p' node '%p'\n", (head), (node));
	//printf("insert_sorted head '%p' node '%p' <- address\n", (&head), (&node));
	SNode* ref = (NULL);
	if (order == S_ASCENDING) {
		ref = s_find_prev_smaller((head), (node));
	} else {
		ref = s_find_prev_greater((head), (node));
	}
	if (!(ref)) {
		(node)->next = (head);
		(head) = (node);
	} else {
		(node)->next = (ref)->next;
		(ref)->next = (node);
	}
	return (head);
}
// return: head
// delete a node from a singly linked list
SNode* s_delete(SNode *head, SNode *node) {
	if (!(head) || !(node)) {
		return (NULL);
	}
	SNode* ref = (NULL);
	ref = s_find_ref((head), (node));
	if (!(ref)) {
		return (head);
	}
	SNode* prev = (NULL);
	prev = s_find_prev_to_ref((head), (ref));
	if (!(prev)) {
		// if head node itself holds the value to be deleted
		SNode* temp = (head);
		(head) = (ref)->next;
		free(temp); // free memory
		(temp) = (NULL);
	} else {
		// unlink the node from the linked list
		(prev)->next = (ref)->next;
		free(ref); // free allocated memory
		(ref) = (NULL);
	}
	return (head);
}
// return: head
// function to swap two nodes
SNode* s_swap_nodes(SNode *head, SNode **left, SNode **right) {
	SNode* prev = (NULL);
	prev = s_find_prev_to_ref((head), (*left));
	SNode* tmp = (*right);
	(*right) = (*right)->next;
	(tmp)->next = (*left);
	if (!(prev)) {
		(head) = (tmp);
	} else {
		(prev)->next = (tmp);
	}
	return (head);
}
// return: head
// bubble sort for linked lists
SNode* s_bubble_sort(SNode* head, int order) {
	if (!(head)) {
		return (NULL);
	}
	bool swapped;
	SNode* idx = (NULL);
	SNode* len = (NULL);
	do {
		swapped = false; // flag to check if any swap occurred in this pass
		(idx) = (head);
		while ((idx)->next != (len)) {
			SNode* right = (idx)->next;
			if ((order == S_ASCENDING && (idx)->next->index < (idx)->index) || ((idx)->index < (idx)->next->index && order == S_DESCENDING)) {
				/*
				// next 3 lines are not correct because swap only index, no nodes
				int temp = (idx)->index;
				(idx)->index = (idx)->next->index;
				(idx)->next->index = temp;*/
				/*
				// next lines are more secure to swap pointers to whole nodes
				SNode* prev = (NULL);
				prev = s_find_prev_to_ref((head), (idx));
				(idx)->next = (idx)->next->next;
				(right)->next = (idx);
				if (!(prev)) {
					(head) = (right);
				} else {
					(prev)->next = (right);
				}*/
				// swap function replaces previous lines using pointers
				(head) = s_swap_nodes((head), (&idx), (&(idx)->next));
				swapped = true; // set flag to true if a swap occurred
			}
			//(idx) = (idx)->next;
			(idx) = (right);
		}
		(len) = (idx);
		// if no elements were swapped, then the array is sorted
		if (swapped == false) break;
	} while (swapped);
	return (head);
}
// return: head
// merge sort for linked lists (more efficient approach than bubble sort)
SNode* s_merge(SNode *left, SNode *right, int order) {
	SNode* head = (NULL);
	if (!(left)) return (right);
	if (!(right)) return (left);
	// case for S_ASCENDING / S_DESCENDING order
	if ((order == S_ASCENDING && (right)->index < (left)->index) || ((left)->index < (right)->index && order == S_DESCENDING)) {
		(head) = (right);
		(head)->next = s_merge((left), (right)->next, order);
	} else { // other than ASC/DESC will stay as is
		(head) = (left);
		(head)->next = s_merge((left)->next, (right), order);
	}
	return (head);
}
// return: head
SNode* s_split(SNode *head, SNode **left, SNode **right) {
	SNode* idx = (head);
	SNode* len = (head)->next;
	while ((len) != NULL) {
		(len) = (len)->next;
		if ((len) != NULL) {
			(idx) = (idx)->next;
			(len) = (len)->next;
		}
	}
	(*left) = (head);
	(*right) = (idx)->next;
	(idx)->next = (NULL);
	return (head);
}
// return: head
// be carefull because the head is modified by reference
SNode* s_merge_sort(SNode **head, int order) {
	if (!(*head) || (!(*head)->next)) {
		return (NULL);
	}
	SNode* left = (NULL);
	SNode* right = (NULL);
	s_split((*head), (&left), (&right));
	s_merge_sort((&left), order);
	s_merge_sort((&right), order);
	(*head) = s_merge((left), (right), order);
	return (*head);
}
// return: head
// add a node to the end of list
SNode* s_push(SNode *head, SNode *node) {
	return (s_insert_at_tail(head, node));
}
// return: tail
// get the last node on the list
SNode* s_peek(SNode *head) {
	return (s_find_tail(head));
}
// return: tail
// get the last node on the list
SNode* s_top(SNode *head) {
	return (s_find_tail(head));
}
// return: head
// delete the last node from the list
SNode* s_pop(SNode *head) {
	SNode* tail = (NULL);
	tail = s_find_tail((head));
	return (s_delete(head, tail));
}
// return true if the list is empty
bool s_isempty(SNode *head) {
	if (!(head)) {
		return (true);
	}
	return (false);
}
// function to print the linked list
void s_print_list(SNode *node) {
	while ((node) != (NULL)) {
		printf("%3d", (node)->index);
		(node) = (node)->next;
	}
	printf("\n");
}
// free allocated memory (important to prevent memory leaks)
void s_free_list(SNode *head) {
	SNode* temp = (NULL);
	while ((head) != (NULL)) {
		(temp) = (head);
		(head) = (head)->next;
		free(temp); // free allocated memory
		(temp) = (NULL);
	}
}
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
