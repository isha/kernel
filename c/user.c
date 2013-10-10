/* user.c : User processes
 */

#include <xeroskernel.h>

extern void producer (void) {
  int i;
  for (i=0; i<12; i++) {
    kprintf(" Happy ");
    sysyield();
  }
  sysstop();
}

extern void consumer (void) {
  int i;
  for (i=0; i<15; i++) {
    kprintf("\n New Year ");
    sysyield();
  }
  sysstop();
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
