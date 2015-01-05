.text

# Helper functions (may be useful for debugging)
# You can assume these don't touch any registers except
# $ra, $v0, and $a0.
print_int:
        li  $v0 1
        syscall
        jr  $ra

print_intx:
        li  $v0 34
        syscall
        jr  $ra

print_str:
        li  $v0 4
        syscall
        jr  $ra

print_space:
        li  $a0 ' '
        li  $v0 11
        syscall
        jr  $ra

print_newline:
        li  $a0 '\n'
        li  $v0 11
        syscall
        jr  $ra
