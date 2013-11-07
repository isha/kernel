/*-
 * Copyright (c) 1990 The Regents of the University of California.
 * All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * William Jolitz.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by the University of
 *	California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *	@(#)srt0.c	5.3 (Berkeley) 4/28/91
 */

/*
 * Startup code for standalone system
 * Non-relocating version -- for programs which are loaded by boot
 * Relocating version for boot
 * Small relocating version for "micro" boot
 */

#define	B_MAGIC			0
#define	BOOT_MAGIC	0xB00DEF01
#define	B_LEN			4

	.data
	.globl	_gdt
	.globl	gdt
	.globl	_idt
	.globl	idt
	.globl	_idtr
	.globl	idtr
_gdt: 
gdt:	.space	64	# must equal NGD*8 (64 = 8 segments)
gdtr:	.word	63	# sizeof _gdt -1 (in bytes)
	.long	gdt
_idt: 
idt:	.space	2048	# must equal 256*8 (2048 == 256 vectors)
_idtr: 
idtr:	.word	2047	# size of _idt -1 (in bytes)
	.long	idt

	.globl _cpudelay
	.globl cpudelay

_cpudelay: 
cpudelay:	.long	1

	.text
	.align 2
	.globl	_end
	.globl	_edata
	.globl	_bootdev
	.globl	_cyloffset
	.globl	_lidt
	.globl	lidt

	.globl	__zero			# artificial value for entry point
	.set	__zero,0


_entry:
entry:	
	.globl	_entry
	.globl	entry
	.globl	_start
	.globl	start

	movl	$BRELOC,%esi    # Move from BRELOC to 
	movl	$0,%edi		# to relocated area
	movl	$300*1024,%ecx  # Size to Move (should be _edata)
	cld
	rep
	movsb
	# relocate program counter to relocation base
	movl	$BRELOC+360*1024, %esp	#Put Stack pointer somewhere safe.  
	pushl	$start
	ret

_start:
start:
	/* setup stack pointer */
	movl	%esp,%esi

	/*
	 * Clear Bss and up to 64K heap
	 */
	movl	$64*1024,%ebx
	movl	$_end,%eax	# should be movl $_end-_edata but ...
	subl	$_edata,%eax
	addl	%ebx,%eax
	pushl	%eax
	pushl	$_edata
	call	bzero

	/*
	 * Clear 64K of stack
	 */
	movl	%esi,%eax
	subl	%ebx,%eax
	subl	$5*4,%ebx
	pushl	%ebx
	pushl	%eax
	call	bzero

	movl	%esi,%esp

	pushl	$0
	popf

	movl	%esp,%ebp

	call	setsegs

	lgdt	gdtr
	/*
	 * reload segment registers; we do code segment by a far
	 * jump
	 */
	ljmp	$0x8,$gdt1	/* CS descriptor 1 */
gdt1:
	movl	$0x10,%eax	/* DS descriptor 2 */
	movw	%ax,%ds
	movw	%ax,%es
	movl	$0x18,%eax	/* SS descriptor 3 */
	movw	%ax,%ss

	movl	initsp,%esp	/* set new stack pointer base */

	movl	%esp,%ebp

        call    initevec        /* initialize interrupt table */
	call	initproc        /* start Xeros */
        call    halt            /* Die a miserable death if I get here */


	.data
_bootdev:	.long	0
_cyloffset:	.long	0
savearea:	.long	0,0	# sp & bp to return to
	.text

	.globl	_inb
	.globl	inb
_inb:
inb:
	movl	4(%esp),%edx
	xorl	%eax,%eax	# clr eax
	inb	%dx,%al
	ret

	.globl	_inw
	.globl	inw
_inw:
inw:
	movl	4(%esp),%edx
	xorl	%eax,%eax	# clr eax
	inw	%dx,%ax
	ret

	.globl	_outb
	.globl	outb
_outb:
outb:
	movl	4(%esp),%edx
	movl	8(%esp),%eax
	outb	%al,%dx
	ret

	.globl	_outw
	.globl	outw
_outw:	
outw:	
	movl	4(%esp),%edx
	movl	8(%esp),%eax
	outw	%ax,%dx
	ret

#ifndef SMALL
	.globl	_rtcin
_rtcin:	movl	4(%esp),%eax
	outb	%al,$0x70
	subl	%eax,%eax	# clr eax
	inb	$0x71,%al
	ret
#endif

	.globl ___udivsi3
___udivsi3:
	movl 4(%esp),%eax
	xorl %edx,%edx
	divl 8(%esp)
	ret

	.globl ___divsi3
___divsi3:
	movl 4(%esp),%eax
	xorl %edx,%edx
	cltd
	idivl 8(%esp)
	ret

	#
	# bzero (base,cnt)
	#

	.globl _bzero
	.globl bzero
_bzero:
bzero:
	pushl	%edi
	movl	8(%esp),%edi
	movl	12(%esp),%ecx
	movb	$0x00,%al
	cld
	rep
	stosb
	popl	%edi
	ret

	#
	# bcopy(src, dst, count)
	#

	.globl	_bcopy
_bcopy:
bcopy:
	pushl	%esi
	pushl	%edi
	movl	12(%esp),%esi
	movl	16(%esp),%edi
L1:
	movl	20(%esp),%ecx
	cld
	rep
	movsb
	popl	%edi
	popl	%esi
	ret

	# insw(port,addr,cnt)
	.globl	_insw
	.globl	insw
_insw:
insw:
	pushl	%edi
	movw	8(%esp),%dx
	movl	12(%esp),%edi
	movl	16(%esp),%ecx
	cld
	.byte 0x66,0xf2,0x6d	# rep insw
	movl	%edi,%eax
	popl	%edi
	ret

	# outsw(port,addr,cnt)
	.globl	_outsw
	.globl	outsw
_outsw:
outsw:
	pushl	%esi
	movw	8(%esp),%dx
	movl	12(%esp),%esi
	movl	16(%esp),%ecx
	cld
	.byte 0x66,0xf2,0x6f	# rep outsw
	movl	%esi,%eax
	popl	%esi
	ret

	#
	# lidt() - load interrupt descriptor table from idtr
	#
_lidt:
lidt:
	lidt	idtr
	ret

	.globl  _jumptocode
	.globl  jumptocode

_jumptocode:
jumptocode:
	ljmp	$0x20,$0x100000	/* CS descriptor 1 */

	.globl _jumptobootcode
	.globl jumptobootcode

_jumptobootcode:
jumptobootcode:
        ljmp    $0x20,$BOOTPLOC /* CS descriptor 1 */
