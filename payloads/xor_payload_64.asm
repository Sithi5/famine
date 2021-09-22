BITS 64

SECTION .data
        woody_msg: db "....WOODY....",10
        woody_msg_len  : equ $-woody_msg
        key_msg: db "0"
        key_len  : equ $-key_msg

SECTION .text
%define write 1
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
    call _print_woody
    call _mprotect
    call _getxorciphervar
    jmp _xorcipher

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

_print_woody:
    mov rax, WRITE                ; sys_write
    mov rdi, STDOUT                ; stdout
    mov rdx,woody_msg_len;len
    lea rsi,[rel $+woody_msg-$]  ; woody
    syscall
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

_ret2textsection:
    call _get_rip
    sub rax, 0x66666666 ; virus size without ret2oep
    sub rax, 0x66666666 ; new_entry_point
    add rax, 0x66666666 ; start of text section
    ret

_mprotect:
    call _ret2textsection
    call _settextoffset
    mov rdi,  rax
    and rdi, -0x1000
    mov rax, 0xa
    mov rsi, r14
    mov rdx, 0x07
    syscall
    ret

_settextoffset:
    mov r14, 0x55555555
    ret

_getxorciphervar:
    call _ret2textsection
    call _settextoffset
    mov rdi, rax
    mov r15, key_len
    lea rsi,[rel $+key_msg-$]
    ret


_xorcipher:

mov r9, 00 ; len counter initialisation

_resetkeyloop:
    cmp r9,r14			; check len and len counter
    je 	_end_payload    ; jump if lencounter < len
    lea rsi,[rel $+key_msg-$]
    mov r8, 00          ; key len counter initialisation

_xorloop:
    mov rax, rdi        ; get data char address
    mov bl, byte[rsi]   ; get key byte to xor
    xor byte[rax],bl	; xor operation on data with key


    add rdi,1			; add 1 to pointer data
    add rsi,1			; add 1 to pointer key
    add r9,01			; add 1 to data len counter
    add r8,01           ; add 1 to key len counter
    cmp r8,r15          ; check key to reset
    je _resetkeyloop    ; reset key pointer
    cmp r9,r14			; check len and len counter
    jle _xorloop		; jump if lencounter < len