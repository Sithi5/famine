GLOBAL asmxorcipher

section .text

asmxorcipher:

push rbp
mov rbp, rsp

;rcx ; keylen 4 param
;rdx ;len 3 param
;rsi ;key 2 param
;rdi ;data 1 param

mov r14, rdx ; store data len
mov r15, rcx ; store key len

extern puts
push rdi ; store arg pointeur to data
push rsi ; store arg pointeur to key

mov r9, 00 ; len counter initialisation

resetkeyloop:
cmp r9,r14			; check len and len counter
je 	return		    ; jump if lencounter < len
pop rsi             ; get start of key
push rsi            ; send back start of key in stack
mov r8, 00          ; key len counter initialisation
xorloop:

mov rax, rdi        ; get data char address
mov bl, byte[rsi]   ; get key byte to xor
xor byte[rax],bl	; xor operation on data with key


add rdi,1			; add 1 to pointer data
add rsi,1			; add 1 to pointer key
add r9,01			; add 1 to data len counter
add r8,01           ; add 1 to key len counter
cmp r8,r15          ; check key to reset
je resetkeyloop     ; reset key pointer
cmp r9,r14			; check len and len counter
jle xorloop			; jump if lencounter < len



; return function
return:
pop rsi             ; remove key address from stack
pop rdi				; retreave data from the stack
mov rax, rdi		; set return address to rdi (data)
mov rsp,rbp
pop rbp
ret

