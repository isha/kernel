/* scanf.c - scanf, fscanf, sscanf, getch, ungetch, sgetch, sungetch */

/*#include <conf.h> */
#include <xeroskernel.h> 
#include <xeroslib.h>
/*#include <io.h> */
#include <tty.h>

static	int   sgetch(int dummy, char **cpp);
static  int   getch(int dev, int *buf);
static  int   sungetch(int dummy, char **cpp);
static  void  ungetch(int dev, int *buf);

/*------------------------------------------------------------------------
 *  scanf  --  read from the console according to a format
 *------------------------------------------------------------------------
 */
int scanf(char * fmt, char args)

{
	int	buf;

	buf = EMPTY;
	return(_doscan(fmt, (int **) &args, getch, ungetch, CONSOLE, &buf));
}

/*------------------------------------------------------------------------
 *  fscanf  --  read from a device (file) according to a format
 *------------------------------------------------------------------------
 */

int fscanf(int dev, char *fmt, int args)
{
	int	buf;

	buf = EMPTY;
	return(_doscan(fmt, (int **)&args, getch, ungetch, dev, &buf));
}

/*------------------------------------------------------------------------
 *  getch  --  get a character from a device with pushback
 *------------------------------------------------------------------------
 */
static	int getch(int dev, int *buf)
{
	if( *buf != EOF && *buf != EMPTY)
		*buf = getc(dev);
	if( *buf != EOF )
		*buf = control(dev, TTC_NEXTC);
	return(*buf);
}

/*------------------------------------------------------------------------
 *  ungetch  --  pushback a character for getch
 *------------------------------------------------------------------------
 */
static void ungetch(int dev, int *buf)
{
	*buf = EMPTY;
}

/*------------------------------------------------------------------------
 *  sscanf  --  read from a string according to a format
 *------------------------------------------------------------------------
 */
int sscanf(char * str, char *fmt, int args)
{
	char	*s;

	s = str;
	return(_doscan(fmt, (int **) &args, sgetch, sungetch, 0, &s));
}

/*------------------------------------------------------------------------
 *  sgetch  -- get the next character from a string
 *------------------------------------------------------------------------
 */
static	int sgetch(int dummy, char **cpp)
{
	return( *(*cpp) == '\0' ? EOF : *(*cpp)++ );
}

/*------------------------------------------------------------------------
 *  sungetc  --  pushback a character in a string
 *------------------------------------------------------------------------
 */
static	int sungetch(int dummy, char **cpp)
{
	return(*(*cpp)--);
}
