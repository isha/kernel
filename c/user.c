/* user.c : User processes
 */

#include <xeroskernel.h>

extern void producer (void) {
  int i;
  for (i=0; i<12; i++) {
    kprintf(" Happy ");
    sysyield();
  }
  char * str;
  sprintf(str, "\nPID for producer is %d", sysgetpid());
  sysputs(str);
  sysstop();
}

extern void consumer (void) {
  int i;
  char * str;
  sprintf(str, "\nPID for consumer is %d , before sleeping", sysgetpid());
  sysputs(str);
  syssleep(2000);
  sprintf(str, "\nAfter sleeping");
  sysputs(str);
  for (i=0; i<15; i++) {
    kprintf("\n New Year ");
    sysyield();
  }
  sysstop();
}

extern void idleproc (void) {
  while (TRUE) 
    __asm__ __volatile__( "hlt\n" );
}

extern void root (void) {
  kprintf("\nTo: World\n\tHello!\nFrom: Root");
  
  int p = syscreate(producer, 4096);
  int c = syscreate(consumer, 4096);
  
  kprintf("\n\nProducer PID %d, Consumer PID %d", p, c);

  while (TRUE) {
    sysyield();
  }
};
