#ifndef UTILS_RESB_H
#define UTILS_RESB_H

#include <stdio.h>            // For scanf, fprintf, printf, perror, getline, ...
#include <stdlib.h>           // For rand, random, realpath, system, strtol, malloc, realloc, free, ...
#include <stdbool.h>          // For bool types
#include "unicode/ustdio.h"
#include "unicode/ustring.h"
#include "unicode/ures.h"
//#include "unicode/putil.h"
//#include "unicode/uloc.h"

/*
// global constants defined by macros
*/

#define URESB_DEFAULTTRUNC 40

/*
// function declarations
*/

UChar* quotedString(const UChar *string);
void printIndent(int32_t indent, UFILE *out);
void printHex(int8_t *what, UFILE *out);
const UChar* getErrorName(char *currdir, char *locale, UErrorCode errorNumber);//, UFILE *outerr);
const UChar* reportError(char *currdir, char *locale, UErrorCode *status, UFILE *outerr);
void printOutBundle(UResourceBundle *resource, char *currdir, char *locale, int32_t indent, UErrorCode *status, UFILE *out, UFILE *outerr);

#endif // UTILS_RESB_H
