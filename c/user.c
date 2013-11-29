/* user.c : User processes
 */

#include <xeroskernel.h>
#include <xeroslib.h>

void producer( void ) {
/****************************/

    int         i;
    char        buff[100];

    for( i = 0; i < 20; i++ ) {

      sprintf(buff, "Producer %d and in hex %x\n", i, i);
      sysputs(buff);
      /*sysyield(); */
    }
    for (i = 0; i < 20; i++) {
      sysputs("P");
      syssleep(1500);
    }
    sprintf(buff, "Producer finished\n");
    sysputs( buff );
    sysstop();
}

void consumer( void ) {
/****************************/

    int         i;
    char        buff[100];

    for( i = 0; i < 10; i++ ) {
      sprintf(buff, "Consume %d\n", i);
      sysputs( buff );
      sysyield();
    }

    for (i = 0; i < 20; i++) {
      sysputs("C");
      syssleep(1000);
    }

    sprintf(buff, "Consumer finished\n");
    sysputs( buff );
    sysstop();
}


void	testKeyboard(void) {
	int fd = sysopen(KEYBOARDECHO);
	int *buffer;
	for(;;) {
		sysread(fd, buffer, 1);
		kprintf("%u \n", *buffer);	
	}
}

void     root( void ) {
/****************************/


    char  buff[100];
    sysputs("Root has been called\n");

    sysyield();
    sysyield();
    
    syscreate(&testKeyboard, 4096);   
    syscreate( &producer, 4096 );
    syscreate( &consumer, 4096 );
    sprintf(buff, "Root finished\n");
    sysputs( buff );
    sysstop();
}

