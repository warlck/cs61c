.data

bmp_write_header:
        .align 4
        .byte
        # 0x00 Magic byte
        0x42, 0x4d,
        # 0x02 File size
        0x00, 0x00, 0x00, 0x00,
        # 0x06 Unused
        0x00, 0x00, 0x00, 0x00,
        # 0x0a Location of data (from start of file)
        # Value: 1078
        0x36, 0x04, 0x00, 0x00,
        # 0x0e Size of the remainder of the header (including this field)
        # Value: 40
        0x28, 0x00, 0x00, 0x00,
        # 0x12 Width
        0x00, 0x00, 0x00, 0x00,
        # 0x16 Height
        0x00, 0x00, 0x00, 0x00,
        # 0x1A Number of planes
        0x01, 0x00,
        # 0x1C Bit depth (8 for grayscale, 24 for rgb color)
        # Value: Grayscale (8)
        0x08, 0x00,
        # 0x1E BI_RBG
        0x00, 0x00, 0x00, 0x00,
        # 0x22 Size of the data (including padding)
        0x00, 0x00, 0x00, 0x00,
        # 0x26 Horizontal PPI
        0x12, 0x0b, 0x00, 0x00,
        # 0x2A Vertical PPI
        0x12, 0x0b, 0x00, 0x00,
        # Number of palette colors (defaults to 2^{bit depth})
        0x00, 0x00, 0x00, 0x00,
        # Number of "important colors"
        0x00, 0x00, 0x00, 0x00
bmp_write_buffer:
        .align 4
        .byte
        0x00, 0x00, 0x00, 0x00
bmp_read_header:
        .align 4
        .space 54
bmp_read_buffer:
        .align 4
        .byte
        0x00, 0x00, 0x00, 0x00

.text

# Reads a little-endian int from a non-aligned buffer
#
# Arguments:
#     buffer (void*)
bmp_read_int:
        # buffer[0]
        lbu $v0 0($a0)

        # buffer[1] << 8
        lbu $t0 1($a0)
        sll $t0 $t0 8
        or $v0 $v0 $t0

        # buffer[2] << 16
        lbu $t0 2($a0)
        sll $t0 $t0 16
        or $v0 $v0 $t0

        # buffer[3] << 24
        lbu $t0 3($a0)
        sll $t0 $t0 24
        or $v0 $v0 $t0

        jr $ra

# Writes a little-endian int to non-aligned buffer
#
# Arguments:
#     buffer (void*)
#     value (unsigned int)
bmp_write_int:
        # buffer[0] = (value >> 0) & 0xff;
        srl $t0 $a1 0
        andi $t0 $t0 0xff
        sb $t0 0($a0)

        # buffer[1] = (value >> 8) & 0xff;
        srl $t0 $a1 8
        andi $t0 $t0 0xff
        sb $t0 1($a0)

        # buffer[2] = (value >> 16) & 0xff;
        srl $t0 $a1 16
        andi $t0 $t0 0xff
        sb $t0 2($a0)

        # buffer[3] = (value >> 24) & 0xff;
        srl $t0 $a1 24
        andi $t0 $t0 0xff
        sb $t0 3($a0)

        jr $ra

