/* user.c : User processes
 */

#include <xeroskernel.h>

/* Idle process - supposed to run when no other process is there */
extern void idleproc (void) {
  while (TRUE) 
    __asm__ __volatile__( "hlt\n" );
}

extern void proc_a (void) {
  char * str;
  sprintf(str, "\nProcess %d: Alive", sysgetpid());
  sysputs(str);
  syssleep(5000);
  
  int *time_to_sleep = kmalloc(sizeof(int));
  int *from_pid;
  *from_pid = 1;
  int rc = sysrecv(from_pid, time_to_sleep, sizeof(int));

  sprintf(str, "\nProcess %d: message recv. Time to sleep is %d. RC %d", sysgetpid(), *time_to_sleep, rc);
  sysputs(str);

  syssleep(*time_to_sleep);
  sprintf(str, "\nProcess %d: done sleeping, goodbye!", sysgetpid());
  sysputs(str);

  sysstop();
}

extern void proc_b (void) {
  char * str;
  sprintf(str, "\nProcess %d: Alive", sysgetpid());
  sysputs(str);
  syssleep(5000);
  
  int *time_to_sleep = kmalloc(sizeof(int));
  int *from_pid;
  *from_pid = 1;
  sysrecv(from_pid, time_to_sleep, sizeof(int));

  sprintf(str, "\nProcess %d: message received. Time to sleep is %d", sysgetpid(), *time_to_sleep);
  sysputs(str);

  syssleep(*time_to_sleep);
  sprintf(str, "\nProcess %d: done sleeping, goodbye!", sysgetpid());
  sysputs(str);

  sysstop();
}

extern void proc_c (void) {
  char * str;
  sprintf(str, "\nProcess %d: Alive", sysgetpid());
  sysputs(str);
  syssleep(5000);
  
  int *time_to_sleep = kmalloc(sizeof(int));
  int *from_pid;
  *from_pid = 1;
  sysrecv(from_pid, time_to_sleep, sizeof(int));

  sprintf(str, "\nProcess %d: message received. Time to sleep is %d", sysgetpid(), *time_to_sleep);
  sysputs(str);

  syssleep(*time_to_sleep);
  sprintf(str, "\nProcess %d: done sleeping, goodbye!", sysgetpid());
  sysputs(str);

  sysstop();
}

extern void proc_d (void) {
  char * str;
  sprintf(str, "\nProcess %d: Alive", sysgetpid());
  sysputs(str);
  syssleep(5000);
  
  int *time_to_sleep = kmalloc(sizeof(int));
  int *from_pid;
  *from_pid = 1;
  sysrecv(from_pid, time_to_sleep, sizeof(int));

  sprintf(str, "\nProcess %d: message received. Time to sleep is %d", sysgetpid(), *time_to_sleep);
  sysputs(str);

  syssleep(*time_to_sleep);
  sprintf(str, "\nProcess %d: done sleeping, goodbye!", sysgetpid());
  sysputs(str);

  sysstop();
}

/* Root process to carry out the extended Producer-Consumer problem as specified in assignment 2 outline */
extern void root (void) {
  sysputs("\nRoot: Alive");
  char * str;

  int a = syscreate(proc_a, 4096);
  int b = syscreate(proc_b, 4096);
  int c = syscreate(proc_c, 4096);
  int d = syscreate(proc_d, 4096);
  
  syssleep(4000);
  
  int *time_to_sleep = kmalloc(sizeof(int));

  *time_to_sleep = 10000; 
  syssend(c, time_to_sleep, sizeof(int));
  
  *time_to_sleep = 7000; 
  syssend(b, time_to_sleep, sizeof(int));
 
  *time_to_sleep = 20000; 
  syssend(a, time_to_sleep, sizeof(int));
  
  *time_to_sleep = 27000; 
  syssend(d, time_to_sleep, sizeof(int));

  int *from_pid = kmalloc(sizeof(int));
  *from_pid = d;
  int rc = sysrecv(from_pid, time_to_sleep, sizeof(int));
  sprintf(str, "Root: return code when trying to receive from %d is %d", d, rc);
  sysputs(str);
  
  rc = syssend(c, time_to_sleep, sizeof(int));
  sprintf(str, "Root: return code when trying to send to %d is %d", c, rc);
  sysputs(str);

  while(TRUE);
};

/* Legacy from assignment 1 */
extern void producer (void) {
  int i;
  for (i=0; i<12; i++) {
    kprintf(" Happy ");
    sysyield();
  }
  char * str;
  sysstop();
}

extern void consumer (void) {
  int i;
  char * str;
  for (i=0; i<15; i++) {
    kprintf("\n New Year ");
    sysyield();
  }
  sysstop();
}
