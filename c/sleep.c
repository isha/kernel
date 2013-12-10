/* sleep.c : sleep device (assignment 2)
 */

#include <xeroskernel.h>

#define TICKS_PER_MS 0.1
PCB * SleepQueue;

/* Adds the passed process to the sleep queue and sets sleep value to the specified value */
extern void sleep(PCB * pcb, unsigned int ms) {
 ms = ms * TICKS_PER_MS; // ms is now a tick value

 pcb->state = SLEEPING;
 // Initialize delta list if doesnt already exist
 if (SleepQueue == NULL) {
   pcb->sleep_counter = ms;
   SleepQueue = pcb;
   return;
 } 
 // If delta list exists, calculate where to insert this process
 else {
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

/* Called at every timer interrupt and checks if any sleeping process need to put back onto the ReadyQueue. Since this is a delta list, only need to check the first element */
extern void tick() {
  if (SleepQueue) {
    SleepQueue->sleep_counter--;

    PCB * current = SleepQueue;
    while (current != NULL && current->sleep_counter == 0) {
      PCB * pcb = current;
      current = current->next;
      pcb->ret = 0;
      ready(pcb);
    }
  }
}
