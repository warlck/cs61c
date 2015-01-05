#Homework 3

	.data
mem: 	.space 80 # 4 bytes/word * 20 words
testAB: .asciiz "Testing linked lists A and B (should be 0): "
testAC: .asciiz "Testing linked lists A and C (should be 1): "
testAD:	.asciiz "Testing linked lists A and D (should be 0): "
nl: 	.asciiz "\n"

# Linked lists for testing:
# A: 1,0,2,2,9,5,NULL
# B: 1,0,2,2,9,1,NULL
# C: 2,9,0,1,5,2,NULL
# D: 1,0,2,2,9,NULL
A5:	.word 5	0
A4: .word 9 A5
A3: .word 2 A4
A2: .word 2 A3
A1: .word 0 A2
A0: .word 1 A1

B5:	.word 1 0
B4:	.word 9 B5
B3:	.word 2 B4
B2:	.word 2 B3
B1:	.word 0 B2
B0:	.word 1 B1

C5:	.word 2 0
C4:	.word 5 C5
C3:	.word 1 C4
C2:	.word 0 C3
C1:	.word 9 C2
C0:	.word 2 C1

D4:	.word 9 0
D3:	.word 2 D4
D2:	.word 2 D3
D1:	.word 0 D2
D0:	.word 1 D1

	.text
	.globl main

# Function that sets the values of a block of memory to zero.
# Do NOT modify this function.
# Params:
#  $a0 = beginning of block of memory
#  $a1 = number of words to set to zero
zeroMem:
	beq $a1,$zero,done
	sw $zero,0($a0)
	addiu $a0,$a0,4
	addi $a1,$a1,-1
	j zeroMem
done:   
	jr $ra

# Function that traverses through a linked list whose values contain
# the numbers 0 to 9 and stores the count of each number into an array.
# Params:
#  $a0 = beginning of the linked list
#  $a1 = a pointer to memory for the array
#	You may assume that the memory block contains enough space for 10 elements

countList:
# YOUR CODE HERE
	beq $a0, $0, countListDone
	lw $t0, 0($a0)
	sll $t0, $t0, 2
	addu $t0, $t0, $a1
	lw $t1, 0($t0)
	addiu $t1, $t1, 1
	sw $t1, 0($t0)
	lw $a0, 4($a0)
	j countList
countListDone:
	jr $ra
# Function that checks if the numbers (0-9) stored in two linked lists are
# anagrams of each other (ie. if two lists contains the same number
# of each int). Each linked list will be at least 1 element long.
# You should first traverse each linked list and store the count of each digit into
# an array (memory for it is given to you), and then check if the two arrays
# are equal.
# Params:
#  $a0 = first element of linked list 1
#  $a1 = first element of linked list 2
#  $a2 = memory segment containing space for 2 arrays of 10 ints each.
#	 Note that the memory segment may contain garbage values.
#
# Return (put in $v0): 1 if linked lists are anagrams, 0 if not

isAnagram:
# YOUR CODE HERE
	addiu $sp, $sp, -16
	sw $s0, 0($sp)
	sw $s1, 4($sp)
	sw $s2, 8($sp)
	sw $ra, 12($sp)

	move $s0, $a0
	move $s1, $a1
	move $s2, $a2
# Perhaps prepare the block of memory somehow?
	li $a1, 20
	move $a0, $s2
	jal zeroMem
# Counts first list
	move $a0, $s0
	move $a1, $s2
	jal countList
# Counts second list
	move $a0, $s1
	addiu $a1, $s2, 40
	jal countList
# Check arrays for equality
	li $v0, 0
	move $s0, $s2
	addiu $s1, $s2, 40
	addiu $s2, $s2, 40
checkEqual:
	lw $t0, 0($s0)
	lw $t1, 0($s1)
	bne $t0, $t1, isAnagramDone
	addiu $s0, $s0, 4
	addiu $s1, $s1, 4
	bne $s0, $s2, checkEqual
	li $v0, 1
isAnagramDone:
	lw $s0, 0($sp)
	lw $s1, 4($sp)
	lw $s2, 8($sp)
	lw $ra, 12($sp)
	addiu $sp, $sp, 16
	jr $ra

# Program starts here. Do NOT modify anything below this line.
main:
	la $a0,testAB	# Test LL A and B
	li $v0,4
	syscall
	la $a0,A0
	la $a1,B0
	la $a2,mem
	jal isAnagram
	move $a0,$v0
	li $v0,1
	syscall
	la $a0,nl
	li $v0,4
	syscall

	la $a0,testAC	# Test LL A and C
	li $v0,4
	syscall
	la $a0,A0
	la $a1,C0
	la $a2,mem
	jal isAnagram
	move $a0,$v0
	li $v0,1
	syscall
	la $a0,nl
	li $v0,4
	syscall

	la $a0,testAD	# Test LL A and D
	li $v0,4
	syscall
	la $a0,A0
	la $a1,D0
	la $a2,mem
	jal isAnagram
	move $a0,$v0
	li $v0,1
	syscall
	li $v0 10	# Exit
	syscall