/* ctsw.c : context switcher
 */

#include <xeroskernel.h>
#include <i386.h>

void _entry_point();

extern void contextinit(void) {
  set_evec(49, (unsigned long) _entry_point);
}

static void *k_stack;
static unsigned int save_esp;

extern RequestType contextswitch (PCB * pcb) {
  kprintf("\nSwitching to PID %d ", pcb->pid);
  save_esp = pcb->esp;
  
  RequestType call; 
  __asm __volatile(
      "pushf\n"
      "pusha\n"
      "movl %%esp, k_stack\n"
      "movl save_esp, %%esp\n"
      "popa\n"
      "iret\n"
    "_entry_point:\n"
      "pusha\n"
      "movl %%esp, save_esp\n"
      "movl k_stack, %%esp\n"
      "popa\n"
      "popf\n"
      : 
      : 
      : "%eax"
  );
  
  return call;
}
