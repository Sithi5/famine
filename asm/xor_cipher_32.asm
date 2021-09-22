GLOBAL asm_xor_cipher

section .text

%define syscall int 0x80
%define write 4
%define STDOUT 1

asm_xor_cipher:
    enter 0,0 ; push ebp, mov ebp, sub esp, N

    mov edx, [ebp + 20] ; keylen
    dec edx ; keylen
    mov ecx, [ebp + 12] ; datalen
    dec ecx ; datalen
    jmp _encrypt

_encrypt:

    mov eax, [ebp + 8] ; data
    mov ebx, [ebp + 16] ; key

    cmp edx, 0
    jge .continue
    mov edx, [ebp + 20] ; keylen
    dec edx ; dec keylen
    .continue:

    mov bl, byte[ebx + edx] ; getting key char
    xor byte[eax + ecx], bl

    dec edx ; dec keylen
    dec ecx ; datalen
    cmp ecx, 0
    jge _encrypt

return:
    leave ; = mov esp, ebp et pop ebp => la pile reprend son ancien etat
    ret