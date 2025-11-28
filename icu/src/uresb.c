#include "uresb.h"            // utils for i18n/i10n resources in icu

/*
// global variables
*/

bool VERBOSE = false;
bool truncate = false;
int32_t truncsize = URESB_DEFAULTTRUNC;
int32_t indentsize = 4;
UChar baderror[] = { 0x0042, 0x0041, 0x0044, 0x0000 }; // baderror = "Error \"%s\"";

/*
// function definitions
*/

UChar* quotedString(const UChar *string) {
	int len = u_strlen(string);
	int alen = len;
	const UChar *sp;
	UChar *newstr, *np;
	for (sp = string; *sp; ++sp) {
		switch (*sp) {
			case '\n':
			case 0x0022:
				++alen;
			break;
		}
	}
	newstr = (UChar*)malloc((alen + 1) * sizeof(UChar)); // +1 for null terminator (\0)
	for (sp = string, np = newstr; *sp; ++sp) {
		switch (*sp) {
			case '\n':
				*np++ = 0x005C;
				*np++ = 0x006E;
			break;
			case 0x0022:
				*np++ = 0x005C;
				*np++ = *sp;
			break;
			default:
				*np++ = *sp;
			break;
		}
	}
	*np = 0;
	return newstr;
}

void printIndent(int32_t indent, UFILE *out) {
	char inchar[256];
	int32_t i = 0;
	for(i = 0; i<indent; i++) {
		inchar[i] = ' ';
	}
	inchar[indent] = '\0';
	u_fprintf(out, "%s", inchar);
}

void printHex(int8_t *what, UFILE *out) {
	u_fprintf(out, "%02X", (uint8_t)*what);
}

const UChar* getErrorName(char *currdir, char *locale, UErrorCode errorNumber) {//, UFILE *outerr) {
	UErrorCode status = U_ZERO_ERROR;
	int32_t len = 0;
	UResourceBundle *error = ures_open(currdir, locale, &status);
	UResourceBundle *errorcodes = ures_getByKey(error, "errorcodes", NULL, &status);
	const UChar *result = ures_getStringByIndex(errorcodes, errorNumber, &len, &status);
	ures_close(errorcodes);
	ures_close(error);
	const UChar *errorname = (NULL);
	if(U_SUCCESS(status)) {
		errorname = (result);
		//return (result);
	} else {
		//UChar baderror[] = { 0x0042, 0x0041, 0x0044, 0x0000 }; // baderror = 'Error "%s"';
		errorname = (baderror);
		//return baderror;
	}
	// #include "unicode/ures.h" includes
	// #include "unicode/utypes.h" which has the function u_errorName(status)
	//u_fprintf(outerr, "Error %d(%s) : %S happened!\n", errorNumber, u_errorName(errorNumber), errorname);
	return (errorname);
}

const UChar* reportError(char *currdir, char *locale, UErrorCode *status, UFILE *outerr) {
	//u_fprintf(outerr, "Error %d(%s) : %U happened!\n", *status, u_errorName(*status), getErrorName(currdir, locale, *status));
	const UChar *errorname = (NULL);
	errorname = getErrorName(currdir, locale, *status);//, outerr);
	// #include "unicode/ures.h" includes
	// #include "unicode/utypes.h" which has the function u_errorName(status)
	u_fprintf(outerr, "Error %d(%s) : %S happened!\n", *status, u_errorName(*status), errorname);
	return (errorname);
}

void printOutBundle(UResourceBundle *resource, char *currdir, char *locale, int32_t indent, UErrorCode *status, UFILE *out, UFILE *outerr) {
	int32_t i = 0;
	const char *key = ures_getKey(resource);
	switch(ures_getType(resource)) {
		case URES_STRING :
		{
			int32_t len=0;
			const UChar *thestr = ures_getString(resource, &len, status);
			UChar *string = quotedString(thestr);
			printIndent(indent, out);
			if(key != NULL) {
				u_fprintf(out, "%s { \"%S\" } ", key, string);
			} else {
				u_fprintf(out, "\"%S\",", string);
			}
			if(VERBOSE) {
				u_fprintf(out, " // STRING");
			}
			u_fprintf(out, "\n");
			free(string);
		}
		break;
		case URES_INT :
			printIndent(indent, out);
			if(key != NULL) {
				u_fprintf(out, "%s", key);
			}
			u_fprintf(out, ":int { %li } ", ures_getInt(resource, status));
			if(VERBOSE) {
				u_fprintf(out, " // INT");
			}
			u_fprintf(out, "\n");
		break;
		case URES_BINARY :
		{
			int32_t len = 0;
			int8_t *data = (int8_t *)ures_getBinary(resource, &len, status);
			if(truncate && len > truncsize) {
				printIndent(indent, out);
				u_fprintf(out, "// WARNING: this resource, size %li is truncated to %li\n", len, truncsize/2);
				len = truncsize/2;
			}
			if(U_SUCCESS(*status)) {
				printIndent(indent, out);
				if(key != NULL) {
					u_fprintf(out, "%s", key);
				}
				u_fprintf(out, ":binary { ");
				for(i = 0; i<len; i++) {
					printHex(data++, out);
				}
				u_fprintf(out, " }");
				if(VERBOSE) {
					u_fprintf(out, " // BINARY");
				}
				u_fprintf(out, "\n");
			} else {
				reportError(currdir, locale, status, outerr);
			}
		}
		break;
		case URES_INT_VECTOR :
		{
			int32_t len = 0;
			const int32_t *data = ures_getIntVector(resource, &len, status);
			if(U_SUCCESS(*status)) {
				printIndent(indent, out);
				if(key != NULL) {
					u_fprintf(out, "%s", key);
				} 
				u_fprintf(out, ":intvector { ");
				for(i = 0; i<len-1; i++) {
					u_fprintf(out, "%d, ", data[i]);
				}
				if(len > 0) {
					u_fprintf(out, "%d ", data[len-1]);
				}
				u_fprintf(out, "}");
				if(VERBOSE) {
					u_fprintf(out, " // INTVECTOR");
				}
				u_fprintf(out, "\n");
			} else {
				reportError(currdir, locale, status, outerr);
			}
		}
		break;
		case URES_TABLE :
		case URES_ARRAY :
		{
			UResourceBundle *t = NULL;
			ures_resetIterator(resource);
			printIndent(indent, out);
			if(key != NULL) {
				u_fprintf(out, "%s ", key);
			}
			u_fprintf(out, "{");
			if(VERBOSE) {
				if(ures_getType(resource) == URES_TABLE) {
					u_fprintf(out, " // TABLE");
				} else {
					u_fprintf(out, " // ARRAY");
				}
			}
			u_fprintf(out, "\n");
			while(ures_hasNext(resource)) {
				t = ures_getNextResource(resource, t, status);
				printOutBundle(t, currdir, locale, (indent + indentsize), status, out, outerr);
			}
			printIndent(indent, out);
			u_fprintf(out, "}\n");
			ures_close(t);
		}
		break;
		default:
		break;
    }
}
