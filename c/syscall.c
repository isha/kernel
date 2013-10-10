/* syscall.c : syscalls
 */

#include <xeroskernel.h>
#include <stdarg.h>

extern int syscall(RequestType call, ... ) {
  va_list ap;
  int ret = 0;
   
  va_start(ap, call);
  __asm __volatile( 
    "movl %0, %%eax\n" : : "g" (call) : "%eax");
  __asm __volatile(
    "movl %1, %%edx\n"
    "int $49\n"
    "movl %%eax, %0\n"
    : "=g" (ret)
    : "g" (ap)
    : "%eax"
  );

  va_end(ap);
  return ret;
}

extern int syscreate (void (*func)(), int stack) {
  int pid = syscall(CREATE, func, stack);
  return pid;
}

extern void sysyield (void) {
  syscall(YIELD);
}

extern void sysstop (void) {
  syscall(STOP);
}
