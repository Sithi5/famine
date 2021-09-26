%include "define.inc"
%include "data.asm"
%include "prints.asm"

section .text
global  _start

_start:
    call _print_famine

_exit:
    mov       rax, EXIT               ; system call for exit
    xor       rdi, rdi                ; exit code 0
    syscall                           ; invoke operating system to exit

