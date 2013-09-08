
#include <xeroslib.h>


/*
 *  Copy the character to the memory pointer
 */
void memset(void *pch,
	    int c,
	    int len)
{
  unsigned char *byte = pch;
  register int n;
  
  if ((n = len) <= 0)
    return;
  do
    *(char *)byte++ = c;
  while (--n);
}
