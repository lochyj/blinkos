global load_gdt
load_gdt:
    mov eax, [esp + 4]
    lgdt [eax]

    jmp 0x08:flush  ; short jump

flush:
    mov ax, 0x10
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    ret
