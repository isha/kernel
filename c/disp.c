/* disp.c : dispatcher
 */

#include <xeroskernel.h>

PCB * ReadyQueue;
PCB * Running;

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

  for (;;) {
    
  }
}

/* Your code goes here */
