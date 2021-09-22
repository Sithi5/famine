BITS 64

SECTION .data
        woody_msg: db "....WOODY....",10
        woody_msg_len  : equ $-woody_msg

SECTION .text
%define WRITE 1
%define STDOUT 1

_start_payload:
    push rax                 ; save all clobbered registers
    push rcx                 ; (rcx and r11 destroyed by kernel)
    push rdx
    push rsi
    push rdi
    push r11

_infection:
    call _print_woody

_end_payload:
    pop r11
    pop rdi
    pop rsi
    pop rdx
    pop rcx
    pop rax

    call _ret2oep           ; jump to original entry point(oep)
    push rax
    ret

_get_rip:
    mov rax, qword [rsp]
    ret

_ret2oep:
    call _get_rip
    sub rax, 0x77777777 ; virus size without ret2oep
    sub rax, 0x77777777 ; new_entry_point
    add rax, 0x77777777 ; old entry_point
    ret

_print_woody:
    mov rax, WRITE                ; sys_write
    mov rdi, STDOUT                ; stdout
    mov rdx,woody_msg_len;len
    lea rsi,[rel $+woody_msg-$]  ; woody
    syscall
    ret