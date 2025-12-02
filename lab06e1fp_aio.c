#define _GNU_SOURCE           // For some systems, getenv, strndup, strcasecmp, random, realpath, PATH_MAX, strsep, asprintf, getline, exp10, ...
#include <locale.h>           // For setlocale, printf("%'d", number) with thousands separator
#include <stdio.h>            // For scanf, fprintf, printf, perror, getline, ...
#include <stdlib.h>           // For setenv, getenv, rand, random, realpath, system, strtol, malloc, realloc, free, ...
#include <string.h>           // For memmove, strdup, strcpy, strcat, strtok, strtol, strstr, strcmp, strlen, ...
#include <stdbool.h>          // For bool types
//#include <stdint.h>           // For int32_t
#include <time.h>             // For time to seed srand/srandom
//#include <limits.h>           // For char, short, int (INT_MIN/INT_MAX), long | long long (LLONG_MIN/LLONG_MAX), PATH_MAX, ...
//#include <float.h>            // For float (FLT_MIN/FLT_MAX), double | long double (LDBL_MIN/LDBL_MAX)
//#include <errno.h>            // For errno == ERANGE, clear errno=0 before call strdup, strcpy, strcat, strtok, strtol, strstr, ...
//#include <ctype.h>            // For toupper, tolower, isalpha, isdigit, isspace (trim spaces from words)
//#include <iconv.h>            // For decimal ASCII to UTF-8
//#include <uchar.h>            // For uchar_t on estandar UTF-8
//#include <wchar.h>            // For wchar_t on wide character
//#include <libio.h>            // For fwide
//#include <libgen.h>           // For dirname, basename, ...
//#ifdef WIN32
//#include <direct.h>           // For _getcwd, ...
//#else // WIN32
//#include <unistd.h>           // For getcwd, getpid, readlink, ...
//#endif // WIN32
//#include <libintl.h>          // For i18n/i10n resources in gettext

/*
// global constants defined by macros for math utils
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
// global constants defined by macros for Person utils
*/

#ifndef RANDOM_MAX
	#define RANDOM_MAX 177 // maximum for a random height
#endif
#ifndef ARRAY_LENGTH
	#define ARRAY_LENGTH 17 // quantity to initialize the array of persons
#endif
#ifndef RANDOM_MIN
	#define RANDOM_MIN (RANDOM_MAX - ARRAY_LENGTH + 1) // minimum height
#endif

/*
// global variables to fill an array struct of Person
*/

