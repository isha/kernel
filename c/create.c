/* create.c : create a process
 */

#include <xeroskernel.h>

static int NextPID = 1;

extern int create (void (*func)(), int stack) {
  // Allocate large enough memory for process
  long size = sizeof(PCB)+sizeof(ContextFrame)+stack+16; // Extra 16 just in case
  PCB * pcb = kmalloc(size);

  if (pcb != NULL) {
    pcb->esp = pcb + size - (sizeof(ContextFrame) + 16);
    
    // Initial Context
    ContextFrame * cf = pcb->esp;
    cf->cs = getCS();
    cf->eip = (unsigned int) func;
    cf->eflags = 0x00003000;
    
    pcb->pid = NextPID++;

    ready(pcb);
  
    return pcb->pid;
  } else {
    return -1;
  }
}

extern void cleanup (PCB * pcb) {
  kfree(pcb);
}
