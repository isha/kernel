/* syscall.c : syscalls
 */

#include <xeroskernel.h>
#include <stdarg.h>


int syscall( int req, ... ) {
/**********************************/

    va_list     ap;
    int         rc;

    va_start( ap, req );

    __asm __volatile( " \
        movl %1, %%eax \n\
        movl %2, %%edx \n\
        int  %3 \n\
        movl %%eax, %0 \n\
        "
        : "=g" (rc)
        : "g" (req), "g" (ap), "i" (KERNEL_INT)
        : "%eax" 
    );
 
    va_end( ap );

    return( rc );
}

 int syscreate( funcptr fp, int stack ) {
/*********************************************/

    return( syscall( SYS_CREATE, fp, stack ) );
}

 int sysyield( void ) {
/***************************/

    return( syscall( SYS_YIELD ) );
}

 int sysstop( void ) {
/**************************/

    return( syscall( SYS_STOP ) );
}

int sysgetpid( void ) {
/****************************/

    return( syscall( SYS_GETPID ) );
}

void sysputs( char *str ) {
/********************************/

    syscall( SYS_PUTS, str );
}

unsigned int syssleep( unsigned int t ) {
/*****************************/

    return syscall( SYS_SLEEP, t );
}

unsigned int sysopen (int device_no ) {
	return syscall( SYS_DEV_OPEN, device_no);
}

unsigned int sysclose (int fd) {
	return syscall(SYS_DEV_CLOSE, fd);
}

unsigned int syswrite(int fd, void *buff, int bufflen) {
	return syscall(SYS_DEV_WRITE, buff, bufflen);
}

unsigned int sysread(int fd, void *buff, int bufflen) {
	return syscall(SYS_DEV_READ, buff, bufflen);
}

unsigned int sysioctl(int fd, unsigned long command, ...) {
	return syscall(SYS_DEV_IOCTL, command);
}

/* Sets the provided newhandler method as handler for indicated signal 
 *  Return 0 on success
 *         -1 if signal is invalid 
 *         -2 if handler at invalid address
 *  Third argument is set to address of old handler
 */
int syssighandler(int signal, void (*newhandler)(void *), void (** oldHandler)(void *)) {
    return syscall(SYS_SIGHANDLER, signal, newhandler, oldHandler);
}


/* Used by trampoline code to replace stored stack pointer for the process
 * with old_sp.
 */
void sigreturn(void *old_sp) {
  return syscall(SYS_SIGRETURN, old_sp);
}

/* System call to request signal indicated by signalNumber to be delivered
 * to process with pid as firts parameter
 * Returns 0 on success
 *          -33 if target process doesn't exist
 *          -12 if signalNumber is invalid
 */
int syskill(int PID, int signalNumber) {
  return syscall(SYS_KILL, PID, signalNumber);
}


/* Suspends process until a signal is received 
 */
int syssigwait(void) {
  return syscall(SYS_SIGWAIT);
}
