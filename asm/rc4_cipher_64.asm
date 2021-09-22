GLOBAL rc4_cipher_start

section .bss
    global stream

    stream resb 256

section .text


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
    lea r8, [rel stream]            ; load stream to memory
    xor r9, r9                      ; int i = 0
    mov r14, rdx                    ; using rdx for modulo, store rdx in r14

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

encrypt:
    cmp r13, rsi                    ; while k < len
    jge return
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

return:
    ret




