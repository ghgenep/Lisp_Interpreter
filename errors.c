
#include "headers.h"

#define EXIT_SOME 0
#define ERROR_STREAM stderr

/* error: print an error message to ERROR_STREAM */
void error(char *message, char *expr, int exit_status) {
	fprintf(ERROR_STREAM, "ERROR: ");
	if (expr != NULL)
		fprintf(ERROR_STREAM, message, expr);
	else
		fprintf(ERROR_STREAM, "%s", message);
	fprintf(ERROR_STREAM, "\n");
	exit(exit_status);
}

/* ec_malloc: an error checked malloc function */
void *ec_malloc(size_t size) {
    void *ptr;

    if ((ptr = malloc(size)) == NULL)
	    error("when allocating memory", NULL, EXIT_SOME);
    return ptr;
}