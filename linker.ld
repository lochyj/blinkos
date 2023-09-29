/* `_start` is the entry point for the kernel */
ENTRY(_start)
OUTPUT_FORMAT("elf32-i386")

SECTIONS {
	/* Begin putting sections at 1 MiB, a conventional place for kernels to be
	   loaded at by the bootloader. */
	. = 0x100000;

	/* The multiboot header needs to be first so that the bootloader recognises the kernel as being multiboot */
	.text BLOCK(4K) : ALIGN(4K) {
		*(.multiboot)
		*(.text)
	}

	.rodata BLOCK(4K) : ALIGN(4K) {
		*(.rodata)
	}
 
	/* Read-write data (initialized) */
	.data BLOCK(4K) : ALIGN(4K) {
		*(.data)
	}
 
	/* Read-write data (uninitialized) and stack */
	.bss BLOCK(4K) : ALIGN(4K) {
		*(COMMON)
		*(.bss)
	}

	kernel_end = .;

    /* Remove all of the useless information */
	/DISCARD/ : {
		*(.comment)
		*(.note.gnu.build-id)
	}
}