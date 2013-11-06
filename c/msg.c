/* msg.c : messaging system (assignment 2)
 */

#include <xeroskernel.h>

/* Return PCB for requested pid if found in the ready queue */
PCB * get_process (unsigned int pid) {
  PCB * process = ReadyQueue;

  while (process != NULL) {
    if (process->pid == pid) return process;
    process = process->next;
  }
  return NULL;
}

PCB * add_to_queue(PCB * queue, PCB * pcb) {
  PCB * current = queue;
  if (current == NULL) return pcb;

  while (current != NULL) current = current->next;
  current->next = pcb;
  return queue;
}

PCB * remove_from_queue(PCB * queue, PCB * pcb) {
  PCB * current = queue; PCB * prev = NULL;
  while (current != NULL && current != pcb) {
    prev = current;
    current = current->next;
  }

  if (prev == NULL) {
    return current->next;
  } else {
    prev->next = current->next;
    return queue;
  }
}

int send(PCB * process, unsigned int dest_pid, void * buffer, int buffer_len) {
  if (dest_pid == process->pid) return CIRCULAR_IPC_ERROR;
  PCB * dest_pcb = get_process(dest_pid);
  if (dest_pcb == NULL) return PROCESS_NOT_FOUND;
  
  PCB * current = dest_pcb->message_queue;
  while (current != NULL) {
    /* Match expected sender on the destination processes message queue */
    if (current->pid == process->pid && dest_pcb->state == BLOCKED) {
      unsigned int length = dest_pcb->message_buffer.size < buffer_len ? dest_pcb->message_buffer.size : buffer_len;
      blkcopy(dest_pcb->message_buffer.address, buffer, length);
      dest_pcb->message_queue = remove_from_queue(dest_pcb->message_queue, process); 
      ready(dest_pcb);
      return length;
    }
    current = current->next;     
  }
  
  process->message_buffer.pid = dest_pcb->pid;
  process->message_buffer.address = buffer;
  process->message_buffer.size = buffer_len;
  process->state = BLOCKED;
  dest_pcb->message_queue = add_to_queue(dest_pcb->message_queue, process); 
  return OTHER_IPC_ERROR;
}

int recv(PCB * process, unsigned int * from_pid, void * buffer, int buffer_len) {
  if (from_pid == process->pid) return CIRCULAR_IPC_ERROR;

  if (*from_pid != 0) {
    PCB * from_pcb = get_process(from_pid);
    if (from_pcb == NULL) return PROCESS_NOT_FOUND;

    if (from_pcb->state == BLOCKED && from_pcb->message_buffer.pid == process->pid) {
      unsigned int length = from_pcb->message_buffer.size > buffer_len ? buffer_len : from_pcb->message_buffer.size;

      blkcopy(buffer, from_pcb->message_buffer.address, length);
      process->message_queue = remove_from_queue(process->message_queue, from_pcb);
      ready(from_pcb);
      return length;
    } else {
      process->message_queue = add_to_queue(process->message_queue, from_pcb);
      process->message_buffer.pid = from_pcb->pid;
    }
  }

  process->message_buffer.address = buffer;
  process->message_buffer.size = buffer_len;
  process->state = BLOCKED;

  return OTHER_IPC_ERROR;
}


