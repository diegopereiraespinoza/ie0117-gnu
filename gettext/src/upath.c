#include "upath.h"            // utils for resolving urls from symlinks

/*
// global variables
*/

/*
// function definitions
*/

// return: path
// get the current working directory
char* current_dir() {
	size_t size = PATH_MAX; // initial buffer size
	char *buffer = (NULL);
	// dynamically allocate memory for the path
	buffer = (char*)malloc((size + 1) * sizeof(char)); // +1 for null terminator (\0)
	if (!(buffer)) { // buffer == NULL
		//perror("No se pudo reservar memoria para 'ruta'\n");
		//printf("No se pudo reservar memoria para 'ruta'\n");
		printf("No memory could be allocated for 'path'\n");
		return (NULL);
	}
	// using a dynamically allocated buffer
	// get the current working directory
	#ifdef WIN32
		buffer = _getcwd(buffer, size);
	#else
		// can let getcwd dynamically allocate memory
		// buffer = getcwd(NULL, 0);
		// but in some systems
		// if buffer is NULL, can lead
		// getcwd to undefined behaviour
		buffer = getcwd(buffer, size);
	#endif
	if (!(buffer)) { // buffer == NULL
		// free the dynamically allocated memory
		free(buffer);
		buffer = (NULL);
		return (NULL);
	}
	return (buffer);
}
