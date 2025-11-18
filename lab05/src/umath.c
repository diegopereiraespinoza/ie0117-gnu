#include "umath.h"   // math utils for random numbers

/*
// global constants defined by macros
*/
/*
#ifndef ZEROED
	#define ZEROED 0 // fill a matrix by index or with zeros
#endif
#ifndef BYINDEX
	#define BYINDEX 1 // fill a matrix by index or with zeros
#endif
#ifndef RANDOM1
	#define RANDOM1 2 // fill a matrix with random numbers
#endif
#ifndef RANDOM2
	#define RANDOM2 3 // fill a matrix with random numbers (more efficient approach)
#endif
*/
/*
// function definitions
*/
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
void fill_array(int length, int array[(length)], int order) {
	if ((length) <= 0) return;
	//char *notation_mode = " %d";
	int minimum = 1;
	int maximum = (length);
	int num = 0;
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
