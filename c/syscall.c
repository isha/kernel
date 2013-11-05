/* syscall.c : syscalls
 */

#include <xeroskernel.h>
#include <stdarg.h>

static int ret;

extern int syscall(RequestType call, ... ) {
  va_list ap;
  va_start(ap, call);
  __asm __volatile( 
    "movl %0, %%eax\n" : : "g" (call) : "%eax"); // Copy request type to eax
  __asm __volatile(
    "movl %1, %%edx\n"  // Copy address of arg list to edx
    "int $49\n" // Kernel int number, set in ctsw.c:contextinit()
    "movl %%eax, %0\n"  // Return value from interrupt to variable
    : "=g" (ret)
    : "g" (ap)
    : "%eax"
  );
  va_end(ap);
  return ret;
}

/* Creates a process and returns PID. Return -1 if unsuccesful */
extern int syscreate (void (*func)(), int stack) {
  int pid = syscall(CREATE, func, stack);
  return pid;
}

/* System yield */
extern void sysyield (void) {
  syscall(YIELD);
}

/* Stops a process and deallocates the memory assigned to it */
extern void sysstop (void) {
  syscall(STOP);
}

/* Returns the PID of the currently running process */
extern unsigned int sysgetpid(void) {
  return syscall(GET_PID);
}

/* Prints character string passed as parameter onto the screen */
extern void sysputs (char *str) {
  return syscall(PUTS, str);
}

/* IPC send message to another process */
extern int syssend (unsigned int dest_pid, void *buffer, int buffer_len ) {
  return syscall(SEND, dest_pid, buffer, buffer_len);
}

/* IPC recieve message from another process */
extern int sysrecv (unsigned int *from_pid, void *buffer, int buffer_len ) {
  return syscall(RECV, from_pid, buffer, buffer_len);
}
