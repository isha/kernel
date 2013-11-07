/* sprintf.c - sprintf */


#include <xeroslib.h>


static int sprntf(char **, char c);

/*------------------------------------------------------------------------
 *  sprintf  --  format arguments and place output in a string
 *------------------------------------------------------------------------
 */
int sprintf(char *str, char *fmt, ...)
{
        char    *s;
	int *args = (int *) &fmt;
	
	/* 
         * In a bit of hackery, determine the address of the parameter
         * after fmt;
         */
	args++;

        s = str;

        _doprnt(fmt, args, sprntf, (unsigned int) &s);
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


