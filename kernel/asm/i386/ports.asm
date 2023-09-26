; INB fetches the current value at (port: 16 bits [esp + 4])
global inb
inb:   ; I think the c compiler handles our esp for us so we dont need to mess with it.
    mov dx, [esp + 4]
    in al, dx
    ret

; OUTB (port: 16 bits [esp + 4], value: 8 bits [esp + 8]) writes value to (port)
global outb
outb:   ; I think the c compiler handles our esp for us so we dont need to mess with it.
    mov dx, [esp + 4]
    mov al, [esp + 8]
    out dx, al
    ret

; INW (port: 16 bits [esp + 4]) fetches the current value at (port: 16 bits [esp + 4])
global inw
inw:   ; I think the c compiler handles our esp for us so we dont need to mess with it.
    mov dx, [esp + 4]
    in ax, dx
    ret

; OUTW (port: 16 bits [esp + 4], value: 16 bits [esp + 8]) writes value to (port)
global outw
outw:   ; I think the c compiler handles our esp for us so we dont need to mess with it.
    mov dx, [esp + 4]
    mov ax, [esp + 8]
    out dx, ax
    ret
