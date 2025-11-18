#ifndef S_LIST_H
#define S_LIST_H

#include "SNode.h"   // singly node definition
#include <stdio.h>   // For fprintf, printf, scanf, ...
#include <stdlib.h>  // For rand(), random(), system, strtok, strtol, malloc, realloc, free, if dynamic allocation is used
#include <stdbool.h> // For bool types

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
#define S_DESCENDING 0 // flag to order an array high to low
#define S_ASCENDING 1 // flag to order an array low to high
/*
// function declarations
*/
// return: node
// allocate memory to create the new node
SNode* s_create(int index);
// return: head
// insertion at the beginning (head)
SNode* s_insert_at_head(SNode *head, SNode *node);
// return: tail
// search for the last (tail)
SNode* s_find_tail(SNode *head);
// return: head
// insertion at the end (tail)
SNode* s_insert_at_tail(SNode *head, SNode *node);
// return: node
// search for the previous to ref node
SNode* s_find_prev_to_ref(SNode *head, SNode *node);
// return: node
// search for the ref node
SNode* s_find_ref(SNode *head, SNode *node);
// return: head
// insertion before the ref
SNode* s_insert_before(SNode *head, SNode *node, SNode *temp);
// return: head
// insertion after the ref
SNode* s_insert_after(SNode *head, SNode *node, SNode *temp);
// return: node
// search for the greater than new node
SNode* s_find_prev_greater(SNode *head, SNode *node);
// return: node
// search for the smaller than new node
SNode* s_find_prev_smaller(SNode *head, SNode *node);
// return: head
// insertion into a sorted linked list, according to order (ASC/DESC)
SNode* s_insert_sorted(SNode *head, SNode *node, int order);
// return: head
// delete a node from a singly linked list
SNode* s_delete(SNode *head, SNode *node);
// return: head
// function to swap two nodes
SNode* s_swap_nodes(SNode *head, SNode **left, SNode **right);
// return: head
// bubble sort for linked lists
SNode* s_bubble_sort(SNode* head, int order);
// return: head
// merge sort for linked lists (more efficient approach than bubble sort)
SNode* s_merge(SNode *left, SNode *right, int order);
// return: head
SNode* s_split(SNode *head, SNode **left, SNode **right);
// return: head
// be carefull because the head is modified by reference
SNode* s_merge_sort(SNode **head, int order);
// return: void
// function to print the linked list
void s_print_list(SNode *node);
// return: void
// free allocated memory (important to prevent memory leaks)
void s_free_list(SNode *head);

#endif // S_LIST_H
