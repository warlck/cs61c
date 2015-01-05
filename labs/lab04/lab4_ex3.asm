	.data
n:	.word 10
k:	.word 5
	
	.text
main:
    la $s0, n
    lw $s0, 0($s0)
    la $s1, k
    lw $s1, 0($s1)
    add $t0, $s0, $0
    add $t1, $s1, $0
    add $t2, $t1, $t0
    add $t3, $t2, $t1
    add $t4, $t3, $t2
    add $t5, $t4, $t3
    add $t6, $t5, $t4
    add $t7, $t6, $t5
finish:
    add $a0, $t7, $0
    li $v0, 1
    syscall
    li $v0, 10
    syscall




