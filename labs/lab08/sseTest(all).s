	.file	"sseTest.c"
	.intel_syntax noprefix
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC7:
	.string	"|%g %g| * |%g %g| = |%g %g|\n"
.LC8:
	.string	"|%g %g|   |%g %g|   |%g %g|\n"
	.section	.text.startup,"ax",@progbits
	.p2align 4,,15
	.globl	main
	.type	main, @function
main:      #for loop does not actually exist in form of jump
.LFB682:
	.cfi_startproc
	sub	rsp, 104       #rsp = rsp - 104
	.cfi_def_cfa_offset 112
	xorpd	xmm6, xmm6 #xmm6 = xmm6 | xmm6
	movsd	xmm7, QWORD PTR .LC1[rip]  
	movabs	rax, 4616189618054758400
	#for loop starts here I guess
	movsd	xmm0, QWORD PTR .LC0[rip]
	mov	QWORD PTR [rsp+56], rax
	movsd	QWORD PTR [rsp+40], xmm7
	mov	esi, OFFSET FLAT:.LC7
	mov	edi, 1
	movsd	QWORD PTR [rsp+32], xmm0
	mov	eax, 6 # eax = 6
	movsd	QWORD PTR [rsp+64], xmm6
	movsd	QWORD PTR [rsp+72], xmm6
	movsd	QWORD PTR [rsp+80], xmm6
	movsd	QWORD PTR [rsp+88], xmm6   #pointer management stuff
	movsd	xmm1, QWORD PTR .LC2[rip]  #xmm1
	movsd	QWORD PTR [rsp], xmm6
	movupd	xmm4, XMMWORD PTR [rsp+32] #this is load instruction since they are 32 bits xmm4 =  4*A
	movapd	xmm5, XMMWORD PTR .LC5[rip]
	mulpd	xmm5, xmm4    #multiplication between xmm5 & xmm4
	movsd	QWORD PTR [rsp+48], xmm1
	movupd	xmm3, XMMWORD PTR [rsp+64]
	movupd	xmm2, XMMWORD PTR [rsp+80]
	addpd	xmm3, xmm5
	movsd	QWORD PTR [rsp+16], xmm7
	xorpd	xmm5, xmm5  #clears i value
	mulpd	xmm4, xmm5
	addpd	xmm2, xmm4
	#stops here?
	movupd	xmm4, XMMWORD PTR [rsp+48]
	mulpd	xmm5, xmm4
	mulpd	xmm4, XMMWORD PTR .LC6[rip]
	addpd	xmm3, xmm5
	addpd	xmm2, xmm4
	movupd	XMMWORD PTR [rsp+64], xmm3
	movapd	xmm3, xmm6
	movupd	XMMWORD PTR [rsp+80], xmm2
	movapd	xmm2, xmm1
	movsd	xmm5, QWORD PTR [rsp+80]
	movsd	xmm4, QWORD PTR [rsp+64]
	call	__printf_chk
	movsd	xmm7, QWORD PTR [rsp+16]
	mov	esi, OFFSET FLAT:.LC8
	movsd	xmm6, QWORD PTR [rsp]
	mov	edi, 1
	movsd	xmm5, QWORD PTR [rsp+88]
	mov	eax, 6
	movsd	xmm4, QWORD PTR [rsp+72]
	movsd	xmm1, QWORD PTR [rsp+56]
	movapd	xmm3, xmm7
	movsd	xmm0, QWORD PTR [rsp+40]
	movapd	xmm2, xmm6
	call	__printf_chk
	xor	eax, eax
	add	rsp, 104
	.cfi_def_cfa_offset 8
	ret
	.cfi_endproc
.LFE682:   #not here
	.size	main, .-main
	.section	.rodata.cst8,"aM",@progbits,8
	.align 8
.LC0:
	.long	0
	.long	1072693248
	.align 8
.LC1:
	.long	0
	.long	1073741824
	.align 8
.LC2:
	.long	0
	.long	1074266112
	.section	.rodata.cst16,"aM",@progbits,16
	.align 16
.LC5:
	.long	0
	.long	1074266112
	.long	0
	.long	1074266112
	.align 16
.LC6:
	.long	0
	.long	1073741824
	.long	0
	.long	1073741824
	.ident	"GCC: (Ubuntu/Linaro 4.6.3-1ubuntu5) 4.6.3"
	.section	.note.GNU-stack,"",@progbits
