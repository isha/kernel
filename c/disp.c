/* disp.c : dispatcher
 */

#include <xeroskernel.h>
#include <xeroslib.h>
#include <stdarg.h>


static pcb      *head = NULL;
static pcb      *tail = NULL;

void     dispatch( void ) {
/********************************/

    pcb         *p;
    int         r;
    funcptr     fp;
    int         stack;
    va_list     ap;

    for( p = next(); p; ) {
      //      kprintf("Process %x selected stck %x\n", p, p->esp);

      r = contextswitch( p );
      switch( r ) {
      case( SYS_CREATE ):
        ap = (va_list)p->args;
        fp = (funcptr)(va_arg( ap, int ) );
        stack = va_arg( ap, int );
        p->ret = create( fp, stack );
        break;
      case( SYS_YIELD ):
        ready( p );
        p = next();
        break;
      case( SYS_STOP ):
        p->state = STATE_STOPPED;
        p = next();
        break;
      default:
        kprintf( "Bad Sys request %d, pid = %d\n", r, p->pid );
      }
    }

    kprintf( "Out of processes: dying\n" );
    
    for( ;; );
}

extern void dispatchinit( void ) {
/********************************/

  //bzero( proctab, sizeof( pcb ) * MAX_PROC );
  memset(proctab, 0, sizeof( pcb ) * MAX_PROC);
}

extern void     ready( pcb *p ) {
/*******************************/

    p->next = NULL;
    p->state = STATE_READY;

    if( tail ) {
        tail->next = p;
    } else {
        head = p;
    }

    tail = p;
}

extern pcb      *next( void ) {
/*****************************/

    pcb *p;

    p = head;

    if( p ) {
        head = p->next;
        if( !head ) {
            tail = NULL;
        }
    }

    return( p );
}

