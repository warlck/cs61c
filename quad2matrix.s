.text

# Decodes a quadtree to the original matrix
#
# Arguments:
#     quadtree (qNode*)
#     matrix (void*)
#     matrix_width (int)
#
# Recall that quadtree representation uses the following format:
#     struct qNode {
#         int leaf;
#         int size;
#         int x;
#         int y;
#         int gray_value;
#         qNode *child_NW, *child_NE, *child_SE, *child_SW;
#     }

quad2matrix:




	lw $t0, 0($a0) # int leaf;
	lw $t1, 4($a0) # int size;
	lw $t2, 8($a0) # int x;
	lw $t3, 12($a0) # int y;
	lw $t4, 16($a0) # int gray;

	beq $t0, 0, branch
	

	
leaf:

	addi $t5, $0, 0 # x
leafx:
	addi $t6, $0, 0 # y
leafy:

	add $t7, $t6, $t3 # add start y
	mult $a2, $t7 # multiply y * width
	mflo $t7
	add $t7, $t7, $t5 # add x
	add $t7, $t7, $t2 # add start x
	add $t7, $t7, $a1 # add to pointer
	
	#addi $t4, $0, 4
	
	sb $t4, ($t7) # save the gray value
	
	addi $t6, $t6, 1 # increment y
	bne $t6, $t1, leafy # loop if y < size
	
	addi $t5, $t5, 1 # increment x
	bne $t5, $t1, leafy # loop if x < size, was leafx before
	
	j done

branch:
	addi $sp, $sp, -24 # move stack
	sw $ra, -4($sp) # save $ra
	sw $a0, -8($sp) # save $a0
	sw $s0, -12($sp) # save $s0
	sw $s1, -16($sp) # save $s1
	sw $s2, -20($sp) # save $s2
	sw $s3, -24($sp) # save $s3
	
	lw $s0, 20($a0) # load NW;
	lw $s1, 24($a0) # load NE;
	lw $s2, 28($a0) # load SW;
	lw $s3, 32($a0) # load SE;
	
	move $a0, $s0
	jal quad2matrix
	
	move $a0, $s1
	jal quad2matrix
	
	move $a0, $s2
	jal quad2matrix
	
	move $a0, $s3
	jal quad2matrix
	
	
	lw $ra, -4($sp) # load ra
	lw $a0, -8($sp) # load a0
	lw $s0, -12($sp) # load $s0
	lw $s1, -16($sp) # load $s1
	lw $s2, -20($sp) # load $s2
	lw $s3, -24($sp) # load $s3
	addi $sp, $sp, 24 # move stack

done:

        jr $ra