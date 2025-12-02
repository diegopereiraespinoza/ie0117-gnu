#include "Person.h"           // struct definition for Person

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
// function definitions
*/

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
