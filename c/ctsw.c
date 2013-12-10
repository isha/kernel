/* ctsw.c : context switcher
 */

#include <xeroskernel.h>
#include <i386.h>

void _common_entry_point(void);
void _timer_entry_point(void); // Placeholder for entry from timer interrupt
void _kernel_entry_point(void); // PLaceholder for entry from a system call

/* Initialize interrupt handler */
extern void contextinit(void) {
  set_evec(49, (int) _kernel_entry_point);
  set_evec(32, (int) _timer_entry_point);
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
    "_timer_entry_point:\n"
      "cli\n"
      "pusha\n"
      "movl %1, %0\n"
      "jmp _common_entry_point\n" : "=g" (call) : "g" (TIMER_INT) : "%eax");
  
  __asm __volatile(       
    "_kernel_entry_point:\n"
      "cli\n"
      "movl %%eax, %0\n" : "=g" (call) :: "%eax");
  
  __asm __volatile(
      "movl %%edx, %0\n"
      "pusha\n"
    "_common_entry_point:\n"
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
