.data

lfsr:
        .align 4
        .half
        0x1

.text

# Implements a 16-bit lfsr
#
# Arguments: None
lfsr_random:

        la $t0 lfsr
        lhu $v0 0($t0)

        # YOUR CODE HERE #

        la $t0 lfsr
        sh $v0 0($t0)
        jr $ra
