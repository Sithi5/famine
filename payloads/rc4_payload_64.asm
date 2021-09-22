BITS 64

SECTION .data
        woody_msg: db "....WOODY....",10
        woody_msg_len  : equ $-woody_msg
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
    call _print_woody
    call _mprotect
    call _getvar
    jmp rc4_cipher_start

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
    mov rax,WRITE                       ; sys_write
    mov rdi,STDOUT                       ; stdout
    mov rdx,woody_msg_len           ;len
    lea rsi,[rel $+woody_msg-$]     ; woody
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

_getencryptedsectionaddr:
    call _get_rip
    sub rax, 0x66666666 ; virus size without getencryptedsectionaddr
    sub rax, 0x66666666 ; new_entry_point
    add rax, 0x66666666 ; start of encrypted section
    ret

_gettextsectionaddr:
    call _get_rip
    sub rax, 0x22222222 ; virus size without ret2oep
    sub rax, 0x22222222 ; new_entry_point
    add rax, 0x22222222 ; start of text section
    ret

_gettextsize:
    mov r14, 0x33333333
    ret

_mprotect:
    call _gettextsectionaddr
    call _gettextsize
    mov rdi,  rax
    and rdi, -0x1000
    mov rax, 0xa
    mov rsi, r14
    mov rdx, 0x07
    syscall
    ret



_getencryptedsectionsize:
    mov r14, 0x55555555
    ret

_getvar:
    call _getencryptedsectionaddr
    call _getencryptedsectionsize
    mov rdi, rax
    mov rsi, r14
    mov rcx, [rel $+key_len-$]
    lea rdx,[rel $+key_msg-$]
    ret


_modulo_rcx:
    xor rdx, rdx					; rdx is remainder
    div rcx
    mov rax, rdx
    ret

_modulo:
    push rcx                        ; save key_len
    xor rdx, rdx                    ; rdx is remainder
    mov rcx, 256                    ; give div argument
    div rcx                         ; div
    pop rcx                         ; restore rcx
    mov rax, rdx
    ret


_swap:
    mov r11b, [r8 + r9]          ; tmp_i = stream[i]
    mov r12b, [r8 + r10]         ; tmp_j = stream[j]
    mov [r8 + r10], r11b         ; stream[j] = stream[i]
    mov [r8 + r9], r12b          ; stream[i] = tmp_j
    mov rax, r8
    ret

rc4_cipher_start:
    ;rdi = start offset
    ;rsi = size
    ;rdx = key
    ;rcx = key_size
    lea r8, [rel $+stream-$]            ; load stream to memory
    xor r9, r9                      ; int i = 0
    mov r14, rdx                    ; using rdx for modulo, store rdx in r14
    jmp init_stream

init_stream:
    cmp r9, 256                     ; while i < 256
    je init_index_values           ; reset i and j when i = 256 and continue
    mov [r8 + r9], r9               ; stream[i] = i;
    inc r9                          ; i++
    jmp init_stream

init_index_values:
    xor r9, r9                      ; int i
    xor r10, r10                    ; int j
    xor r11, r11                    ; int tmp_i for swap
    xor r12, r12                    ; int tmp_j for swap
    jmp stream_generation

stream_generation:
    cmp r9, 256                     ; while i < 256
    je reset_index_values
    add r10b, byte[r8 + r9]              ; j = j + stream[i]
    mov rax, r9
    call _modulo_rcx
    add r10b, byte[r14 + rax]            ; + key[i % key_size]
    mov rax, r10
    call _modulo
    mov r10, rax
    call _swap
    inc r9                          ; i + 1
    jmp stream_generation

reset_index_values:
    xor r9, r9                      ; int i
    xor r10, r10                    ; int j
    xor r11, r11                    ; int tmp_i for swap
    xor r12, r12                    ; int tmp_j for swap
    xor r13, r13                    ; int k
    xor r14, r14
    xor r15, r15                    ; int res
    jmp encrypt

encrypt:
    cmp r13, rsi                    ; while k < len
    jge _end_payload
    add r9, 1                       ; i = (i + 1) % N;
    mov rax, r9
    call _modulo
    mov r9, rax
    add r10b, byte[r8 + r9]              ; j = (j + stream[i])
    mov rax, r10                    ; % 256
    call _swap
    mov r14b, byte[r8 + r9]              ; stream[i] in r14
    add r14b, byte[r8 + r10]             ; + stream[j]
    mov rax, r14
    call _modulo
    mov r14, rax
    mov r15b, byte[r8 + r14]             ;res = stream[(stream[i] + stream[j]) % 256]
    xor [rdi + r13], r15            ; data[k] ^ res (modify directly data)
    inc r13                         ; k++
    jmp encrypt
