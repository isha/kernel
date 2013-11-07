/*
 * Returns the number of
 * non-NULL bytes in string argument.
 */


#include <xeroslib.h>


int strlen(register char *s)
{
	register int n;

	n = 0;
	while (*s++)
		n++;

	return n;
}
