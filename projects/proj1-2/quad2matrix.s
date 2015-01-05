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

    # YOUR CODE HERE # NOTES
    # remember look at next line, so maybe we don't need to do adding 4 bytes to address
    # Each element in the matrix is a byte (8 bits), not a word.
    # takes $a0-pointer to the root of quadtree, $a1-pointer to the first element of the matrix, $a2-width of matrix

    #The formula (x + y * map_width) is used to represent the index of that coordinate in the one dimensional array 
    #can do check if it has children so lead idea at bottom to not waste space

    lw $t0, 0($a0) #leaf value either 1 or 0
    lw $t1, 4($a0) #size amount 
	lw $t2, 8($a0) #x
	lw $t3, 12($a0) #y
	lw $t4, 16($a0) #gray_value
    beq $t0, 0, recurse #we have leaf = 0 which is a parent

OuterLoop:
	addi $t5, $0, 0 #int column x = 0
	addi $t6, $0, 0 #int row y = 0
	add $t7, $t6, $t3 # t7 = y(coor) + y=0(shift) 
	mul $t7, $t7, $a2 # t7 = t7(y + y) * matrix_width
	add $t7, $t7, $t5 # var coord = coord(y * matrix_widht) + x(shift)
	add $t7, $t7, $t2 # coord += x(coor)
	add $t7, $t7, $a1  #coord += a1(pointer address to get to right position)
	sb $t4, 0($t7) #matrix[coor] = gray
	addi $t6, $t6, 1 #h++;
	slt $t8, $t6, $t1 # y=row=0 < size
	beq $t8, $0, done #done if condition false so y == size
	addi $t5, $t5, 1 #s++;
	slt $t9, $t5, $t1 #x=column=0 < size
	bne $t9, $0, done #done if condition false so x == size
	j OuterLoop # go back to while loop

recurse: #Prologue
	addi $sp, $sp, -24 #6 things 4 parameters to recursive call, return address, & single argument that changes
	sw $ra, 0($sp) #register address
	sw $a0, 4($sp) #first argument to function quad2matrix
    sw $s0, 8($sp) #saved registers for parent's 4 children address value
    sw $s1, 12($sp)
    sw $s2, 16($sp)
    sw $s3, 20($sp)

    lw $s0, 20($a0) # load NW_child;
	lw $s1, 24($a0) # load NE_child;
	lw $s2, 28($a0) # load SW_child;
	lw $s3, 32($a0) # load SE_child;

    # 4 recursive calls
    move $a0, $s0
    jal quad2matrix
    move $a0, $s1
    jal quad2matrix
    move $a0, $s2
    jal quad2matrix
    move $a0, $s3
    jal quad2matrix
    
    #Epilogue
    lw $s3, 20($sp) # load $s3
	lw $s2, 16($sp) # load $s2
	lw $s1, 12($sp) # load $s1
	lw $s0, 8($sp) # load $s0
	lw $a0, 4($sp) # load a0
	lw $ra, 0($sp) # load ra
	addi $sp, $sp, 24 # restore stack
done:
	jr $ra


	





