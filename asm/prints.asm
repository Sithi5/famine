section .text

_print_famine:
    mov rax, WRITE                ; sys_write
    mov rdi, STDOUT                ; stdout
    mov rdx,famine_msg_len;len
    lea rsi,[rel $+famine_msg-$]  ; famine
    syscall
    ret