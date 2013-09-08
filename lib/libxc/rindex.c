/*
 * Return the ptr in sp at which the character c last
 * appears; NULL if not found
*/

#include <xeroslib.h>
#include <xeroskernel.h>

char *
rindex(register char *sp, register char c)
{
	register char *r;

	r = NULL;
	do {
		if (*sp == c)
			r = sp;
	} while (*sp++);
	return r;
}
