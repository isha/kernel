/* intr.s - disable(), restore(), enable(), pause(), halt() */


#include <icu.s> 

	.text
	.globl	_enable
	.globl	enable
	.globl	_disable
	.globl	disable
	.globl	_restore
	.globl	restore
	.globl	_pause
	.globl	pause
	.globl	_halt
	.globl	halt
	.globl	_getirmask
	.globl	getirmask

/*------------------------------------------------------------------------
 * Xtrap - trap entry
 *------------------------------------------------------------------------
 */
Xtrap:
	call	trap

	movb	$OCW1_2,%al
	outb	%al,$EOI
	popl	%eax		/* exception # */
	testl	$35,%eax
	jb	3f
	shrl	$5,%eax		/* convert to IRQ # */

	/* issue global rearm to cards */
	testl	$7,%eax
	ja	1f
	orl	$0x2f0,%eax	/* 0x2fX for IRQ 3-7 */
	movb	%bl,(%eax)
	jmp	3f
1:	testl	$9,%eax
	jz	2f
	testl	$15,%eax
	ja	3f
	orl	$0x6f0,%eax	/* 0x2f(X-8) for IRQ 10-15 */
	movb	%bl,(%eax)
	jmp	3f
2:	movb	%al,0x2f2	/* 0x2f2 for IRQ 9 */
3:
	popal
	leave
	iret

/*------------------------------------------------------------------------
 * _Xint? - default exception and interrupt entry points
 *	NB: These must be contiguous
 *------------------------------------------------------------------------
 */
#include "../c/xint.s"

		.globl	_defevec
		.globl	defevec
_defevec:
defevec:
		.long	_Xint0
		.long	_Xint1
		.long	_Xint2
		.long	_Xint3
		.long	_Xint4
		.long	_Xint5
		.long	_Xint6
		.long	_Xint7
		.long	_Xint8
		.long	_Xint9
		.long	_Xint10
		.long	_Xint11
		.long	_Xint12
		.long	_Xint13
		.long	_Xint14
		.long	_Xint15
		.long	_Xint16
		.long	_Xint17
		.long	_Xint18
		.long	_Xint19
		.long	_Xint20
		.long	_Xint21
		.long	_Xint22
		.long	_Xint23
		.long	_Xint24
		.long	_Xint25
		.long	_Xint26
		.long	_Xint27
		.long	_Xint28
		.long	_Xint29
		.long	_Xint30
		.long	_Xint31
		.long	_Xint32
		.long	_Xint33
		.long	_Xint34
		.long	_Xint35
		.long	_Xint36
		.long	_Xint37
		.long	_Xint38
		.long	_Xint39
		.long	_Xint40
		.long	_Xint41
		.long	_Xint42
		.long	_Xint43
		.long	_Xint44
		.long	_Xint45
		.long	_Xint46
		.long	_Xint47


/*---------------------------------------------------------
 * halt: do nothing forever
 *---------------------------------------------------------
 */
_halt:
halt:
	jmp halt
	ret

/*---------------------------------------------------------
 * disable    - disable interrupts
 *---------------------------------------------------------
 */
_disable:
disable:
	cli
	pushfl
	movb	$0xff,%al
	outb	%al,$IMR2
	movb	$0xff,%al
	outb	%al,$IMR1
	popfl
	ret
