#ifndef UTILS_MATH_H
#define UTILS_MATH_H

#define _GNU_SOURCE  // Required by some systems for random()/srandom(), strsep, asprintf, getline, exp10, ...
#include <stdlib.h>  // For rand(), random(), system, strtok, strtol, malloc, realloc, free, if dynamic allocation is used
#include <string.h>  // For memmove, strlen, though getline returns length

/*
// global constants defined by macros
*/
#define ZEROED 0 // fill a matrix by index or with zeros
#define BYINDEX 1 // fill a matrix by index or with zeros
#define RANDOM1 2 // fill a matrix with random numbers
#define RANDOM2 3 // fill a matrix with random numbers (more efficient approach)
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
void fill_array(int length, int array[(length)], int order);

#endif // UTILS_MATH_H
