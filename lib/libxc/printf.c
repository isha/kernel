/* printf.c - printf */

#include <xeroslib.h>
#include <xeroskernel.h>
#include <tty.h>


/*------------------------------------------------------------------------
 *  printf  --  write formatted output on CONSOLE 
 *------------------------------------------------------------------------
 */
int printf(char *fmt, int args)
{
	_doprnt(fmt, &args, putc, CONSOLE);
	return OK;
}
