mov rax, 0      ; Syscall number for sys_read
mov rdi, 0      ; File descriptor 0 (stdin)
mov rsi, r12    ; Address of the buffer (your tape pointer)
mov rdx, 1      ; Number of bytes to read
syscall
