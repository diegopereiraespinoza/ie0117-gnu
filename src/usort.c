#include "usort.h"            // sort utils for struct Person

/*
// global variables
*/

/*
// function definitions
*/

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
