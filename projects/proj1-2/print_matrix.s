.data

# Header for matrix
pm_head:  .asciiz "  -----0----------1----------2----------3----------4----------5----------6----------7-----\n"

.text

# Prints a $a1 x $a2 matrix
#
# Arguments:
#     matrix (unsigned char*)
#     matrix_width (int)
#     matrix_height (int)
print_matrix:
        addiu $sp $sp -12
        sw $ra 0($sp)
        sw $a0 4($sp)
        sw $a1 8($sp)

        # t0 = matrix
        move $t0 $a0
        # t1 = matrix_width
        move $t1 $a1
        # t2 = matrix_height
        move $t2 $a2

        # printf("%s", pm_head)
        la $a0 pm_head
        jal print_str

        # for (t3 = 0; t3 < height; t3++)
        move $t3 $zero
pm_loop1:
        slt $t9 $t3 $t2
        beq $t9 $zero pm_loop1end

        # printf("%d ", t3)
        move $a0 $t3
        jal print_int
        jal print_space

        # for (t4 = 0; t4 < width; t4++)
        move $t4 $zero
pm_loop2:
        slt $t9 $t4 $t1
        beq $t9 $zero pm_loop2end

        # printf("%x ", matrix[t3 * width + t4])
        mul $t5 $t1 $t3
        addu $t5 $t5 $t4
        addu $t5 $t0 $t5
        lbu $a0 0($t5)
        jal print_intx
        jal print_space

        addiu $t4 $t4 1
        j pm_loop2
pm_loop2end:

        # printf("\n")
        jal print_newline

        addiu $t3 $t3 1
        j pm_loop1

pm_loop1end:

        lw $ra 0($sp)
        lw $a0 4($sp)
        lw $a1 8($sp)
        addiu $sp $sp 12
        jr $ra
