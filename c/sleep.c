/* sleep.c : sleep device (assignment 2)
 */

#include <xeroskernel.h>

PCB * SleepQueue;

extern void sleep(PCB * pcb, unsigned int ms) {
 pcb->state = SLEEPING;
 if (SleepQueue == NULL) {
   pcb->sleep_counter = ms;
   SleepQueue = pcb;
   return;
 } else {
   int sleep_so_far = 0;
   PCB *prev, *current = SleepQueue;
   while (current != NULL && ms < sleep_so_far) {
     sleep_so_far += current->sleep_counter;
     prev = current;
     current = current->next;
   }
  
   // to be added at the last position
   if (current == NULL && prev != NULL) {
     prev->next = pcb; 
     pcb->sleep_counter = ms - sleep_so_far;
   } 
   // to be added at the first position
   else if (current == SleepQueue) {
     pcb->sleep_counter = ms;
     SleepQueue->sleep_counter -= pcb->sleep_counter;
     pcb->next = SleepQueue;
     SleepQueue = pcb;
   } 
   // somewhere in between
   else {
     pcb->sleep_counter = ms - sleep_so_far;
     prev->next = pcb;
     current->sleep_counter -= pcb->sleep_counter; 
     pcb->next = current;
   }
 }
}

extern void tick() {
  if (SleepQueue) {
    SleepQueue->sleep_counter--;
    if (SleepQueue->sleep_counter == 0) {
      PCB * pcb = SleepQueue;
      SleepQueue = SleepQueue->next;
      pcb->ret = 0;
      ready(pcb);
    }
  }
}
