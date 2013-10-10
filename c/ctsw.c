/* ctsw.c : context switcher
 */

#include <xeroskernel.h>
#include <i386.h>

/* Place holder for entry point into kernel from interrupt */
void _entry_point(void);

/* Initialize interrupt handler */
extern void contextinit(void) {
  set_evec(49, (int) _entry_point);
}

static void *k_stack; // Kernel stack 
static unsigned int save_esp; // Process stack
static int call;  // Request type from process
static long args; // Address of args from process

/* Switches context from process->kernel and vice versa */
extern RequestType contextswitch (PCB * pcb) {
  save_esp = pcb->esp;
  __asm __volatile(
      "pushf\n"
      "pusha\n"
      "movl %0, %%eax\n"
      "movl %%esp, k_stack\n"
      "movl save_esp, %%esp\n"
      "movl %%eax, 28(%%esp)\n"
      "popa\n"
      :
      : "g" (pcb->ret) 
      : "%eax"
  );
  
  __asm __volatile(
      "iret\n"   
    "_entry_point:\n"
      "movl %%eax, %0\n" : "=g" (call) :: "%eax");
  
  __asm __volatile(
      "movl %%edx, %0\n"
      "pusha\n"
      "movl %%esp, save_esp\n"
      "movl k_stack, %%esp\n"
      "popa\n"
      "popf\n"
      : "=g" (args)
      : 
      : "%eax", "%edx"
  );
  
  pcb->esp = save_esp;
  pcb->args = args;

  return call;
}
