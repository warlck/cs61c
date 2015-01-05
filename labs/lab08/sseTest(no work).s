	.file	"sseTest.c"
	.intel_syntax noprefix
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC2:
	.string	"|%g %g| * |%g %g| = |%g %g|\n"
.LC6:
	.string	"|%g %g|   |%g %g|   |%g %g|\n"
	.section	.text.startup,"ax",@progbits
	.p2align 4,,15
	.globl	main
	.type	main, @function
main:
.LFB682:
	.cfi_startproc
	xorpd	xmm6, xmm6
	sub	rsp, 56
	.cfi_def_cfa_offset 64
	mov	esi, OFFSET FLAT:.LC2
	mov	edi, 1
	mov	eax, 6
	movsd	QWORD PTR [rsp+16], xmm6
	movapd	xmm3, xmm6
	movsd	QWORD PTR [rsp+32], xmm6
	movsd	QWORD PTR [rsp+24], xmm6
	movsd	QWORD PTR [rsp+40], xmm6
	movsd	QWORD PTR [rsp], xmm6
	movupd	xmm1, XMMWORD PTR [rsp+16]
	movupd	xmm0, XMMWORD PTR [rsp+32]
	movupd	XMMWORD PTR [rsp+16], xmm1
	movupd	XMMWORD PTR [rsp+32], xmm0
	movsd	xmm1, QWORD PTR .LC1[rip]
	movsd	xmm5, QWORD PTR [rsp+32]
	movsd	xmm4, QWORD PTR [rsp+16]
	movapd	xmm2, xmm1
	movsd	xmm0, QWORD PTR .LC3[rip]
	call	__printf_chk
	movsd	xmm6, QWORD PTR [rsp]
	mov	esi, OFFSET FLAT:.LC6
	movsd	xmm0, QWORD PTR .LC4[rip]
	mov	edi, 1
	movsd	xmm5, QWORD PTR [rsp+40]
	mov	eax, 6
	movsd	xmm4, QWORD PTR [rsp+24]
	movapd	xmm3, xmm0
	movapd	xmm2, xmm6
	movsd	xmm1, QWORD PTR .LC5[rip]
	call	__printf_chk
	xor	eax, eax
	add	rsp, 56
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE682:
	.size	main, .-main
	.section	.rodata.cst8,"aM",@progbits,8
	.align 8
.LC1:
	.long	0
	.long	1074266112
	.align 8
.LC3:
	.long	0
	.long	1072693248
	.align 8
.LC4:
	.long	0
	.long	1073741824
	.align 8
.LC5:
	.long	0
	.long	1074790400
	.ident	"GCC: (Ubuntu/Linaro 4.6.3-1ubuntu5) 4.6.3"
	.section	.note.GNU-stack,"",@progbits