# Loads an image from a file
#
# Arguments:
#     filename (null terminated char*)
load_image:
        addiu $sp $sp -36
        sw $s0 0($sp)
        sw $s1 4($sp)
        sw $s2 8($sp)
        sw $s3 12($sp)
        sw $s4 16($sp)
        sw $s5 20($sp)
        sw $s6 24($sp)
        sw $s7 28($sp)
        sw $ra 32($sp)

        # s0 = open(fout, "r")
        li $v0 13
        li $a1 0
        li $a2 0
        syscall
        move $s0 $v0

        # read(s0, bmp_read_header, 54)
        li $v0 14
        move $a0 $s0
        la $a1 bmp_read_header
        li $a2 54
        syscall

        # Skip the color table
        # s1 = bmp_read_int(bmp_read_header + 0x0a) - 54
        # read(s0, malloc(s1), s1)
        la $a0 bmp_read_header
        addiu $a0 $a0 0x0a
        jal bmp_read_int
        addiu $s1 $v0 -54
        li $v0 9
        move $a0 $s1
        syscall
        move $a0 $s0
        move $a1 $v0
        move $a2 $s1
        li $v0 14
        syscall

        # s2 (width) s3 (height)
        # s2 = bmp_read_int(bmp_read_header + 0x12)
        # s3 = bmp_read_int(bmp_read_header + 0x16)
        la $a0 bmp_read_header
        addiu $a0 $a0 0x12
        jal bmp_read_int
        move $s2 $v0
        la $a0 bmp_read_header
        addiu $a0 $a0 0x16
        jal bmp_read_int
        move $s3 $v0

        # s2 = abs(s2)
        # s3 = abs(s3)
        abs $s2 $s2
        abs $s3 $s3

        # s4 = malloc(s2 * s3)
        li $v0 9
        mult $s2 $s3
        mflo $a0
        syscall
        move $s4 $v0

        # s5 = (4 - width % 4) % 4
        li $t0 4
        divu $s2 $t0
        mfhi $s5
        subu $s5 $t0 $s5
        divu $s5 $t0
        mfhi $s5

        # for (s6 = height - 1; s6 >= 0; s6--)
        addiu $s6 $s3 -1
bmp_row_loop:
        slt $t0 $s6 $zero
        bne $t0 $zero bmp_row_loop_done

        # a1 = &data[row * width]
        mult $s2 $s6
        mflo $a1
        addu $a1 $a1 $s4

        # read(s0, a1, s2)
        li $v0 14
        move $a0 $s0
        move $a2 $s2
        syscall

        # if (s5 != 0)
        beq $s5 $zero bmp_dont_read1

        # read(s0, garbage, s5)
        li $v0 14
        move $a0 $s0
        la $a1 bmp_read_buffer
        move $a2 $s5
        syscall

bmp_dont_read1:

        addiu $s6 $s6 -1
        j bmp_row_loop

bmp_row_loop_done:

        # close(s0)
        li $v0 16
        move $a0 $s0
        syscall

        li $v0 9
        li $a0 12
        syscall

        sw $s4 0($v0)
        sw $s2 4($v0)
        sw $s3 8($v0)

        lw $s0 0($sp)
        lw $s1 4($sp)
        lw $s2 8($sp)
        lw $s3 12($sp)
        lw $s4 16($sp)
        lw $s5 20($sp)
        lw $s6 24($sp)
        lw $s7 28($sp)
        lw $ra 32($sp)
        addiu $sp $sp 36
        jr $ra

# Saves an image to a file
#
# Arguments:
#     filename (null terminated char*)
#     data
#     width
#     height
save_image:
        addiu $sp $sp -36
        sw $s0 0($sp)
        sw $s1 4($sp)
        sw $s2 8($sp)
        sw $s3 12($sp)
        sw $s4 16($sp)
        sw $s5 20($sp)
        sw $s6 24($sp)
        sw $s7 28($sp)
        sw $ra 32($sp)

        # s0 = data
        # s1 = width
        # s2 = height
        move $s0 $a1
        move $s1 $a2
        move $s2 $a3

        # s3 = open(fout, "w")
        li $v0 13
        li $a1 1
        li $a2 0
        syscall
        move $s3 $v0

        # bmp_write_int(bmp_write_header + 0x12, width)
        la $a0 bmp_write_header
        addu $a0 $a0 0x12
        move $a1 $s1
        jal bmp_write_int

        # bmp_write_int(bmp_write_header + 0x16, height)
        la $a0 bmp_write_header
        addu $a0 $a0 0x16
        move $a1 $s2
        jal bmp_write_int

        # s4 = (4 - width % 4) % 4
        li $t0 4
        divu $s1 $t0
        mfhi $s4
        subu $s4 $t0 $s4
        divu $s4 $t0
        mfhi $s4

        # s5 = additional padding at footer
        # (4 - ((1078 + height * (width + s4)) % 4)) % 4
        addu $s5 $s1 $s4
        mult $s2 $s5
        mflo $s5
        addiu $s5 $s5 1078
        li $t0 4
        divu $s5 $t0
        mfhi $s5
        subu $s5 $t0 $s5
        divu $s5 $t0
        mfhi $s5

        # bmp_write_int(bmp_write_header + 0x2, total_size)
        # total_size = 1078 + height * (width + s4) + s5
        la $a0 bmp_write_header
        addu $a0 $a0 0x2
        addu $a1 $s1 $s4
        mult $s2 $a1
        mflo $a1
        addiu $a1 $a1 1078
        addu $a1 $a1 $s5
        jal bmp_write_int

        # bmp_write_int(bmp_write_header + 0x22, data_size)
        # total_size = height * (width + s4) + s5
        la $a0 bmp_write_header
        addu $a0 $a0 0x22
        addu $a1 $s1 $s4
        mult $s2 $a1
        mflo $a1
        addu $a1 $a1 $s5
        jal bmp_write_int

        # write(s3, bmp_write_header, 54)
        li $v0 15
        move $a0 $s3
        la $a1 bmp_write_header
        li $a2 54
        syscall

        # for (s6 = 0; s6 != 0x100; s6++) {
        li $s6 0
