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


/*void	testKeyboard(void) {
	int fd = sysopen(KEYBOARDECHO);
	int *buffer;
	for(;;) {
		sysread(fd, buffer, 1);
		kprintf("%u \n", *buffer);	
	}
}*/

void     root( void ) {
/****************************/


    char  buff[100]; int rc; void (** old) (void *); int ret_val;

    sysputs("\nStep 1: Greeting");


    sysputs("\nStep 2: Open echo keyboard");
	int echo_fd = sysopen(KEYBOARDECHO);
	if(echo_fd == -3) sysputs("\nError, device already in use ");
	if(echo_fd == -2) sysputs("\nError, bad device number.");
	if(echo_fd == -1) sysputs("\nError, no empty fdslot.");
	if(echo_fd > -1) sysputs("\nfd retreival success!");
   
    sysputs("\nStep 3: Read chars one at a time till 10");
	char *c;
	int i;
	for (i = 0; i <10; i++) {
//		ret_val = sysread(echo_fd, c, 1);
	//Here we only want to get the next character if it the user HAS entered it. I
	//f the userhas not, 0 is returned by sysread, so we don't iterate until we get something from him
//		if (ret_val != 0) i++;
		sprintf(buff, "\n %d", c);
		sysputs(buff);	
	}


    sysputs("\nStep 4: Open no echo keyboard");
	int noecho_fd = sysopen(KEYBOARD);
	if(noecho_fd == -3) sysputs("\nError, device already in use ");
	if(noecho_fd == -2) sysputs("\nError, bad device number. ");
	if(noecho_fd == -1) sysputs("\nError, no empty fdslot.");
	if(noecho_fd > -1) sysputs("\nfd retreival success!");


    sysputs("\nStep 5: Open echo keyboard");
	echo_fd = sysopen(KEYBOARDECHO);
	if(echo_fd == -3) sysputs("\nError, device already in use");
	if(echo_fd == -2) sysputs("\nError, bad device number.");
	if(echo_fd == -1) sysputs("\nError, no empty fdslot.");
	if(echo_fd > -1) sysputs("\nfd retreival success!");


    sysputs("\nStep 6: Close keyboard");
	ret_val = sysclose(noecho_fd);
	if(ret_val == -1) sysputs("\nError, bad FD entered");


    sysputs("\nStep 7: Open no echo keyboard");
	noecho_fd = sysopen(KEYBOARD);
	if(noecho_fd == -3) sysputs("\nError, device already in use");
	if(noecho_fd == -2) sysputs("\nError, bad device number.");
	if(noecho_fd == -1) sysputs("\nError, no empty fdslot.");
	if(noecho_fd > -1) sysputs("\nfd retreival success!");


    sysputs("\nStep 8: 3 reads of 10 chars");
//	for (i = 0; i <30;i++) {
//		ret_val = sysread(noecho_fd, c, 1);
//		sprintf(buff, "%d \n", *c);
//		if (ret_val != 0) i++; //see reasoning in step 3. 
//		sysputs(buff);
//	}	
	sysputs("\n");


    sysputs("\nStep 9: Continue reading until EOF");
	sysioctl(noecho_fd, 0x04) ; //SET EOF to be Capslock.
	
//	for (;;) {
//		ret_val = sysread(noecho_fd, c, 1);
//		sprintf(buff, "%d \n", *c);
//		if (ret_val == 2) break; //we won't exit this loop until capslock is hit. 
//		sysputs(buff);
//	}
	

    sysputs("\nStep 10: Close keyboard and open echo");
	ret_val = sysclose(noecho_fd);
	if(ret_val == -1) sysputs("Error, bad FD entered");

	echo_fd = sysopen(KEYBOARDECHO);
	if(echo_fd == -3) sysputs("\nError, device already in use");
	if(echo_fd == -2) sysputs("\nError, bad device number.");
	if(echo_fd == -1) sysputs("\nError, no empty fdslot.");
	if(echo_fd > -1) sysputs("\nfd retreival success! ");
	

    sysputs("\nStep 11: Install sighandler for 18");
      rc = syssighandler(18, &root_handler, old);
      sprintf(buff, "\nInstalled syshandler for root (pid %d), return code %d, old handler %d", sysgetpid(), rc, *old);
      sysputs(buff);
    

    sysputs("\nStep 12: Create process");    
      syscreate( &process_a, 4096 );


    sysputs("\nStep 13: Do a read");
//	sysread(echo_fd, c, 1);
//	sprintf(buff, "%d \n", *c);


    sysputs("\nStep 14: Previous read interrupted by signal");

    sysputs("\nStep 15: Create process");
      syscreate(&process_b, 4096);
 

    sysputs("\nStep 16: Install second sighandler for 18");
      rc = syssighandler(18, &root_second_handler, old);
      sprintf(buff, "\nInstalled syshandler for root (pid %d), return code %d, old handler %d", sysgetpid(), rc, *old);
      sysputs(buff);
   

    sysputs("\nStep 17: Do another read");
//	sysread(echo_fd, c, 1);
//	sprintf(buff, "%d \n", *c);

    sysputs("\nStep 18: Previous read interrupted by signal");
    


    sysputs("\nStep 19: Install handler for 20 (which was old 18 handler)");
      void * h = *old;
      rc = syssighandler(20, h, old);
      sprintf(buff, "\nInstalled syshandler for root (pid %d), return code %d, old handler %d", sysgetpid(), rc, *old);
      sysputs(buff);
       

    sysputs("\nStep 20: Create process");
      syscreate(&process_c, 4096);
      

    sysputs("\nStep 21: Do another read"); 
//	sysread(echo_fd, c, 1);
//	sprintf(buff, "%d \n", *c);
  

  sysputs("\nStep 22: Previous read interrupted by signal");

    sysputs("\nStep 23: Read until EOF");

	sysioctl(noecho_fd, 0x04) ; //SET EOF to be Capslock.
	
//	for (;;) {
//		ret_val = sysread(noecho_fd, c, 1);
//		sprintf(buff, "%d \n", *c);
//		if (ret_val == 2) break; //we won't exit this loop until capslock is hit. 
//		sysputs(buff);
//	}
   

 sysputs("\nStep 24: Read again");
   
//	sysread(echo_fd, c, 1);
//	sprintf(buff, "%d \n", *c);
      
    sysputs("\nStep 25: Termination and exit");

    while(1);   
    sysstop();
}

