/* disp.c : dispatcher
 */

#include <xeroskernel.h>
#include <stdarg.h>

/* Pointer to first PCB in the Ready queue linked list */
PCB * ReadyQueue;

/* Adds specified PCB to the ready queue or creates a queue if doesnt exist */
void ready(PCB * pcb) {
  PCB * p;
  
  if (ReadyQueue) {
    p = ReadyQueue;
    while (p->next != NULL) p = p->next;
    p->next = pcb;
  } else {
    ReadyQueue = p;
  }
  pcb->next = NULL;
  pcb->state = READY;
}

/* Removes and returns a PCB from the ready queue */
PCB * next() {
  if (ReadyQueue) {
    PCB * next_ready = ReadyQueue;
    ReadyQueue = ReadyQueue->next;
    return next_ready;
  } else return NULL;
}

extern void dispatch() {
  PCB * process;
  process = next();
  
  RequestType request;

  for (;;) {
    request = contextswitch(process);
    
    switch(request) {
      case CREATE: 
      {
        va_list ap = process->esp;
        void (*func)() = va_arg(ap, long);
        int stack = va_arg(ap, int); 
        create(func, stack); 
        break;
      }
      case YIELD: 
        ready(process); 
        process = next(); 
        break;
      case STOP: 
        cleanup(process); 
        process = next(); 
        break;
    }  
  }
}

