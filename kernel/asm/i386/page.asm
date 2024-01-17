global load_page_directory
load_page_directory:

    mov eax, cr4        ; Get the value of the cr4 register.
    or eax, 0x00000010  ; Set the PSE bit in cr4. (Page Size Extensions)
    mov cr4, eax        ; Write the new value of cr4 back to the register. (With PSE enabled.)

    mov eax, cr0        ; Get the value of the cr0 register.
    or eax, 0x80000000  ; Set the paging bit in cr0.
    mov cr0, eax        ; Write the new value of cr0 back to the register. (With paging enabled.)

    mov eax, [esp + 4]  ; Get the physical address of the page directory that is passed to us.
    mov cr3, eax        ; Move the physical address of the page dir into the cr3 register.

    ret
    ; jmp $               ; Paging is enabled now. :)

