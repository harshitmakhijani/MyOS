AS = nasm
CC = gcc
LD = ld

CFLAGS = -m32 -ffreestanding -Wall -Wextra
ASFLAGS = -f elf32
LDFLAGS = -m elf_i386 -T linker.ld

KERNEL = kernel.bin
ISO = MyOS.iso

all: $(ISO)

boot/boot.o: boot/boot.s
	$(AS) $(ASFLAGS) $< -o $@

kernel/kernel.o: kernel/kernel.c
	$(CC) $(CFLAGS) -c $< -o $@

$(KERNEL): boot/boot.o kernel/kernel.o linker.ld
	$(LD) $(LDFLAGS) -o $@ boot/boot.o kernel/kernel.o

$(ISO): $(KERNEL)
	mkdir -p isodir/boot/grub
	cp $(KERNEL) isodir/boot/kernel.bin
	cp grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o $(ISO) isodir

clean:
	rm -rf isodir $(KERNEL) $(ISO) boot/*.o kernel/*.o