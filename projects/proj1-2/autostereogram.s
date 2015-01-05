.data
usage:  .asciiz "mars autostereogram.s pa [input_file] [output_file] [strip_size] [--verbose]\n"
depth_map_label:
        .asciiz "Depth map:\n"
output_label:
        .asciiz "Output:\n"

.text

main:

        li $t0 3
        beq $a0 $t0 params_ok_3
        li $t0 4
        beq $a0 $t0 params_ok_4

        # usage(); exit();
        li $v0 4
        la $a0 usage
        syscall
        li $v0 10
        syscall

params_ok_3:

        li $s7 0
        j params_ok

params_ok_4:

        li $s7 1
        j params_ok

params_ok:

        # s0 = argv
        move $s0 $a1

        # s1,s2,s3 = load_image(argv[0])
        lw $a0 0($s0)
        jal load_image
        lw $s1 0($v0)
        lw $s2 4($v0)
        lw $s3 8($v0)

        # s4 = atoi(argv[2])
        lw $a0 8($s0)
        jal atoi
        move $s4 $v0

        # s5 = malloc(s2 * s3)
        mul $a0 $s2 $s3
        li $v0 9
        syscall
        move $s5 $v0

        # Allocate space on the stack for variables
        addiu $sp $sp -20

        # autostereogram
        sw $s5 0($sp)

        # depth_map
        sw $s1 4($sp)

        # width
        sw $s2 8($sp)

        # height
        sw $s3 12($sp)

        # strip
        sw $s4 16($sp)

        jal calc_autostereogram
        addiu $sp $sp 20

        # save_image(argv[1], s5, s2, s3)
        lw $a0 4($s0)
        move $a1 $s5
        move $a2 $s2
        move $a3 $s3
        jal save_image

        beq $s7 $zero dont_print_matrix

        # printf("%s", depth_map_label)
        la $a0 depth_map_label
        jal print_str

        # print_matrix(depth_map, s2, s3)
        move $a0 $s1
        move $a1 $s2
        move $a2 $s3
        jal print_matrix

        # printf("%s", output_label)
        la $a0 output_label
        jal print_str

        # print_matrix(autostereogram, s2, s3)
        move $a0 $s5
        move $a1 $s2
        move $a2 $s3
        jal print_matrix

dont_print_matrix:

        # exit()
        li $v0 10
        syscall

        .include "calc_autostereogram.s"
        .include "debug_random.s"
        .include "lfsr_random.s"
        .include "print_helpers.s"
        .include "print_matrix.s"
        .include "utils.s"
