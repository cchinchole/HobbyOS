OUT_FILE_NAME = myos

GAS = i686-elf-as
GCC = i686-elf-gcc
OBJCOPY = i686-elf-objcopy

ABS_DIR = /home/christian/src/HobbyOS

LINKER_SCRIPT = linker.ld
GRUB_CFG = grub.cfg

INC_FILES=include/

COMPILER_FLAGS = -c -ffreestanding -O2 -Wall -Wextra -std=gnu99 -ggdb -I$(INC_FILES)
LINKER_FLAGS = -T $(LINKER_SCRIPT) -ffreestanding -O2 -nostdlib -lgcc -ggdb

ASM_FILES := $(wildcard boot/*.asm)
ASM_OBJ_FILES := $(ASM_FILES:.asm=.o)

C_FILES := $(wildcard kernel/*.c libc/*.c arch/i686/*.c drivers/*.c)
C_OBJ_FILES := $(C_FILES:.c=.o)

assemble: $(ASM_OBJ_FILES)

compile: $(C_OBJ_FILES)

link: $(addprefix build/, $(ASM_OBJ_FILES)) $(addprefix build/, $(C_OBJ_FILES))
	$(GCC) $(LINKER_FLAGS) -o build/$(OUT_FILE_NAME).bin $^

grub-iso: link
	mkdir -p build/isodir
	mkdir -p build/isodir/boot
	cp build/$(OUT_FILE_NAME).bin build/isodir/boot/$(OUT_FILE_NAME).bin
	mkdir -p build/isodir/boot/grub
	cp $(GRUB_CFG) build/isodir/boot/grub/$(GRUB_CFG)
	grub-mkrescue -o $(OUT_FILE_NAME).iso build/isodir

run: grub-iso
	qemu-system-i386 -cdrom $(OUT_FILE_NAME).iso -m 128 -curses

clean:
	rm -rf build/
	
build/%.o: %.asm
	$(shell mkdir -p $(ABS_DIR)/$(dir $@))
	$(GAS) -o $(ABS_DIR)/$@ $<
build/%.o: %.c
	$(shell mkdir -p $(ABS_DIR)/$(dir $@))
	$(GCC) $(COMPILER_FLAGS) -o $(ABS_DIR)/$@ $<