.data

debug_period:
        .word
        3
debug_data:
        .word
        0

.text

# Debug random 1
#
# Implements a 16-bit number generator with period of
# 3. (e.g. Every 3rd pixel is white.)
#
# Arguments: None
debug_random1:

        la $t0 debug_period
        lw $t1 0($t0)
        la $t2 debug_data
        lw $t3 0($t2)
        div $t3 $t1
        mfhi $t4

        addiu $t3 $t3 1
        sw $t3 0($t2)

        # Return white if t3 % t1 == 0
        beq $t4 $zero debug_random3
        j debug_random2

# Debug random 2
#
# Returns 0 (black).
#
# Arguments: None
debug_random2:

        li $v0 0
        jr $ra

# Debug random 3
#
# Returns 1 (white).
#
# Arguments: None
debug_random3:

        li $v0 0xff
        jr $ra
