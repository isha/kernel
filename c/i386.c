/* i386.c - setsegs */

#include <icu.h>
#include <i386.h>
#include <xeroslib.h>
#include <xeroskernel.h>


#define BOOTP_CODE
#define	KCODE	1
#define	KSTACK	2
#define	KDATA	3

void enable_irq( unsigned int,  int);


struct sd gdt_copy[NGD] = {
		/* 0th entry NULL */
	{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, 
		/* 1st, Kernel Code Segment */
	{ 0xffff, 0, 0, 6, 1, 1, 0, 1, 0xf, 0, 0, 1, 1, 0 },
		/* 2nd, Kernel Data Segment */
	{ 0xffff, 0, 0, 2, 0, 1, 0, 1, 0xf, 0, 0, 1, 1, 0 },
		/* 3rd, Kernel Stack Segment */
	{ 0xffff, 0, 0, 2, 0, 1, 0, 1, 0xf, 0, 0, 1, 1, 0 },
		/* 4st, Bootp Code Segment */
	{ 0xffff, 0, 0, 6, 1, 1, 0, 1, 0xf, 0, 0, 1, 1, 0 },

};

extern struct sd gdt[];

long	initsp;		/* initial SP for init() */
long    freemem;        /* start of free memory */
char	*maxaddr;       /* end of memory space */



/*------------------------------------------------------------------------
 * sizmem - return memory size (in pages)
 *------------------------------------------------------------------------
 */
long sizmem(void)
{
        return 1024;
}


/*------------------------------------------------------------------------
 * setsegs - initialize the 386 processor
 *------------------------------------------------------------------------
 */
void setsegs(void)
{
	extern int	etext, end;
	struct sd	*psd;
	unsigned int	np, npages;

	npages = sizmem();
	maxaddr = (char *)(npages * NBPG - 1);

	psd = &gdt_copy[1];	/* kernel code segment */
	np = ((int)&etext + NBPG-1) / NBPG;	/* # code pages */
	psd->sd_lolimit = np;
	psd->sd_hilimit = np >> 16;

	psd = &gdt_copy[2];	/* kernel data segment */
	psd->sd_lolimit = npages;
	psd->sd_hilimit = npages >> 16;

	psd = &gdt_copy[3];	/* kernel stack segment */
	psd->sd_lolimit = npages;
	psd->sd_hilimit = npages >> 16;

	psd = &gdt_copy[4];	/* bootp code segment */
	psd->sd_lolimit = npages;   /* Allows execution of 0x100000 CODE */
	psd->sd_hilimit = npages >> 16;

	blkcopy(gdt, gdt_copy, sizeof(gdt_copy));
	initsp = npages*NBPG  - 4;

        /* let's move the stack a bit closer to home */
        initsp = (int)&end + KERNEL_STACK - 4;
        freemem = (int)&end + KERNEL_STACK;
}

/*------------------------------------------------------------------------
 * init8259 - initialize the 8259A interrupt controllers
 *------------------------------------------------------------------------
 */
void init8259()
{
	/* Master device */
	outb(ICU1, 0x11);	/* ICW1: icw4 needed		*/
	outb(ICU1+1, 0x20);	/* ICW2: base ivec 32		*/
	outb(ICU1+1, 0x4);	/* ICW3: cascade on irq2	*/
	outb(ICU1+1, 0x1);	/* ICW4: buf. master, 808x mode */
	outb(ICU1, 0xb);	/* OCW3: set ISR on read	*/

	/* Slave device */
	outb(ICU2, 0x11);	/* ICW1: icw4 needed		*/
	outb(ICU2+1, 0x28);	/* ICW2: base ivec 40		*/
	outb(ICU2+1, 0x2);	/* ICW3: slave on irq2		*/
	outb(ICU2+1, 0xb);	/* ICW4: buf. slave, 808x mode	*/
	outb(ICU2, 0xb);	/* OCW3: set ISR on read	*/

	disable();
} 


/*------------------------------------------------------------------------
 * initPIT - initialize the programmable interval timer
 *------------------------------------------------------------------------
 */
void initPIT( int divisor )
{
        outb( TIMER_MODE, TIMER_SEL0 | TIMER_RATEGEN | TIMER_16BIT );
        outb( TIMER_1_PORT, TIMER_DIV(divisor) & 0xff );
        outb( TIMER_1_PORT, TIMER_DIV(divisor) >> 8 );
        enable_irq( TIMER_IRQ, 0 );
}


/*------------------------------------------------------------------------
 * end_of_intr - signal EOI to rearm hardware interrupts
 *------------------------------------------------------------------------
 */
void end_of_intr(void)
{
        outb( ICU1, 0x20 );
        outb( ICU2, 0x20 );
}


/*------------------------------------------------------------------------
 * enable_irq - enable/disable hardware irq
 *------------------------------------------------------------------------
 */
void enable_irq( unsigned int irq, int disable )
{
    unsigned int        port;
    unsigned char       val;

    if( irq < 8 ) {
        port = ICU1 + 1;
    } else {
        port = ICU2 + 1;
        irq -= 8;
    }

    val = inb( port );

    if( disable ) {
        val |= ( 1 << irq );
    } else {
        val &= ~( 1 << irq );
    }

    outb( port, val );
}


/*------------------------------------------------------------------------
 * getCS - returns current CS selector
 *------------------------------------------------------------------------
 */
extern unsigned short getCS( void )
{
    unsigned short	cs;

    __asm __volatile( " \
	movw	%%cs, %%ax \n\
	movw	%%ax, %0 \n\
	"  
	: "=g" (cs)
	: 
	: "%eax"
    );

    return( cs );
}


/*------------------------------------------------------------------------
 * getDS - returns current DS selector
 *------------------------------------------------------------------------
 */
extern unsigned short getDS( void )
{
    unsigned short	ds;

    __asm __volatile( " \
	movw	%%ds, %%ax \n\
	movw	%%ax, %0 \
	" 
	: "=g" (ds)
	: 
	: "%eax"
    );

    return( ds );
}


/*
pseg(psd)
struct sd	*psd;
{
	int		i;
	unsigned char	*pb = (unsigned char *)psd;
	unsigned int	base;
	unsigned int	limit;

	kprintf("pseg (%X)\n", pb);
	kprintf("hex:   %02x", (unsigned int)*pb++);
	for(i=1; i<8; ++i) {
		kprintf(" (%X)", pb);
		kprintf(" %02x", (unsigned int)*pb++);
	}
	limit = ((int)psd->sd_hilimit) << 16;
	limit |= psd->sd_lolimit;
	base = ((int)psd->sd_hibase) << 24;
	base |= ((int)psd->sd_midbase)<< 16;
	base |= psd->sd_lobase;
	
	kprintf("\nby field: base %X limit %d perm %d\n", base, limit,
		psd->sd_perm);
	kprintf("iscode %d isapp %d dpl %d present %d avl %d\n",
		psd->sd_iscode, psd->sd_isapp, psd->sd_dpl, psd->sd_present,
		psd->sd_avl);
	kprintf("mbz %d 32b %d gran %d\n", psd->sd_mbz, psd->sd_32b,
		psd->sd_gran);
}*/