char *names[ARRAY_LENGTH] = {
	"Emily Maryan Flores Rojas",
	"Andrés Zumbado Moreira",
	"Jonatan Hidalgo Morales",
	"Sebastián Alejandro Araya Fuks",
	"Livia Valentina Corrales Madrigal",
	"José Mario Monge Guerrero",
	"Adrián Méndez Chavarría",
	"George Brian Morison Pallais",
	"Javier Fernando Bolaños Castellón",
	"Melany Dayana Rosales Montiel",
	"José Manuel Solís Quesada",
	"Randall Alonso Méndez Blanco",
	"Felipe Alberto Mata Mata",
	"Pablo Andrés Bermudez Duarte",
	"allan Castro Acosta",
	"Randy Arturo Barrantes Arroyo",
	"Angelica Isabel Aguilar Jiménez"
};
int ages[ARRAY_LENGTH] = {22, 19, 19, 20, 22, 29, 24, 19, 19, 23, 22, 20, 21, 22, 21, 22, 21};

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
// global constants defined by macros for sort utils
*/

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
void fill_array(int length, int array[(length)], int order, int start) {
	if ((length) <= 0) return;
	int minimum = start;
	int maximum = (length);
	int num = 0;
	//printf("Array:");
	//char *notation_mode = "%4d";
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
		case BYRANGE:
			// initialize the array by index
			for (int i = 0; i < (length); i++) {
				//num = (RANDOM_MAX - length + i + 1);
				num = (start + i);
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

// return: person
// allocate memory to create the new person
Person* create_person(char *name, int age, double height) {
	Person *person = (NULL);
	person = (Person*)malloc(sizeof(Person));
	if (!(person)) { // person == NULL
		//perror("No memory could be allocated for 'Person'\n");
		printf("No se pudo reservar memoria para 'Person'\n");
		free(person);
		person = (NULL);
		return (NULL);
	}
	(person)->name = (name); // nombre de la persona
	(person)->age = (age); // edad de la persona
	(person)->height = (height); // altura de la persona en centimetros
	return (person);
}

void init_persons(int length, Person **persons) {
	if (!(persons)) return;
	for (int i = 0; i < length; i++) {
		persons[i] = (NULL); // initialize each person to NULL before any operation
	}
}

void free_persons(int length, Person **persons) {
	if (!(persons)) return;
	for (int i = 0; i < length; i++) {
		if(persons[i] != NULL) {
			free(persons[i]); // free the memory pointed to by each person
		}
		persons[i] = (NULL); // set each person to NULL for safety after free
	}
	// free the array of Person pointers itself
	// only when the array is initialized as double pointer
	// Person **persons = (Person**)malloc(length * sizeof(Person*));
	// free and NULL the array (persons)
	free(persons); // free the main array pointer
	persons = (NULL); // set to NULL after free
}

void set_persons_static(int length, Person persons[length]) {
	// length has to be duplicated because it is modified 
	// by the function random_unique_array_v2 (&len, arr)
	int len = length;
	int arr[len];
	fill_array(len, arr, BYRANGE, RANDOM_MIN);
	for (int i = 0; i < length; i++) {
		// strcpy does not need to be free
		// strcpy does not perform any memory allocation
		// but only works for known size buffers in struct
		// like char name[50];
		//strcpy(persons[i].name, names[i]);
		// can assign directly for pointers in struct
		// like char *name;
		persons[i].name = names[i];
		persons[i].age = ages[i];
		persons[i].height = random_unique_array_v2(&len, arr);
	}
}

Person** set_persons_dynamic(int length, Person **persons) {
	free_persons(length, persons);
	persons = (Person**)malloc(length * sizeof(Person*));
	if (!(persons)) { // base_dynamic == NULL
		//perror("No memory could be allocated for 'Person'\n");
		printf("No se pudo reservar memoria para 'Person'\n");
		free(persons);
		persons = (NULL);
		return (NULL);
	}
	init_persons(length, persons);
	// length has to be duplicated because it is modified 
	// by the function random_unique_array_v2 (&len, arr)
	int len = length;
	int arr[len];
	fill_array(len, arr, BYRANGE, RANDOM_MIN);
	for (int i = 0; i < length; i++) {
		while(!(persons[i] = create_person(names[i], ages[i], random_unique_array_v2(&len, arr))));
	}
	return (persons);
}

int compare_by_name(const void *left, const void *right) {
	// cast void pointers to Person* (pointers to struct)
	const Person *personL = (const Person*)left;
	const Person *personR = (const Person*)right;
	// result -1 for Left < Right, +1 for Left > Right, or 0 Left = Right
	// use strcmp to compare the strings (case-sensitive)
	//int result = strcmp(personL->name, personR->name);
	// use strcasecmp to compare strings (case-insensitive)
	int result = strcasecmp(personL->name, personR->name);
	return (result);
}

int compare_by_age(const void *left, const void *right) {
	// cast void pointers to Person* (pointers to struct)
	const Person *personL = (const Person*)left;
	const Person *personR = (const Person*)right;
	if (personL->age < personR->age) {
		return (-1);
	} else if (personL->age > personR->age) {
		return (1);
	} else {
		return (0); // are equal
	}
}

int compare_by_height(const void *left, const void *right) {
	// cast void pointers to Person* (pointers to struct)
	const Person *personL = (const Person*)left;
	const Person *personR = (const Person*)right;
	if (personL->height < personR->height) {
		return (-1);
	} else if (personL->height > personR->height) {
		return (1);
	} else {
		return (0); // are equal
	}
}

// function to swap two nodes
// only for swap 2 array elements, not for lists
// because prev and next pointers are not updated
// instead of s_quick_swap use the function s_swap_nodes
void quick_swap(void **left, void **right) {
	// cast void pointers to Person* (pointers to struct)
	Person **personL = (Person**)left;
	Person **personR = (Person**)right;
    Person *temp = (*personL);
    (*personL) = (*personR);
    (*personR) = temp;
}

// function to partition the array
int quick_partition(void **array, int left, int right, int order, int (*compare_func)(const void *left, const void *right)) {
	//Person **arr = (Person**)array;
    //int pivot = array[right]; // right is last node as pivot
    void *pivot = array[right]; // right is last node as pivot
    int prev = (left - 1); // previous to left as first node
    for (int j = left; j < right; j++) {
        // if current is smaller than pivot
        // if left is smaller than right
        if ((order == ASCENDING && compare_func(array[j], pivot) < 0) || (0 < compare_func(array[j], pivot) && order == DESCENDING)) {
            prev++; // previous is incremented to next position
            quick_swap(&array[prev], &array[j]);
        }
    }
    quick_swap(&array[prev + 1], &array[right]);
    return (prev + 1);
}

// function to implement quick sort
void quick_sort(void **array, int left, int right, int order, int (*compare_func)(const void *left, const void *right)) {
	if (!(array)) return;
	//Person **arr = (Person**)array;
    if (left < right) {
        // current is the partition index incremented to the right
        int curr = quick_partition(array, left, right, order, compare_func);
        quick_sort(array, left, curr - 1, order, compare_func); // sort before partition
        quick_sort(array, curr + 1, right, order, compare_func); // sort after partition
    }
}

void print_persons_static(int length, Person persons[length]) {
	if (!(persons)) return;
	printf("%s\n", "N# Edad Altura Nombre");
	for (int i = 0; i < length; i++) {
		printf("%2d %4d %6.lf %s\n", (i+1), (persons[i]).age, (persons[i]).height, (persons[i]).name);
	}
}

void print_persons_dynamic(int length, Person **persons) {
	if (!(persons)) return;
	printf("%s\n", "N# Edad Altura Nombre");
	for (int i = 0; i < length; i++) {
		printf("%2d %4d %6.lf %s\n", (i+1), (persons[i])->age, (persons[i])->height, (persons[i])->name);
	}
}

void sort_persons_static(int length) {
	if (length <= 0) return;
	// length has to be duplicated because it is modified 
	// by the function random_unique_array_v2 (&len, arr)
	int len = length;
	int arr[len];
	fill_array(len, arr, BYRANGE, RANDOM_MIN);
	Person persons[length];
	set_persons_static(length, persons);
	int size = sizeof(persons[0]);
	length = (sizeof(persons) / size);
	/*
	printf("%s\n", "-------------- PERSONAS SIN UN ORDEN --------------");
	printf("%s %d\n", "N# Edad Altura Nombre", length);
	for (int i = 0; i < length; i++) {
		printf("%2d %4d %6.lf %s\n", (i+1), (persons[i]).age, (persons[i]).height, (persons[i]).name);
	}*/
	printf("%s\n", "###################################################");
	printf("%s\n", "******** QSORT SÓLO ORDENA STRUCT ESTÁTICO ********");
	printf("%s\n", "###################################################");
	// sort the array of persons using qsort
	// base: pointer to the first element of the array
	// length: number of elements in the array (persons)
	// size: size of each Person sizeof(Person*)
	// compar: comparison function pointer
	printf("%s\n", "---------------- NOMBRE ASCENDENTE ----------------");
	qsort(persons, length, size, compare_by_name);
	print_persons_static(length, persons);
	printf("%s\n", "----------------- EDAD ASCENDENTE -----------------");
	qsort(persons, length, size, compare_by_age);
	print_persons_static(length, persons);
	printf("%s\n", "---------------- ALTURA ASCENDENTE ----------------");
	qsort(persons, length, size, compare_by_height);
	print_persons_static(length, persons);
}

void sort_persons_dynamic(int length) {
	if (length <= 0) return;
	printf("%s\n", "###################################################");
	printf("%s\n", "********* QUICK SORT PARA STRUCT DINÁMICO *********");
	printf("%s\n", "###################################################");
	//Person *persons[length];
	Person **persons = (NULL);
	persons = set_persons_dynamic(length, persons);
	int (*compare_func)(const void *left, const void *right);
	printf("%s\n", "---------------- NOMBRE ASCENDENTE ----------------");
	compare_func = compare_by_name;
	//quick_sort((void**)persons, 0, length - 1, DESCENDING, compare_func);
	quick_sort((void**)persons, 0, length - 1, ASCENDING, compare_func);
	print_persons_dynamic(length, persons);
	printf("%s\n", "----------------- EDAD ASCENDENTE -----------------");
	compare_func = compare_by_age;
	//quick_sort((void**)persons, 0, length - 1, DESCENDING, compare_func);
	quick_sort((void**)persons, 0, length - 1, ASCENDING, compare_func);
	print_persons_dynamic(length, persons);
	printf("%s\n", "---------------- ALTURA ASCENDENTE ----------------");
	compare_func = compare_by_height;
	//quick_sort((void**)persons, 0, length - 1, DESCENDING, compare_func);
	quick_sort((void**)persons, 0, length - 1, ASCENDING, compare_func);
	print_persons_dynamic(length, persons);
	// free the array elements (persons)
	free_persons(length, persons);
	// free and NULL the array (persons)
	//free(persons); // free the main array pointer
	persons = (NULL); // set to NULL after free
}

/*
// program entry main function
*/

int main() {//int argc, char *argv[]) {
	//printf("\x1b[2J\x1b[1;1H"); // clear the terminal WORKING
	//system("clear"); // clear the terminal WORKING
	srand(time(NULL)); // seed the rand() number generator only once from main
	srandom(time(NULL)); // seed the random() number generator only once from main
	
	int length = ARRAY_LENGTH;
	sort_persons_static(length); // initializing a struct array statically
	sort_persons_dynamic(length); // initializing a struct array dynamically
	
	// exit () performs standard program termination cleanup
	// return () generally does not involve program-wide cleanup
	//exit (EXIT_FAILURE); // terminate immediately, regardless where is called from
	return (EXIT_SUCCESS); // terminate and return control to where is called from
}
