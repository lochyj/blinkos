BITS 32
ALIGN 4

multiboot:
	; Multiboot headers:
	;   Page aligned loading, please
	MULTIBOOT_PAGE_ALIGN	equ 1<<0

	;   Get memory alignment info
	MULTIBOOT_MEMORY_INFO	equ 1<<1

	;   Do we want graphics?
	MULTIBOOT_USE_GFX		equ 1<<2

	;   We are multiboot compatible!
	MULTIBOOT_HEADER_MAGIC	equ 0x1BADB002

	;   Load up those flags.
	MULTIBOOT_HEADER_FLAGS	equ MULTIBOOT_PAGE_ALIGN | MULTIBOOT_MEMORY_INFO | MULTIBOOT_USE_GFX

	;   Checksum the result
	MULTIBOOT_CHECKSUM		equ -(MULTIBOOT_HEADER_MAGIC + MULTIBOOT_HEADER_FLAGS)

	; Load the headers into the binary image.
	dd MULTIBOOT_HEADER_MAGIC
	dd MULTIBOOT_HEADER_FLAGS
	dd MULTIBOOT_CHECKSUM
	dd 0x00000000 ; header_addr
	dd 0x00000000 ; load_addr
	dd 0x00000000 ; load_end_addr
	dd 0x00000000 ; bss_end_addr
	dd 0x00000000 ; entry_addr

	; Graphics requests
	dd 0x00000000 ; 0 = linear graphics
	dd 0
	dd 0
	dd 32         ; It is good practise to set this to 32

; Reference constants in the linker script
extern code, bss, end

global _start
_start:
	; Set up the stack pointer.
	mov esp, 0x7FFFF    ; This should be fine for now... May need to set it up at a different address in the future.
	push esp

	; Push the incoming mulitboot headers
	push eax    ; Header magic
	push esp    ; Stack pointer
	push ebx    ; Header pointer

	cli         ; Disable interrupts

	extern kmain
	call kmain

lp:
	jmp lp