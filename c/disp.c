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
    ReadyQueue = pcb;
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

/* Dispatches to next action based on process request. 
 * Also fetches process arguments for a system call
 */
extern void dispatch() {
  PCB * process;
  process = next();
  
  RequestType request;
  for (;;) {
    request = contextswitch(process);
    switch(request) {
      case CREATE: 
      {
        va_list ap = process->args;
        void (*func)() = va_arg(ap, long);
        int stack = va_arg(ap, int); 
        process->ret = create(func, stack); 
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
      case GET_PID:
        process->ret = process->pid;
        break;
      case PUTS:
      {
        va_list ap = process->args;
        char *str = va_arg(ap, char *);
        kprintf("%s", str);
        break;
      }
      case SEND:
      { 
        va_list ap = process->args;
        int dest_pid = va_arg(ap, unsigned int);
        void * buff = va_arg(ap, void *);
        int buff_len = va_arg(ap, int);
        process->ret = send(process, dest_pid, buff, buff_len);
        break;
      }
      case RECV:
      { 
        va_list ap = process->args;
        unsigned int * from_pid = va_arg(ap, unsigned int *);
        void * buff = va_arg(ap, void *);
        int buff_len = va_arg(ap, int);
        process->ret = recv(process, from_pid, buff, buff_len);
        break;
      }
    }  
  }
}

