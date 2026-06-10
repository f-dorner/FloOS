# FloOS

FloOS is a small hobby operating system project for x86.

It boots into VGA text mode, has a basic shell, keyboard input, a tiny file system, and some simple commands like `help`, `ls`, `touch`, `write`, `cat`, and `rm` yet, more will come.

## Features

- Multiboot boot flow through GRUB
- VGA text mode terminal
- German keyboard layout
- Basic shell and line editor
- Simple command system
- Tiny disk-backed file system
- ATA sector read/write experiments

## Requirements

- `i686-elf-gcc`
- `i686-elf-as`
- `grub-mkrescue`
- `xorriso`
- `qemu-system-i386`
- `make`

The Makefile currently expects the cross compiler here:

```sh
~/opt/cross/bin/i686-elf-gcc
~/opt/cross/bin/i686-elf-as
```

## Build

```sh
make
```

This creates `myos.iso`.

## Run

```sh
make run
```

This starts the OS in QEMU and attaches `disk.img` as an IDE disk.

## Clean

```sh
make clean
```

## Notes

The file system is still very small and experimental. It currently uses fixed sectors and a fixed number of file entries.

Warning: Do not boot this on real hardware. This project has no safety precautions yet and is meant to run in QEMU.
