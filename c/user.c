/* user.c : User processes
 */

#include <xeroskernel.h>
#include <xeroslib.h>


void helloworld( void ) {
  kprintf("\nHello World!");
}

void first(void) {
  kprintf("\nFirst this");
}

void producer( void ) {
/****************************/

    void * old;  
    
    syssighandler(10, &first, old);
    syssighandler(2, &helloworld, old);
    while(1);
    
    sysstop();
}

void consumer( void ) {
/****************************/
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

    int n = syscreate( &producer, 4096 );

    sysyield();
    sysyield();
    
    syskill(n, 2);
    syskill(n, 10);

    sprintf(buff, "Root finished\n");
    sysputs( buff );
    sysstop();
}

