/* sprintf.c - sprintf */


#include <xeroslib.h>


static int sprntf(char **, char c);

/*------------------------------------------------------------------------
 *  sprintf  --  format arguments and place output in a string
 *------------------------------------------------------------------------
 */
int sprintf(char *str, char *fmt, int args)
{
        char    *s;
	
        s = str;
        _doprnt(fmt, &args, sprntf, (int) &s);
        *s++ = '\0';

        return((int)str);
}

/*------------------------------------------------------------------------
 *  sprntf  --  routine called by doprnt to handle each character
 *------------------------------------------------------------------------
 */

static	int sprntf(char **cpp, char c)
{
        return(*(*cpp)++ = c);
}


