/*
 * Swap bytes in 16-bit [half-]words
 * for going between the 11 and the interdata
 */

#include <xeroslib.h>

void swab(register short *pf, register short *pt, register int n)
{

	n = n/2;
	while (--n >= 0) {
		*pt++ = (*pf << 8) + ((*pf >> 8) & 0377);
		pf++;
	}
}
