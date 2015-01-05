.data

.include    "quad2matrix_data.s"

matrix:
            .align 4
            .byte     0 : 64
start_msg:  .asciiz "Starting program...\n"
exit_msg:   .asciiz "Exiting program...\n"

.text

main:
        # printf("%s", start_msg)
        la $a0 start_msg
        jal print_str

        # quad2matrix(&q00, &matrix, 8)
        la $a0 q00
        la $a1 matrix
        li $a2 8
        jal quad2matrix

        # print_matrix(&matrix, 8)
        la $a0 matrix
        li $a1 8
        li $a2 8
        jal print_matrix

        # print a new line
        jal print_newline

        # printf("%s", exit_msg)
        la $a0 exit_msg
        jal print_str

        # exit()
        li $v0 10
        syscall

.include    "quad2matrix.s"
.include    "print_matrix.s"
.include    "print_helpers.s"
