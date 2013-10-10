/* ctsw.c : context switcher
 */

#include <xeroskernel.h>
#include <i386.h>

void _entry_point(void);

extern void contextinit(void) {
  set_evec(49, (int) _entry_point);
}

static void *k_stack; // Kernel stack 
static unsigned int save_esp; // Process stack
static int call;

extern RequestType contextswitch (PCB * pcb) {
  save_esp = pcb->esp;
  __asm __volatile(
      "pushf\n"
      "pusha\n"
      "movl %%esp, k_stack\n"
      "movl save_esp, %%esp\n"
      "popa\n"
      "iret\n"
    "_entry_point:\n"
      "movl %%eax, %0\n" : "=g" (call) :: "%eax");
  
  __asm __volatile(
      "pusha\n"
      "movl %%esp, save_esp\n"
      "movl k_stack, %%esp\n"
      "popa\n"
      "popf\n"
      :
      : 
      : "%eax"
  );
  pcb->esp = save_esp;
  return call;
}
