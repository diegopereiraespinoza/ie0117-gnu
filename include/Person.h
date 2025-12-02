#ifndef PERSON_H
#define PERSON_H

#include <stdio.h>            // For scanf, fprintf, printf, perror, getline, ...
#include <stdlib.h>           // For setenv, getenv, rand, random, realpath, system, strtol, malloc, realloc, free, ...
#include <string.h>           // For memmove, strdup, strcpy, strcat, strtok, strtol, strstr, strcmp, strlen, ...
#include "umath.h"            // math utils for random numbers

/*
// global constants defined by macros for Person utils
*/

#ifndef RANDOM_MAX
	#define RANDOM_MAX 177 // maximum height (NOT BEING USED, FOR INFO ONLY)
#endif
#ifndef ARRAY_LENGTH
	#define ARRAY_LENGTH 17 // quantity to initialize the array of persons
#endif
#ifndef RANDOM_MIN
	#define RANDOM_MIN 161 // (RANDOM_MAX - ARRAY_LENGTH + 1) // minimum height
#endif

/*
// global variables defined by struct for Person
*/

// data type for a struct to be sorted
// typedef struct Person Person;
typedef struct Person {
	char *name; // nombre de la persona
	int age; // edad de la persona
	double height; // altura de la persona en centimetros
	//struct Person* next;
} Person;

/*
// function declarations
*/

// return: person
// allocate memory to create the new person
Person* create_person(char *name, int age, double height);
void init_persons(int length, Person **persons);
void free_persons(int length, Person **persons);
void set_persons_static(int length, Person persons[length]);
Person** set_persons_dynamic(int length, Person **persons);

#endif // PERSON_H
