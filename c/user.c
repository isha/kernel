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

void process_a( void ) {
    syssleep(1000);
    syskill(2, 20);
    syskill(2, 18);
    sysstop();
}

void process_b(void) {
  syssleep(5000);
  syskill(2, 18);
  sysstop();
}

void process_c(void) {
  syssleep(5000);
  syskill(2, 20);
  sysstop();
}


void root_handler (void) {
  sysputs("\nIn root handler");
}

void root_second_handler (void) {
  sysputs("\nIn second handler for root");
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


    char  buff[100]; int rc; void (** old) (void *);

    sysputs("\nStep 1: Greeting");

    sysputs("\nStep 2: Open echo keyboard");

    sysputs("\nStep 3: Read chars one at a time till 10");

    sysputs("\nStep 4: Open no echo keyboard");

    sysputs("\nStep 5: Open echo keyboard");

    sysputs("\nStep 6: Close keyboard");

    sysputs("\nStep 7: Open no echo keyboard");

    sysputs("\nStep 8: 3 reads of 10 chars");

    sysputs("\nStep 9: Continue reading until EOF");

    sysputs("\nStep 10: Close keyboard and open echo");

    sysputs("\nStep 11: Install sighandler for 18");
      rc = syssighandler(18, &root_handler, old);
      sprintf(buff, "\nInstalled syshandler for root (pid %d), return code %d, old handler %d", sysgetpid(), rc, *old);
      sysputs(buff);
    
    sysputs("\nStep 12: Create process");    
      syscreate( &process_a, 4096 );

    sysputs("\nStep 13: Do a read");

    sysputs("\nStep 14: Previous read interrupted by signal");

    sysputs("\nStep 15: Create process");
      syscreate(&process_b, 4096);
 
    sysputs("\nStep 16: Install second sighandler for 18");
      rc = syssighandler(18, &root_second_handler, old);
      sprintf(buff, "\nInstalled syshandler for root (pid %d), return code %d, old handler %d", sysgetpid(), rc, *old);
      sysputs(buff);
   
    sysputs("\nStep 17: Do another read");
    
    sysputs("\nStep 18: Previous read interrupted by signal");
    
    sysputs("\nStep 19: Install handler for 20 (which was old 18 handler)");
      void * h = *old;
      rc = syssighandler(20, h, old);
      sprintf(buff, "\nInstalled syshandler for root (pid %d), return code %d, old handler %d", sysgetpid(), rc, *old);
      sysputs(buff);
       
    sysputs("\nStep 20: Create process");
      syscreate(&process_c, 4096);
      
    sysputs("\nStep 21: Do another read"); 

    sysputs("\nStep 22: Previous read interrupted by signal");

    sysputs("\nStep 23: Read until EOF");

    sysputs("\nStep 24: Read again");
    
    sysputs("\nStep 25: Termination and exit");

    while(1);   
    sysstop();
}

