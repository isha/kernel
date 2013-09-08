/*
 * Return the ptr in sp at which the character c appears;
 * NULL if not found
 */

#include <xeroslib.h>
#include <xeroskernel.h>

char * index(char *sp, char c)
{
	do {
		if (*sp == c)
			return(sp);
	} while (*sp++);
	return(NULL);
}
