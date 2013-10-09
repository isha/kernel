/* syscall.c : syscalls
 */

#include <xeroskernel.h>
#include <stdarg.h>

extern int syscall(RequestType call, ... ) {
  va_list ap;
  int ret;
  
  va_start(ap, call);

  __asm __volatile( 
    "movl %1, %%eax\n"
    "movl %2, %%edx\n"
    "int %3\n"
    "movl %%eax, %0\n"
    : "=g" (ret)
    : "g" (call), "g" (ap), "i" (49)
    : "%eax"
  );

  va_end(ap);
  return ret;
}

extern int syscreate (void (*func)(), int stack) {
  int pid = syscall(CREATE, func, stack);
}

extern void sysyield (void) {
  syscall(YIELD);
}

extern void sysstop (void) {
  syscall(STOP);
}
