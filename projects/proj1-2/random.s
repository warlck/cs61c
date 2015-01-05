.text

main:

        li $s0 20

main_random_loop:
        beq $s0 $zero main_end

        # printf("%d", lfsr_random());
        jal lfsr_random
        move $a0 $v0
        jal print_int

        # printf("\n");
        jal print_newline

        addiu $s0 $s0 -1
        j main_random_loop

main_end:
        # exit()
        li $v0 10
        syscall

        .include "lfsr_random.s"
        .include "print_helpers.s"
