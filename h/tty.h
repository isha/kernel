/* tty.h */

#define	IBLEN		256		/* input buffer size		*/
#define	OBLEN		256		/* output buffer size		*/


/* Device name definitions */

#define	CONSOLE     0			/* type tty      */
#define	SERIAL0     1			/* type serial   */
#define	SERIAL1     2			/* type serial   */
#define	KBMON       3			/* type kbmon    */


/* control() functions */

#define TTC_CPID	0x0001		/* set controlling PID		*/
#define	TTC_SYNC	0x0002		/* do synchronous output	*/
#define	TTC_GIF		0x0003		/* get input flags		*/
#define	TTC_GOF		0x0004		/* get output flags		*/
#define	TTC_NEXTC	0x0005		/* peek at next character	*/
#define	TTC_SUSER	0x0006		/* set uid for a tty		*/
#define	TTC_CUSER	0x0007		/* clear uid for a tty		*/

#define	NOCHAR		256

