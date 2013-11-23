#include <xeroskernel.h>
#include <stdarg.h>

/* Runs in user space and is used to control the signal processing in the 
 * application. When handler returns, call syssigreturn
 */
void sigtramp(void (*handler)(void *), void *cntx, void *osp) {
  handler(cntx);
  sigreturn(osp);

  kprintf("\nsigtramp.c: Control should never get here!");
}


/* Delivers signal to a process 
 * Returns 0 on success
 *         -1 if PID is invalid
 *         -2 if signal is invalid
 */
int signal(int pid, int sig_no) {
  pcb * p = findPCB(pid);


}


