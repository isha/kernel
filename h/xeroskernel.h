/* xeroskernel.h - disable, enable, halt, restore, isodd, min, max */

/* Symbolic constants used throughout Xinu */

typedef char    Bool;   /* Boolean type     */
#define FALSE   0   /* Boolean constants    */
#define TRUE    1
#define EMPTY   (-1)    /* an illegal gpq   */
#define NULL    0   /* Null pointer for linked lists*/
#define NULLCH    '\0'    /* The null character   */


/* Universal return constants */

#define OK     1    /* system call ok   */
#define SYSERR    -1    /* system call failed   */
#define EOF   -2    /* End-of-file (usu. from read) */
#define TIMEOUT   -3    /* time out  (usu. recvtim) */
#define INTRMSG   -4    /* keyboard "intr" key pressed  */
          /*  (usu. defined as ^B)  */
#define BLOCKERR  -5    /* non-blocking op would block  */

/* Functions defined by startup code */


void bzero(void *base, int cnt);
void bcopy(const void *src, void *dest, unsigned int n);
int kprintf(char * fmt, ...);
void lidt(void);
void init8259(void);
void disable(void);
void outb(unsigned int, unsigned char);
unsigned char inb(unsigned int);

/* Memory manager */
extern void kmeminit( void );
extern void *kmalloc( int );
extern void kfree( void *);

/* Different states that a process can be in */
typedef enum {
  READY,
  BLOCKED,
  STOPPED,
  RUNNING,
  SLEEPING
} ProcessState;

/* Message buffer associated with a process */
typedef struct {
  unsigned int pid;
  void * address;
  unsigned int size;
} MessageBuffer;

/* Process control block structure */
typedef struct {
  int pid;
  ProcessState state;
  int priority;
  struct PCB * next;
  long esp;
  long args;
  int ret;
  struct PCB * message_queue;
  MessageBuffer message_buffer;
  int sleep_counter;
} PCB;

/* Queues to keep track of the ready and blocked processes */
extern PCB * ReadyQueue;
extern PCB * BlockedQueue;

/* Handlers for the above queues */
extern void ready(PCB *);
extern void block(PCB *);

/* Context frame structure used to initialize a new process' stack */
typedef struct {
  unsigned int edi;
  unsigned int esi;
  unsigned int ebp;
  unsigned int esp;
  unsigned int ebx;
  unsigned int edx;
  unsigned int ecx;
  unsigned int eax;
  unsigned int eip;
  unsigned int cs;
  unsigned int eflags;
} ContextFrame;

/* Types of requests to the cpu */
typedef enum {
  CREATE = 0,
  YIELD =  1,
  STOP = 2,
  GET_PID = 3,
  PUTS = 4,
  SEND = 5,
  RECV = 6,
  TIMER_INT = 7,
  SLEEP = 8
} RequestType;


extern void dispatch();

/* System calls */
extern int syscall(RequestType call, ... );
extern int syscreate (void (*func)(), int stack);
extern void sysyield (void);
extern void sysstop (void);
extern unsigned int sysgetpid (void);
extern void sysputs (char *);
extern int syssend (unsigned int, void *, int);
extern int sysrecv (unsigned int *, void *, int);
extern int syssleep (unsigned int);

/* Return value constants for IPC methods */
#define PROCESS_NOT_FOUND -1
#define CIRCULAR_IPC_ERROR -2
#define OTHER_IPC_ERROR -3
#define IPC_BLOCKED -4

/* Sleeping */
extern PCB * SleepQueue;
extern void tick (void);
extern void sleep (PCB *, unsigned int);

/* User process declarations */
extern void root (void);
extern void idleproc (void);
extern unsigned int IDLE_PROC_PID;
