/* disp.c : dispatcher
 */
#include <xeroskernel.h>
#include <i386.h>
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
    char        *str;
    int         len;
    int       	fd;
    int       	dev_no;
    void        *buf;
    unsigned long command;
    int         sig;

    for( p = next(); p; ) {
      //      kprintf("Process %x selected stck %x\n", p, p->esp);
      
      service_signals(p);
       
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
      case( SYS_PUTS ):
	      ap = (va_list)p->args;
	      str = va_arg( ap, char * );
	      kprintf( "%s", str );
	      p->ret = 0;
	      break;
      case( SYS_GETPID ):
	      p->ret = p->pid;
	      break;
      case( SYS_SLEEP ):
	      ap = (va_list)p->args;
	      len = va_arg( ap, int );
	      sleep( p, len );
	      p = next();
	      break;
      case (SYS_KEYBOARD ):
	      keyboard_isr();
	      end_of_intr();
      case( SYS_TIMER ):
	      tick();
	      ready( p );
	      p = next();
	      end_of_intr();
	      break;
      case ( SYS_DEV_OPEN ) :
	      ap = (va_list)p->args;
 	      dev_no = va_arg(ap, int);
	      p->ret = di_open(dev_no, p);
	      break;
      case ( SYS_DEV_CLOSE) :
	      ap = (va_list)p->args;
	      fd = va_arg(ap, int);
	      p->ret = di_close(fd, p);
	      break;
      case ( SYS_DEV_WRITE) :
	      ap = (va_list)p->args;
	      fd = va_arg(ap, int);
	      buf = va_arg(ap, void *);
	      len = va_arg(ap, int);
	      p->ret = di_write(fd, buf, len, p);
	      break;
      case ( SYS_DEV_READ) :
	      ap = (va_list)p->args;
	      fd = va_arg(ap, int);
	      buf = va_arg(ap, void *);
	      len = va_arg(ap, int);
	      p->ret = di_read(fd, buf, len, p);
	      break;
      case (SYS_DEV_IOCTL) :
	      ap = (va_list)p->args;
	      fd = va_arg(ap, int);
	      command = va_arg(ap, unsigned long);
	      p->ret = di_ioctl(fd, command, p);
	      break;
      case( SYS_SIGHANDLER ):
        ap = (va_list) p->args;
        sig = va_arg(ap, int);
        
        if (sig > 31 || sig < 0) { p->ret = -1; break; }

        void * newhandler = (void *) (va_arg(ap, unsigned int)); 
        void ** oldhandler = (void *) (va_arg(ap, unsigned int*));
        
        *oldhandler = &p->signal_table[sig];
        p->signal_table[sig] = newhandler;
        p->signal_register_mask = (1 << sig) | p->signal_register_mask;
        
        p->ret = 0;
        break;
      case( SYS_SIGWAIT ):
        p->state = STATE_WAIT;
        p = next();
        break;
      case( SYS_KILL ):
        ap = (va_list) p->args;
        int pid = va_arg(ap, int);
        sig = va_arg(ap, int);

        if (sig > 31 || sig < 0) { p->ret = -12; break; }
        pcb * sig_pcb = findPCB(pid);
        if (!sig_pcb) { p->ret = -33; break; }
         
        p->ret = signal(pid, sig);
        
        if (sig_pcb->state == STATE_WAIT) { ready(sig_pcb); }
        break;
      case( SYS_SIGRETURN ):
        ap = (va_list) p->args;
        unsigned int old_sp = (unsigned int) (va_arg(ap, void*));
        p->esp = old_sp;
        p->ret = 0;
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
    } else {
        kprintf( "BAD\n" );
        for(;;);
    }

    p->next = NULL;
    p->prev = NULL;
    return( p );
}


extern pcb *findPCB( int pid ) {
/******************************/

    int	i;

    for( i = 0; i < MAX_PROC; i++ ) {
        if( proctab[i].pid == pid ) {
            return( &proctab[i] );
        }
    }

    return( NULL );
}

