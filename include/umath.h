#ifndef UTILS_MATH_H
#define UTILS_MATH_H

#define _GNU_SOURCE           // For some systems, getenv, strndup, random, realpath, PATH_MAX, strsep, asprintf, getline, exp10, ...
#include <stdio.h>            // For scanf, fprintf, printf, perror, getline, ...
#include <stdlib.h>           // For setenv, getenv, rand, random, realpath, system, strtol, malloc, realloc, free, ...
#include <string.h>           // For memmove, strdup, strcpy, strcat, strtok, strtol, strstr, strcmp, strlen, ...

/*
// global constants defined by macros
*/

#ifndef ZEROED
	#define ZEROED 0 // fill array with zeros
#endif
#ifndef BYINDEX
	#define BYINDEX 1 // fill array with index
#endif
#ifndef RANDOM1
	#define RANDOM1 2 // fill array with random numbers
#endif
#ifndef RANDOM2
	#define RANDOM2 3 // fill array with not duplicates (more efficient approach)
#endif
#ifndef BYRANGE
	// between range of (RANDOM_MAX - ARRAY_LENGTH + 1) and (RANDOM_MAX)
	#define BYRANGE 4 // between range of (RANDOM_MIN) and (RANDOM_MIN + ARRAY_LENGTH - 1)
#endif

/*
// function declarations
*/

// function to retreive a random number between min and max (inclusive)
int rand_int(int min, int max);
// function to retreive a random number between min and max (inclusive)
int random_int(int min, int max);
// function to retreive a random index from a given number filled array (more efficient approach)
// to avoid repeated occurences, the index is removed from the array
int random_unique_array_v2(int *length, int array[(*length)]);
// function to fill an array according to a given type of order
void fill_array(int length, int array[(length)], int order, int start);

#endif // UTILS_MATH_H
