AS = nasm
CC = gcc
LD = ld

CFLAGS = -m32 -ffreestanding -Wall -Wextra
ASFLAGS = -f elf32
LDFLAGS = -m elf_i386 -T linker.ld

KERNEL = kernel.bin
ISO = MyOS.iso

C_SOURCES = \
	kernel/kernel.c \
	kernel/gdt.c \
	kernel/idt.c \
	kernel/keyboard.c \
	kernel/ports.c \
	kernel/pic.c

C_OBJECTS = $(C_SOURCES:.c=.o)

ASM_SOURCES = \
	boot/boot.s \
	kernel/gdt_flush.s \
	kernel/interrupts.s

ASM_OBJECTS = $(ASM_SOURCES:.s=.o)

all: $(ISO)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

%.o: %.s
	$(AS) $(ASFLAGS) $< -o $@

$(KERNEL): $(C_OBJECTS) $(ASM_OBJECTS) linker.ld
	$(LD) $(LDFLAGS) -o $@ $(ASM_OBJECTS) $(C_OBJECTS)

$(ISO): $(KERNEL)
	mkdir -p isodir/boot/grub
	cp $(KERNEL) isodir/boot/kernel.bin
	cp grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o $(ISO) isodir

clean:
	rm -rf isodir $(KERNEL) $(ISO) \
		kernel/*.o boot/*.o