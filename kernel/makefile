CC = x86_64-elf-gcc
OBJCOPY = x86_64-elf-objcopy
ASM = nasm

BUILD = ../build

C_FLAGS = \
	-Wall \
	-Wextra \
	-std=gnu11 \
	-ffreestanding \
	-fno-stack-protector \
	-fno-stack-check \
	-fno-lto \
	-fno-PIC \
	-ffunction-sections \
	-fdata-sections \
	-m64 \
	-march=x86-64 \
	-mabi=sysv \
	-mno-80387 \
	-mno-mmx \
	-mno-sse \
	-mno-sse2 \
	-mno-red-zone \
	-mcmodel=kernel \
	-Iinclude \
	-Iinclude/lib

LD_FLAGS = \
	-ffreestanding \
	-nostdlib \
	-T arch/x64/boot/linker.ld

C_SRCS = $(shell find . -iname "*.c" -printf "%P\n")
C_OBJS = $(patsubst %.c, $(BUILD)/kernel/%.c.o, $(C_SRCS))
ASM_SRCS = $(shell find . -iname "*.asm" -printf "%P\n")
ASM_OBJS = $(patsubst %.asm, $(BUILD)/kernel/%.asm.o, $(ASM_SRCS))
FONT_SRCS = $(shell find . -iname "*.psf" -printf "%P\n")
FONT_OBJS = $(patsubst %.psf, $(BUILD)/kernel/%.psf.o, $(FONT_SRCS))
	 
OBJS = $(C_OBJS) $(ASM_OBJS) $(FONT_OBJS)

all: $(BUILD)/meerOS_kernel.bin
$(BUILD)/kernel/%.c.o: %.c
	@printf "CC\t\t%s\n" $(patsubst ../%,%,$@)
	@mkdir -p $(dir $@)
	@$(CC) -c $< -o $@ $(C_FLAGS)
$(BUILD)/kernel/%.asm.o: %.asm
	@printf "NASM (64)\t%s\n" $(patsubst ../%,%,$@)
	@mkdir -p $(dir $@)
	@$(ASM) -f elf64 $< -o $@
$(BUILD)/kernel/%.psf.o: %.psf
	@printf "OBJCOPY (FONT)\t%s\n" $(patsubst ../%,%,$@)
	@mkdir -p $(dir $@)
	@$(OBJCOPY) -O elf64-x86-64 -B i386 -I binary $< $@
#	@readelf -sW $@
$(BUILD)/meerOS_kernel.bin: $(OBJS)
	@printf "LD\t\t%s\n" $(patsubst ../%,%,$@)
	@$(CC) $^ -o $@ $(LD_FLAGS)
