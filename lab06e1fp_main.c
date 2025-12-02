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

#include "umath.h"            // math utils for random numbers
#include "Person.h"           // struct definition for Person
#include "usort.h"            // sort utils for struct Person

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
