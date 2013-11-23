#include <xeroskernel.h>
#include <stdarg.h>

/* Runs in user space and is used to control the signal processing in the 
 * application. When handler returns, call syssigreturn
 */
void sigtramp(void (*handler)(void *), void *cntx, void *osp) {

}


/* Delivers signal to a process 
 * Returns 0 on success
 *         -1 if PID is invalid
 *         -2 if signal is invalid
 */
int signal(int pid, int sig_no) {

}


