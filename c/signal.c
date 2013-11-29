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
  
  /* Signal not registered */
  if (!p->signal_table[sig_no]) return;
  
  /* Sets the signal as waiting in the waiting mask */
  p->signal_waiting_mask = (1 << sig_no) | p->signal_waiting_mask;
      
  if (p->state != STATE_READY) {
    p->ret = -129;
    ready(p);
  }

  return 0;
}

/* Sets up the stack for a process to service the 
 * highest priority signal it has waiting
 * (depicted by a set bit in the waiting mask)
 */
void service_signals(pcb * p) {
  int mask = p->signal_waiting_mask & p->signal_register_mask;

  if (mask) {

    /* Find highest set bit */
    int sig_no = 0;
    while (mask != 0) { sig_no++; mask = mask >> 1; }
    sig_no--;
    
    unsigned int stack = p->esp;
  
    stack -= sizeof(signal_stack);
    signal_stack* s = (signal_stack*)(stack);
    s->handler = p->signal_table[sig_no];
    s->esp = p->esp;
    s->ret = p->ret;
    s->old_sp = p->esp;
       
    /* Context frame for going into trampoline code */ 
    stack -= sizeof(context_frame);
    context_frame* cf = (context_frame*)(stack);
    cf->ebp = stack;
    cf->esp = stack;
    cf->iret_eip = &sigtramp;
    cf->iret_cs = getCS();
    cf->eflags = 0x00003200;    
        
    p->esp = stack;
    
    /* Remove signal from the waiting mask */
    p->signal_waiting_mask &= ~(1 << sig_no);
  }
}
