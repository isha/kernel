/* puts.c - puts */

#include <xeroslib.h>
#include <tty.h>

/*------------------------------------------------------------------------
 *  puts  --  write a null-terminated string to the console
 *------------------------------------------------------------------------
 */
int puts(register char *s)
{
	register int c;

	while ((c = *s++))
                putc(CONSOLE,c);
        return(putc(CONSOLE,'\n'));
}