bmp_loop1:
        li $t0 0x100
        beq $s6 $t0 bmp_loop1_done
        la $a1 bmp_write_buffer
        # color_table = (unsigned char*) { s6, s6, s6, 0 };
        sb $s6 0($a1)
        sb $s6 1($a1)
        sb $s6 2($a1)
        sb $zero 3($a1)
        # write(s3, &color_table, 4)
        li $v0 15
        move $a0 $s3
        li $a2 4
        syscall
        # (s6++)
        addiu $s6 $s6 1
        j bmp_loop1
bmp_loop1_done:

        # for (s6 = height - 1; s6 != -1; s6--)
        addiu $s6 $s2 -1
bmp_loop2:
        li $t0 -1
        beq $s6 $t0 bmp_loop2_done

        # a1 = data + s6 * width
        mult $s1 $s6
        mflo $a1
        addu $a1 $a1 $s0

        # write(s3, a1, width)
        li $v0 15
        move $a0 $s3
        move $a2 $s1
        syscall

        # if ($s4 != 0)
        beq $s4 $zero bmp_dont_write1

        la $a1 bmp_write_buffer
        sb $zero 0($a1)
        sb $zero 1($a1)
        sb $zero 2($a1)
        sb $zero 3($a1)

        # write(s3, a1, s4) -- row padding
        li $v0 15
        move $a0 $s3
        move $a2 $s4
        syscall

        # endif

bmp_dont_write1:

        addiu $s6 $s6 -1
        j bmp_loop2

bmp_loop2_done:

        beq $s5 $zero bmp_dont_write2

        la $a1 bmp_write_buffer
        sb $zero 0($a1)
        sb $zero 1($a1)
        sb $zero 2($a1)
        sb $zero 3($a1)

        # write(s3, a1, s5) -- file padding
        li $v0 15
        move $a0 $s3
        move $a2 $s5
        syscall

bmp_dont_write2:

        # close(s3)
        li $v0 16
        move $a0 $s3
        syscall

        lw $s0 0($sp)
        lw $s1 4($sp)
        lw $s2 8($sp)
        lw $s3 12($sp)
        lw $s4 16($sp)
        lw $s5 20($sp)
        lw $s6 24($sp)
        lw $s7 28($sp)
        lw $ra 32($sp)
        addiu $sp $sp 36
        jr $ra

# Converts a null-terminated string into an integer
#
# Arguments:
#     number (null terminated char*)
atoi:
        li $v0 0
atoi_loop:
        lb $t0 0($a0)
        beq $t0 0 atoi_end
        addiu $t0 $t0 -48
        li $t1 10
        mul $v0 $v0 $t1
        addu $v0 $v0 $t0
        addiu $a0 $a0 1
        j atoi_loop
atoi_end:
        jr $ra
