/* create.c : create a process
 */

#include <xeroskernel.h>
#include <xeroslib.h>

pcb     proctab[MAX_PROC];

/* make sure interrupts are armed later on in the kernel development
 */
#define STARTING_EFLAGS         0x00003000
#define ARM_INTERRUPTS          0x00000200

//#define ARM_INTERRUPTS          0x00000000

static int      nextpid = 1;



int      create( funcptr fp, int stack ) {
/***********************************************/

    context_frame       *cf;
    pcb                 *p = NULL;
    int                 i;

    if( stack < PROC_STACK ) {
        stack = PROC_STACK;
    }

    for( i = 0; i < MAX_PROC; i++ ) {
        if( proctab[i].state == STATE_STOPPED ) {
            p = &proctab[i];
            break;
        }
    }

    if( !p ) {
        return( -1 );
    }


    cf = kmalloc( stack*2);
    if( !cf ) {
        return( -1 );
    }

    cf = (context_frame *)((int)cf + stack - 4);
    cf--;

    memset(cf, 0x81, sizeof( context_frame ));

    cf->iret_cs = getCS();
    cf->iret_eip = (unsigned int)fp;
    cf->eflags = STARTING_EFLAGS | ARM_INTERRUPTS;

    cf->esp = (int)(cf + 1);
    cf->ebp = cf->esp;
    p->esp = (int)cf;
    p->state = STATE_READY;
    p->pid = nextpid++;

    ready( p );
    return( p->pid );
}
