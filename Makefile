PREFIX := $(HOME)/opt/cross/bin
AS := $(PREFIX)/i686-elf-as
CC := $(PREFIX)/i686-elf-gcc

CFLAGS := -std=gnu99 -ffreestanding -O2 -Wall -Wextra -Ikernel

all: myos.iso

boot/boot.o: boot/boot.s
	$(AS) boot/boot.s -o boot/boot.o

kernel/kernel.o: kernel/kernel.c kernel/terminal.h kernel/keyboard.h kernel/shell.h kernel/fs.h kernel/system_info.h kernel/disk.h
	$(CC) -c kernel/kernel.c -o kernel/kernel.o $(CFLAGS)

kernel/terminal.o: kernel/terminal.c kernel/terminal.h kernel/vga.h kernel/io.h kernel/string.h
	$(CC) -c kernel/terminal.c -o kernel/terminal.o $(CFLAGS)

kernel/keyboard.o: kernel/keyboard.c kernel/keyboard.h kernel/io.h kernel/terminal.h kernel/shell.h kernel/keymap_de.h
	$(CC) -c kernel/keyboard.c -o kernel/keyboard.o $(CFLAGS)

kernel/keymap_de.o: kernel/keymap_de.c kernel/keymap_de.h
	$(CC) -c kernel/keymap_de.c -o kernel/keymap_de.o $(CFLAGS)

kernel/shell.o: kernel/shell.c kernel/shell.h kernel/commands.h kernel/terminal.h kernel/string.h kernel/line_editor.h
	$(CC) -c kernel/shell.c -o kernel/shell.o $(CFLAGS)

kernel/line_editor.o: kernel/line_editor.c kernel/line_editor.h kernel/terminal.h
	$(CC) -c kernel/line_editor.c -o kernel/line_editor.o $(CFLAGS)

kernel/commands.o: kernel/commands.c kernel/commands.h kernel/terminal.h kernel/string.h kernel/fs.h kernel/panic.h kernel/reboot.h kernel/system_info.h kernel/parser.h kernel/disk.h kernel/shell.h kernel/option.h
	$(CC) -c kernel/commands.c -o kernel/commands.o $(CFLAGS)

kernel/fs.o: kernel/fs.c kernel/fs.h kernel/string.h kernel/disk.h
	$(CC) -c kernel/fs.c -o kernel/fs.o $(CFLAGS)

kernel/panic.o: kernel/panic.c kernel/panic.h kernel/terminal.h
	$(CC) -c kernel/panic.c -o kernel/panic.o $(CFLAGS)

kernel/parser.o: kernel/parser.c kernel/parser.h kernel/string.h
	$(CC) -c kernel/parser.c -o kernel/parser.o $(CFLAGS)

kernel/reboot.o: kernel/reboot.c kernel/reboot.h kernel/io.h
	$(CC) -c kernel/reboot.c -o kernel/reboot.o $(CFLAGS)

kernel/system_info.o: kernel/system_info.c kernel/system_info.h kernel/terminal.h
	$(CC) -c kernel/system_info.c -o kernel/system_info.o $(CFLAGS)

kernel/string.o: kernel/string.c kernel/string.h
	$(CC) -c kernel/string.c -o kernel/string.o $(CFLAGS)

kernel/disk.o: kernel/disk.c kernel/disk.h kernel/io.h
	$(CC) -c kernel/disk.c -o kernel/disk.o $(CFLAGS)

kernel/option.o: kernel/option.c kernel/option.h kernel/string.h kernel/terminal.h kernel/fs.h
	$(CC) -c kernel/option.c -o kernel/option.o $(CFLAGS)

myos.bin: boot/boot.o kernel/kernel.o kernel/terminal.o kernel/keyboard.o kernel/keymap_de.o kernel/shell.o kernel/line_editor.o kernel/commands.o kernel/fs.o kernel/panic.o kernel/parser.o kernel/reboot.o kernel/system_info.o kernel/string.o kernel/disk.o kernel/option.o linker.ld
	$(CC) -T linker.ld -o myos.bin -ffreestanding -O2 -nostdlib boot/boot.o kernel/kernel.o kernel/terminal.o kernel/keyboard.o kernel/keymap_de.o kernel/shell.o kernel/line_editor.o kernel/commands.o kernel/fs.o kernel/panic.o kernel/parser.o kernel/reboot.o kernel/system_info.o kernel/string.o kernel/disk.o kernel/option.o -lgcc

myos.iso: myos.bin isodir/boot/grub/grub.cfg
	cp myos.bin isodir/boot/myos.bin
	grub-mkrescue -o myos.iso isodir

run: myos.iso
	qemu-system-i386 -cdrom myos.iso -drive file=disk.img,format=raw,if=ide

clean:
	rm -f boot/*.o kernel/*.o myos.bin myos.iso isodir/boot/myos.bin
