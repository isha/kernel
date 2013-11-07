/* evec.c -- initevec, set_evec */

#include <i386.h>    
#include <xeroslib.h>
#include <xeroskernel.h>

//#include <string.h>

extern struct idt idt[256];
extern struct segtr idtr; 

extern long	defevec[];
extern int	userret(void);

void set_evec(unsigned int xnum, unsigned long handler);


void initevec(void)
{
	int		i;

        /* bzero( idt, sizeof( struct idt ) * 256 ); */
	memset( idt, 0, sizeof( struct idt ) * 256 ); 

	for (i=0; i<NID; ++i)
		set_evec(i, (long)defevec[i]);

	lidt();
	init8259();

}


/*------------------------------------------------------------------------
 * set_evec - set exception vector to point to an exception handler
 *------------------------------------------------------------------------
 */
void set_evec(unsigned int xnum, unsigned long handler)
//unsigned int	xnum;
//unsigned long	handler;	/* int value of function pointer */
{
	struct	idt	*pidt;

	pidt = &idt[xnum];
	pidt->igd_loffset = handler;
	pidt->igd_segsel = 0x8;		/* kernel code segment */
	pidt->igd_mbz = 0;
	pidt->igd_type = IGDT_TRAPG;
	pidt->igd_dpl = 0;
	pidt->igd_present = 1;
	pidt->igd_hoffset = handler >> 16;
}

char *inames[17] = {
	"divided by zero",
	"debug exception",
	"NMI interrupt",
	"breakpoint",
	"overflow",
	"bounds check failed",
	"invalid opcode",
	"coprocessor not available",
	"double fault",
	"coprocessor segment overrun",
	"invalid TSS",
	"segment not present",
	"stack fault",
	"general protection violation",
	"page fault",
	"coprocessor error",
};

static long *fp; // This is the fp that is used in the asm
                 // "instruction" within trap.

/*------------------------------------------------------------------------
 * trap -- print some debugging info when a trap occurred 
 * Note: Need some more work.
 *------------------------------------------------------------------------
 */

#define ELEMENTS_ON_STACK 14

void trap(int inum)
{
  long		*sp;

	disable();

	
	/* The following copies the current value of the frame pointer into
         * the global variable fp. What this means is that fp is pointing 
         * to the current frame. 
         */
	asm("movl	%ebp,fp");
      
	sp = fp;	/* eflags/CS/eip/ebp/regs/trap#/Xtrap/ebp */

	kprintf("trap!\n");
	if (inum < 16) {
		kprintf("exception %d (%s)\n", inum, inames[inum] );
	} else {
		kprintf("exception %d\n", inum );
	}

	/* We are going to use the variable sp to walk the stack frame 
         * to print out information with respect to regsiter values etc
         * at the time the trap/exception occurred. 
	 */
	sp = sp + ELEMENTS_ON_STACK; 
	kprintf("Address of sp %x value of sp %x\n", &sp, sp);
	kprintf("eflags %X ", *sp); sp--;
        kprintf("CS: %X ", *sp);    sp--;
	kprintf("eip %X\n", *sp);   sp--;

	if (inum == 8 || (inum >= 10 && inum <= 14)) {
		kprintf("error code %08x (%u)\n", *sp, *sp);
		sp--;
	}
	sp--;	/* skip FP for Xint call */
	kprintf("register dump:\n");

	kprintf("eax %08X (%u)\n", *sp, *sp); sp--;
	kprintf("ecx %08X (%u)\n", *sp, *sp); sp--;
	kprintf("edx %08X (%u)\n", *sp, *sp); sp--;
	kprintf("ebx %08X (%u)\n", *sp, *sp); sp--;
	kprintf("esp %08X (%u)\n", *sp, *sp); sp--;
	kprintf("ebp %08X (%u)\n", *sp, *sp); sp--;
	kprintf("esi %08X (%u)\n", *sp, *sp); sp--;
	kprintf("edi %08X (%u)\n", *sp, *sp); sp--;

	kprintf("\nHalting.....\n");
        for(;;);
}
