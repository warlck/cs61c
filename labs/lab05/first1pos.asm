.data
shouldben1:	.asciiz "Should be -1, and firstposshift and firstposmask returned: "
shouldbe0:	.asciiz "Should be 0 , and firstposshift and firstposmask returned: "
shouldbe16:	.asciiz "Should be 16, and firstposshift and firstposmask returned: "
shouldbe31:	.asciiz "Should be 31, and firstposshift and firstposmask returned: "

.text
main:
	la	$a0, shouldbe31
	jal	print_str
	lui	$a0, 0x8000	# should be 31
	jal	first1posshift
	move	$a0, $v0
	jal	print_int
	jal	print_space

	lui	$a0, 0x8000
	jal	first1posmask
	move	$a0, $v0
	jal	print_int
	jal	print_newline

	la	$a0, shouldbe16
	jal	print_str
	lui	$a0, 0x0001	# should be 16
	jal	first1posshift
	move	$a0, $v0
	jal	print_int
	jal	print_space

	lui	$a0, 0x0001
	jal	first1posmask
	move	$a0, $v0
	jal	print_int
	jal	print_newline

	la	$a0, shouldbe0
	jal	print_str
	li	$a0, 1		# should be 0
	jal	first1posshift
	move	$a0, $v0
	jal	print_int
	jal	print_space

	li	$a0, 1
	jal	first1posmask
	move	$a0, $v0
	jal	print_int
	jal	print_newline

	la	$a0, shouldben1
	jal	print_str
	move	$a0, $0		# should be -1
	jal	first1posshift
	move	$a0, $v0
	jal	print_int
	jal	print_space

	move	$a0, $0
	jal	first1posmask
	move	$a0, $v0
	jal	print_int
	jal	print_newline

	li	$v0, 10
	syscall

first1posshift:
	### YOUR CODE HERE ###
	li 	$t1, 31 #going to count backwards

ForLoop1:
	beq	$a0, $0, return #if our given number is 0, then we return -1
	slt	$t0, $a0, $0  #if a0 < 0, t0 = 1, else t0 = 0
	bne	$t0, $0, returnPos #while t0 != 0, while not false
	addi	$t1, $t1, -1  #subtract 1 from s0, which represents position starting from leftmost pos at 31
	sll	$a0, $a0, 1 #shift to left a0, to discard leftmost bit
	j ForLoop1	

return:
	li	$v0,-1 # return -1
	jr 	$ra

returnPos:
	move 	$v0, $t1 # return position
	jr 	$ra
	
first1posmask:
	### YOUR CODE HERE ###
	li $t0, 0x80000000
	li $t1, 31
ForLoop2:
	beq $a0, $0, ret #
	and $t2, $a0, $t0
	bne $t2, $0, retPos #so if we get a 1 after anding both #s we know that at curr. pos. we found leftmost bit	
	addi $t1, $t1, -1 #else, if the number zero outs then we shift t0 to right 1
	srl $t0, $t0, 1
	j ForLoop2

ret:
	li $v0, -1
	jr $ra

retPos:
	move $v0, $t1
	jr $ra
		
print_int:
	move	$a0, $v0
	li	$v0, 1
	syscall
	jr	$ra

print_str:
	li	$v0, 4
	syscall
	jr	$ra

print_space:
	li	$a0, ' '
	li	$v0, 11
	syscall
	jr	$ra

print_newline:
	li	$a0, '\n'
	li	$v0, 11
	syscall
	jr	$ra
