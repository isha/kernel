/* disp.c : dispatcher
 */

#include <xeroskernel.h>

PCB * ReadyQueue;

void ready(PCB * pcb) {
  PCB * p = ReadyQueue;
  while (p->next != NULL) p = p->next;
  p->next = pcb;
  pcb->next = NULL;
  pcb->state = READY;
}

PCB * next() {
  PCB * next_ready = ReadyQueue;
  ReadyQueue = ReadyQueue->next;
  return next_ready;
}

extern void setup_process_queues (PCB * process) {
  process->next = NULL;
  process->state = READY;
  ReadyQueue = process;
}

extern void dispatch() {
  PCB * process;
  process = next();
  
  RequestType request;

  for (;;) {
    request = contextswitch(process);
    switch(request) {
      case CREATE: create(); break;
      case YIELD: ready(process); process = next(); break;
      case STOP: cleanup(process); process = next(); break;
    }  
  }
}

/* Your code goes here */
