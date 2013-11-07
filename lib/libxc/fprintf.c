/* fprintf.c - fprintf */

#define	OK	1
#include <xeroslib.h>


/*------------------------------------------------------------------------
 *  fprintf  --  print a formatted message on specified device (file)
 *------------------------------------------------------------------------
 */
int fprintf(int dev, char *fmt, int args)
{


        _doprnt(fmt, &args, putc, dev);
        return(OK);
}
