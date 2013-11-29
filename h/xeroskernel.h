/* xeroskernel.h - disable, enable, halt, restore, isodd, min, max */

/* Symbolic constants used throughout Xinu */

typedef	char		Bool;		/* Boolean type			*/
#define	FALSE		0		/* Boolean constants		*/
#define	TRUE		1
#define	EMPTY		(-1)		/* an illegal gpq		*/
#define	NULL		0		/* Null pointer for linked lists*/
#define	NULLCH		'\0'		/* The null character		*/


/* Universal return constants */

#define	OK		 1		/* system call ok		*/
#define	SYSERR		-1		/* system call failed		*/
#define	EOF		-2		/* End-of-file (usu. from read)	*/
#define	TIMEOUT		-3		/* time out  (usu. recvtim)	*/
#define	INTRMSG		-4		/* keyboard "intr" key pressed	*/
					/*  (usu. defined as ^B)	*/
#define	BLOCKERR	-5		/* non-blocking op would block	*/

/* Functions defined by startup code */


void bzero(void *base, int cnt);
void bcopy(const void *src, void *dest, unsigned int n);
int kprintf(char * fmt, ...);
void lidt(void);
void init8259(void);
void disable(void);
void outb(unsigned int, unsigned char);
unsigned char inb(unsigned int);


#define MAX_PROC        64
#define KERNEL_INT      80
#define TIMER_INT       (TIMER_IRQ + 32)
#define KEYBOARD_INT	(1 + 32) //Keyboard Int = 1, + 32
#define PROC_STACK      (4096 * 4)

#define STATE_STOPPED   0
#define STATE_READY     1
#define STATE_SLEEP     2

#define SYS_STOP        0
#define SYS_YIELD       1
#define SYS_CREATE      2
#define SYS_TIMER       3
#define SYS_KEYBOARD	4
#define SYS_GETPID      8
#define SYS_PUTS        9
#define SYS_SLEEP       10
#define SYS_DEV_OPEN 	11
#define SYS_DEV_CLOSE   12
#define SYS_DEV_WRITE	13
#define SYS_DEV_READ	14
#define SYS_DEV_IOCTL	15

#define DEV_NUM 	2
#define KEYBOARD 	0
#define KEYBOARDECHO	1

#define FDTAB_SIZE	4

typedef void    (*funcptr)(void);

typedef struct struct_pcb pcb;
struct struct_pcb {
  long        esp;
  pcb         *next;
  pcb         *prev;
  int         state;
  int         pid;
  int         otherpid;
  void *      buffer;
  int         bufferlen;
  int         ret;
  int         sleepdiff;
  long        args;
  struct devsw* fdtab[FDTAB_SIZE]; //add var later
};

extern pcb     proctab[MAX_PROC];
#pragma pack(1)

typedef struct context_frame {
  unsigned int        edi;
  unsigned int        esi;
  unsigned int        ebp;
  unsigned int        esp;
  unsigned int        ebx;
  unsigned int        edx;
  unsigned int        ecx;
  unsigned int        eax;
  unsigned int        iret_eip;
  unsigned int        iret_cs;
  unsigned int        eflags;
  unsigned int        stackSlots[0];
} context_frame;


//stuff for devices

struct devsw {
	int dvnum;
	char *dvname;
	unsigned int (*dvinit) (int echo);
	unsigned int (*dvclose) (void);
	unsigned int (*dvread) (void *buf, int len);
	unsigned int (*dvwrite) (void *buf, int len);
	unsigned int (*dvioctl) (unsigned long command);
	int dvminor;
	int inuse;
};

struct devsw devtab[DEV_NUM];
extern pcb      proctab[MAX_PROC];

extern void kmeminit(void);
extern void *kmalloc(int size);
extern void kfree(void *ptr);

unsigned short getCS(void);
void     dispatch( void );
void     dispatchinit( void );
void     ready( pcb *p );
pcb      *next( void );
void     contextinit( void );
int      contextswitch( pcb *p );
int      create( funcptr fp, int stack );
void     set_evec(unsigned int xnum, unsigned long handler);
void     tick(void);
void     sleep(pcb *, int);


extern void     root( void );
void printCF (void * stack);


/* System Calls - probably should be in a different .h file */

int syscall(int call, ...);
int syscreate( funcptr fp, int stack );
int sysyield(void);
int sysstop(void);
int sysgetpid( void );
void sysputs( char *str );
unsigned int syssleep( unsigned int t );
unsigned int sysopen(int device_no);
unsigned  int sysclose(int fd);
unsigned int syswrite(int fd, void *buff, int bufflen);
unsigned int sysread(int fd, void *buff, int bufflen);
unsigned int sysioctl(int fd, unsigned long command, ...);

extern int  di_open(int dev_no, pcb *p);
extern int di_close(int fd, pcb *p);
extern int di_write(int fd, void *buff, int buflen, pcb *p);
extern int di_read(int fd, void *buff, int buflen, pcb *p);
extern int di_ioctl(int fd, unsigned long command, pcb *p);

extern unsigned int kbtoa(unsigned char);
extern int keyboard_isr(void);
extern int keyboard_init(void);
