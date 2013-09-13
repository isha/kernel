	.file	"fprintf.c"
	.text
	.globl	fprintf
	.type	fprintf, @function
fprintf:
.LFB0:
	.cfi_startproc
	subl	$28, %esp
	.cfi_def_cfa_offset 32
	movl	32(%esp), %eax
	movl	%eax, 12(%esp)
	movl	$putc, 8(%esp)
	leal	40(%esp), %eax
	movl	%eax, 4(%esp)
	movl	36(%esp), %eax
	movl	%eax, (%esp)
	call	_doprnt
	movl	$1, %eax
	addl	$28, %esp
	.cfi_def_cfa_offset 4
	ret
	.cfi_endproc
.LFE0:
	.size	fprintf, .-fprintf
	.ident	"GCC: (SUSE Linux) 4.6.2"
	.section	.comment.SUSE.OPTs,"MS",@progbits,1
	.string	"ospWg"
	.section	.note.GNU-stack,"",@progbits
