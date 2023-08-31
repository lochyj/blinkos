# $< = input | $@ = output

# Future: $(shell find . -name '*.c')

C_SOURCES = $(wildcard					\
				kernel/*.c kernel/lib/*.c kernel/mm/*.c\
				drivers/tty/*.c			\
				)

ASM_SOURCES = $(wildcard				\
				kernel/asm/i386/*.asm	\
			)

C_OBJS = ${C_SOURCES:.c=.o}
ASM_OBJS = ${ASM_SOURCES:.asm=.out}

CFLAGS = -std=gnu99 -ffreestanding -O2 -Wall -Wextra -Wno-int-conversion -Wno-unused-parameter -Wno-unused-function -I . -I ./include/
ASMFLAGS = -felf32
BUILD_DIR = ./build

.PHONY: all link mboot buildiso run build clean

all: $(ASM_OBJS) $(C_OBJS) link mboot buildiso run clean

%.out: %.asm
	nasm $(ASMFLAGS) $< -o $@

%.o: %.c
	i686-elf-gcc $(CFLAGS) -c $< -o $@

link:
	i686-elf-ld -T linker.ld $(ASM_OBJS) $(C_OBJS) -o $(BUILD_DIR)/BlinkOS.bin

mboot: $(BUILD_DIR)/BlinkOS.bin
	grub2-file --is-x86-multiboot $<

buildiso:
	mkdir -p $(BUILD_DIR)/iso/boot/grub

	cp $(BUILD_DIR)/BlinkOS.bin $(BUILD_DIR)/iso/boot/BlinkOS.bin
	cp grub.cfg $(BUILD_DIR)/iso/boot/grub/grub.cfg

	grub2-mkrescue -o $(BUILD_DIR)/image/BlinkOS.iso $(BUILD_DIR)/iso

run:
	qemu-system-i386                                 	\
		-accel tcg,thread=single                       	\
		-cpu core2duo                                  	\
		-m 128                                         	\
		-drive format=raw,media=cdrom,file=$(BUILD_DIR)/image/BlinkOS.iso\
		-monitor stdio                                 	\
		-smp 1                                         	\
		-usb                                           	\
		-vga std										\
		-d int											\
#		-no-reboot                                     	\
		-no-shutdown

build: $(ASM_OBJS) $(C_OBJS) link mboot buildiso run

clean:
	rm -f $(C_OBJS)
	rm -f $(ASM_OBJS)
	rm -f $(BUILD_DIR)/BlinkOS.bin