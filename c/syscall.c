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
/*
 * Opens a device based upon device number.
 * Returns the File Decriptor if successful.
 * If it fails, returns -1 if thee process has no applicable FD, -2 if the device does not exist, -3 if the device is already in use. 
 */
unsigned int sysopen (int device_no ) {
	return syscall( SYS_DEV_OPEN, device_no);
}

/*
 *takes in an fd, and closes that device file.
 * returns -1 if the fd is invalid. 
 * returns 0 on success
 * ensures both echokeyboard and regular keyboard are closed/
 */
unsigned int sysclose (int fd) {
	return syscall(SYS_DEV_CLOSE, fd);
}

/*
 *writes information to the buffer that will be shared with the device.
 * returns -1 if the fd is invalid.
 * returns 0 on sucess
 */
unsigned int syswrite(int fd, void *buff, int bufflen) {
	return syscall(SYS_DEV_WRITE, buff, bufflen);
}

/* reads information from the buffer that is shared from the device.
 * returns -1 if fd is invalid.
 * returns 0 on success
 */
unsigned int sysread(int fd, char *buff, int bufflen) {
	return syscall(SYS_DEV_READ, buff, bufflen);
}
/* sends special control information as a long int to the device.
 * returns -1 if the fd is bad, 0 if the control signal sent is a success
 */
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
