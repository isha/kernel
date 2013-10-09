/* disp.c : dispatcher
 */

#include <xeroskernel.h>

PCB * ReadyQueue;

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
      case CREATE: create(); break;
      case YIELD: ready(process); process = next(); break;
      case STOP: cleanup(process); process = next(); break;
    }  
  }
}

