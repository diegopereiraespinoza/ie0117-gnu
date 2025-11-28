#ifndef UTILS_PATH_H
#define UTILS_PATH_H

#define _GNU_SOURCE           // For some systems, random, realpath, PATH_MAX, strsep, asprintf, getline, exp10, ...
#include <stdio.h>            // For scanf, fprintf, printf, perror, getline, ...
#include <stdlib.h>           // For setenv, getenv, rand, random, realpath, system, strtol, malloc, realloc, free, ...
#include <limits.h>           // For char, short, int (INT_MIN/INT_MAX), long | long long (LLONG_MIN/LLONG_MAX), PATH_MAX, ...
#ifdef WIN32
#include <direct.h>           // For _getcwd, ...
#else // WIN32
#include <unistd.h>           // For getcwd, getpid, readlink, ...
#endif // WIN32

/*
// global constants defined by macros
*/

// realpath and PATH_MAX requires _GNU_SOURCE
#ifndef PATH_MAX
	// _POSIX_PATH_MAX = 256 // maximum in POSIX
	// _XOPEN_PATH_MAX = 1024 // maximum in XSI
	#define PATH_MAX 1024 // maximum storable as a pathname in buffer
#endif // PATH_MAX

/*
// function declarations
*/

// return: path
// get the current working directory
char* current_dir();

#endif // UTILS_PATH_H
