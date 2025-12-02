#ifndef UTILS_SORT_H
#define UTILS_SORT_H

#define _GNU_SOURCE           // For some systems, getenv, strndup, strcasecmp, random, realpath, PATH_MAX, strsep, asprintf, getline, exp10, ...
#include <stdio.h>            // For scanf, fprintf, printf, perror, getline, ...
#include <string.h>           // For memmove, strdup, strcpy, strcat, strtok, strtol, strstr, strcmp, strlen, ...
#include "Person.h"           // struct definition for Person

/*
// global constants defined by macros for sort utils
*/

#ifndef DESCENDING
	#define DESCENDING 0 // flag to order an array high to low
#endif
#ifndef ASCENDING
	#define ASCENDING 1 // flag to order an array low to high
#endif

/*
// function declarations
*/

int compare_by_name(const void *left, const void *right);
int compare_by_age(const void *left, const void *right);
int compare_by_height(const void *left, const void *right);

// function to swap two nodes
// only for swap 2 array elements, not for lists
// because prev and next pointers are not updated
// instead of s_quick_swap use the function s_swap_nodes
void quick_swap(void **left, void **right);
// function to partition the array
int quick_partition(void **array, int left, int right, int order, int (*compare_func)(const void *left, const void *right));
// function to implement quick sort
void quick_sort(void **array, int left, int right, int order, int (*compare_func)(const void *left, const void *right));

void print_persons_static(int length, Person persons[length]);
void print_persons_dynamic(int length, Person **persons);
void sort_persons_static(int length);
void sort_persons_dynamic(int length);

#endif // UTILS_SORT_H
