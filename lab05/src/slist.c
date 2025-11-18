#include "slist.h"   // singly list utils

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
/*
#ifndef S_DESCENDING
	#define S_DESCENDING 0 // flag to order an array high to low
#endif
#ifndef S_ASCENDING
	#define S_ASCENDING 1 // flag to order an array low to high
#endif
*/
/*
// function definitions
*/
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
// return: void
// function to print the linked list
void s_print_list(SNode *node) {
	while ((node) != (NULL)) {
		printf("%3d", (node)->index);
		(node) = (node)->next;
	}
	printf("\n");
}
// return: void
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
