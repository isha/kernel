/* sleep.c : sleep device (assignment 2)
 */

#include <xeroskernel.h>


#define MILLISECONDS_TICK 10
static pcb	*sleepQ;


// Len is the length of time to sleep

void	sleep( pcb *p, int len ) {
/****************************************/

    pcb	*tmp;


    if( len < 1 ) {
        ready( p );
        return;
    }

    // Convert the length of time to sleep in ticks
    // each tick is 10ms 
    len = len / MILLISECONDS_TICK;

    p->state = STATE_SLEEP;
    p->next = NULL;
    p->prev = NULL;
    if( !sleepQ ) {
        sleepQ = p;
        p->sleepdiff = len;
    } else if( sleepQ->sleepdiff > len ) {
        p->next = sleepQ;
        sleepQ->sleepdiff -= len;
        p->sleepdiff = len;
        sleepQ = p;
    } else {
        len -= sleepQ->sleepdiff;
        for( tmp = sleepQ; tmp->next; tmp = tmp->next ) {
            if( len < tmp->next->sleepdiff ) {
                break;
            } else {
                len -= tmp->next->sleepdiff;
            }
        }

        p->next = tmp->next;
        p->prev = tmp;
        p->sleepdiff = len;
        tmp->next = p;
        if( p->next ) {
            p->next->prev = p;
            p->next->sleepdiff -= len;
        }
    }
}


extern void	tick( void ) {
/****************************/

    pcb	*tmp;

    if( !sleepQ ) {
        return;
    }

    for( sleepQ->sleepdiff--; sleepQ && !sleepQ->sleepdiff; ) {
        tmp = sleepQ;
        sleepQ = tmp->next;

        tmp->state = STATE_READY;
        tmp->next = NULL;
        tmp->prev = NULL;
        tmp->ret = 0;
        ready( tmp );
    }
}
