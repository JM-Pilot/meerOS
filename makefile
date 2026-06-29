MAKEFLAGS += \
	--no-print-directory \
	-j$(nproc)

BUILD = build
ISO_DIR = $(BUILD)/iso
TOOLS = tools

all: $(BUILD)/meerOS.iso

# make the kernel
$(BUILD)/meerOS_kernel.bin:
	@$(MAKE) -C kernel

# copy the limine-binary and build the tools
$(TOOLS)/limine-binary:
	@printf "CURL\t\tlimine-binary.tar.gz\n"
	@curl -L -sS \
	https://github.com/Limine-Bootloader/Limine/releases/latest/download/limine-binary.tar.gz \
	| gunzip | tar -xf - --one-top-level=$(dir $@)
	@$(MAKE) -s -C $@

# copy the ovmf binaries for the uefi booting
$(TOOLS)/edk2-ovmf-bins:
	@curl -L -sS \
	https://github.com/osdev0/edk2-ovmf-stable-bins/releases/latest/download/edk2-ovmf-bins.tar.gz | gunzip | tar -xf - --one-top-level=$(dir $@)

# make the iso
$(BUILD)/meerOS.iso: $(BUILD)/meerOS_kernel.bin $(TOOLS)/limine-binary
	@printf "MAKING ISO"
	@mkdir -p $(ISO_DIR)/boot/limine
	@cp kernel/boot/limine.conf $(ISO_DIR)/boot/limine
	@cp $(BUILD)/meerOS_kernel.bin $(ISO_DIR)
	@cp $(TOOLS)/limine-binary/BOOTX64.EFI $(ISO_DIR)/boot/limine
	@cp $(TOOLS)/limine-binary/*.bin $(ISO_DIR)/boot/limine
	@cp $(TOOLS)/limine-binary/limine-bios.sys $(ISO_DIR)/boot/limine
	@xorriso -as mkisofs -R -r -J -b boot/limine/limine-bios-cd.bin \
        	-no-emul-boot -boot-load-size 4 -boot-info-table -hfsplus \
        	-apm-block-size 2048 --efi-boot boot/limine/limine-uefi-cd.bin \
        	-efi-boot-part --efi-boot-image --protective-msdos-label \
        	$(ISO_DIR) -o $@ >/dev/null 2>&1
	@$(TOOLS)/limine-binary/limine bios-install $@ >/dev/null 2>&1
	@printf " DONE!\n"

# run it in uefi 
run-efi: $(TOOLS)/edk2-ovmf-bins $(BUILD)/meerOS.iso
	@printf "RUNNING ON QEMU\n"
	@qemu-system-x86_64 -cdrom $(BUILD)/meerOS.iso \
		-drive if=pflash,format=raw,readonly=on,file=$(TOOLS)/edk2-ovmf-bins/ovmf-code-x86_64.fd \
		-drive if=pflash,format=raw,file=$(TOOLS)/edk2-ovmf-bins/ovmf-vars-x86_64.fd \
		-m 512M \
		-d int,cpu_reset -D $(BUILD)/QEMU_LOGS.txt
clean:
	rm -rf $(BUILD)
clean-tools:
	rm -rf $(TOOLS)