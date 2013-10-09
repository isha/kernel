/* create.c : create a process
 */

#include <xeroskernel.h>

static int NextPID = 1;

extern int create (void (*func)(), int stack) {
  long size = sizeof(PCB)+sizeof(ContextFrame)+stack*2;
  PCB * pcb = kmalloc(size);

  if (pcb != NULL) {
    pcb->esp = pcb + size - (sizeof(ContextFrame) + 16);
    
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
}
/* Your code goes here. */
