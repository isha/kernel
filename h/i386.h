/* i386.h - DELAY */

#define	NBPG		4096
#define KERNEL_STACK	(4*4096)    


#define	NID		48
#define	NGD		 8

#define	IRQBASE		32	/* base ivec for IRQ0			*/

struct idt {
	unsigned short	igd_loffset;
	unsigned short	igd_segsel;
	unsigned int	igd_rsvd : 5;
	unsigned int	igd_mbz : 3;
	unsigned int	igd_type : 5;
	unsigned int	igd_dpl : 2;
	unsigned int	igd_present : 1;
	unsigned short	igd_hoffset;
};

#define	IGDT_TASK	 5	/* task gate IDT descriptor		*/
#define	IGDT_INTR	14	/* interrupt gate IDT descriptor	*/
#define	IGDT_TRAPG	15	/* Trap Gate				*/


/* Segment Descriptor */

struct sd {
	unsigned short	sd_lolimit;
	unsigned short	sd_lobase;
	unsigned char	sd_midbase;
	unsigned int	sd_perm : 3;
	unsigned int	sd_iscode : 1;
	unsigned int	sd_isapp : 1;
	unsigned int	sd_dpl : 2;
	unsigned int	sd_present : 1;
	unsigned int	sd_hilimit : 4;
	unsigned int	sd_avl : 1;
	unsigned int	sd_mbz : 1;		/* must be '0' */
	unsigned int	sd_32b : 1;
	unsigned int	sd_gran : 1;
	unsigned char	sd_hibase;
};

#define	sd_type		sd_perm

/* System Descriptor Types */

#define	SDT_INTG	14	/* Interrupt Gate	*/

/* Segment Table Register */
struct segtr {
	unsigned int	len : 16;
	unsigned int	addr : 32;
};

/*
 * Delay units are in microseconds.
 */
#define	DELAY(n)					\
{							\
        extern int cpudelay;				\
        register int i;					\
	register long N = (((n)<<4) >> cpudelay);	\
 							\
	for (i=0;i<=4;i++)				\
	   {						\
	   N = (((n) << 4) >> cpudelay);		\
	   while (--N > 0) ;				\
	   }						\
}

#define HOLESIZE        (600)
#define HOLESTART       (640 * 1024)
#define HOLEEND         ((1024 + HOLESIZE) * 1024)
/* Extra 600 for bootp loading, and monitor */

/* Code grokked from cs452 (waterloo) libs
 */
#define TIMER_IRQ	0	/* IRQ of counter 0 on timer 1 */
#define TIMER_1_PORT    0x040   /* 8253 Timer #1 */
#define TIMER_2_PORT    0x048   /* 8253 Timer #2 (EISA only) */

#ifndef TIMER_FREQ
#define TIMER_FREQ      1193182
#endif
#define TIMER_DIV(x) ((TIMER_FREQ+(x)/2)/(x))

/*
 * Macros for specifying values to be written into a mode register.
 */
#define TIMER_CNTR0     (TIMER_1_PORT + 0) /* timer 0 counter port */
#define TIMER_CNTR1     (TIMER_1_PORT + 1) /* timer 1 counter port */
#define TIMER_CNTR2     (TIMER_1_PORT + 2) /* timer 2 counter port */
#define TIMER_MODE      (TIMER_1_PORT + 3) /* timer mode port */
#define         TIMER_SEL0      0x00    /* select counter 0 */
#define         TIMER_SEL1      0x40    /* select counter 1 */
#define         TIMER_SEL2      0x80    /* select counter 2 */
#define         TIMER_INTTC     0x00    /* mode 0, intr on terminal cnt */
#define         TIMER_ONESHOT   0x02    /* mode 1, one shot */
#define         TIMER_RATEGEN   0x04    /* mode 2, rate generator */
#define         TIMER_SQWAVE    0x06    /* mode 3, square wave */
#define         TIMER_SWSTROBE  0x08    /* mode 4, s/w triggered strobe */
#define         TIMER_HWSTROBE  0x0a    /* mode 5, h/w triggered strobe */
#define         TIMER_LATCH     0x00    /* latch counter for reading */
#define         TIMER_LSB       0x10    /* r/w counter LSB */
#define         TIMER_MSB       0x20    /* r/w counter MSB */
#define         TIMER_16BIT     0x30    /* r/w counter 16 bits, LSB first */
#define         TIMER_BCD       0x01    /* count in BCD */


void initPIT( int divisor );
void end_of_intr( void );
