/* create.c : create a process
 */

#include <xeroskernel.h>
typedef enum {
  READY,
  BLOCKED,
  STOPPED,
  RUNNING
} ProcessState;

typedef struct {
  int pid;
  ProcessState state;
  int priority;
  struct PCB * next;
} PCB;

extern PCB * ReadyQueue;
extern PCB * Running;

/* Your code goes here. */
