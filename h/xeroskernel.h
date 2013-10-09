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
  long esp;
} PCB;

extern PCB * ReadyQueue;

typedef enum {
  CREATE,
  YIELD,
  STOP
} RequestType;

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

extern void dispatch();
extern int syscall(RequestType call, ... );
extern int syscreate (void (*func)(), int stack);
extern void sysyield (void);
extern void sysstop (void);
