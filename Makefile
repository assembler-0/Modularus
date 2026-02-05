CHAIN ?= gcc
TARGET_TRIPLE := x86_64-elf
HOST_CC = cc

#gcc
ifeq ($(CHAIN), gcc)
	CC := $(TARGET_TRIPLE)-gcc
	LD := $(TARGET_TRIPLE)-ld
	AS := nasm
#clang
else ifeq ($(CHAIN), clang)
	CC := clang --target=$(TARGET_TRIPLE)
	LD := ld.lld
	AS := nasm
endif

export PROJ_ROOT := $(CURDIR)
export BUILD_DIR := $(PROJ_ROOT)/.Build
ISO_ROOT 		 := $(BUILD_DIR)/iso_root
IMAGE_NAME 		 := modularus

ISO_IMG  := $(BUILD_DIR)/modularus.iso
DISK_IMG := $(BUILD_DIR)/modularus.img
KERNEL 	 := $(BUILD_DIR)/bin/modularus

OK_COLOR    := \033[0;32m #	green
INFO_COLOR  := \033[0;33m #	yellow (idea: how about cyan?)
ERROR_COLOR := \033[0;31m #	red
RESET       := \033[0m

# so that its cleaner
DEPS_DIR := $(PROJ_ROOT)/Dependencies

# some wraps
OVMF := $(DEPS_DIR)/edk2-ovmf/edk2-ovmf/ovmf-code-x86_64.fd

.PHONY: \
all \
clean \
distclean \
bear \
core \
fetch-deps

# Redundant
# qemu
# run-uefi

all: \
	fetch-deps \
	$(ISO_IMG) \
	$(DISK_IMG)
		@echo -e "$(OK_COLOR)[SUCCESS] Built targets using $(CHAIN) $(RESET)"

$(DEPS_DIR):
	@mkdir -p $(DEPS_DIR)

fetch-deps: \
	$(DEPS_DIR) \
	$(DEPS_DIR)/limine/limine \
	$(DEPS_DIR)/edk2-ovmf/edk2-ovmf/ovmf-code-x86_64.fd


$(DEPS_DIR)/limine/limine:
	@echo -e "$(INFO_COLOR)[DEPENDENCY] Cloning and building Limine...$(RESET)"

	@rm -rf $(DEPS_DIR)/limine
	@git clone https://codeberg.org/Limine/Limine.git $(DEPS_DIR)/limine \
		--branch=v10.x-binary \
		--depth=1
	@$(MAKE) -C $(DEPS_DIR)/limine CC="$(HOST_CC)" > /dev/null

$(DEPS_DIR)/edk2-ovmf/edk2-ovmf/ovmf-code-x86_64.fd:
	@echo -e "$(INFO_COLOR)[DEPENDENCY] Fetching OVMF for UEFI testing...$(RESET)"

	@rm -rf $(DEPS_DIR)/edk2-ovmf
	@mkdir -p $(DEPS_DIR)/edk2-ovmf
	@curl -L https://github.com/osdev0/edk2-ovmf-nightly/releases/latest/download/edk2-ovmf.tar.gz | \
		tar \
		-xz \
		-C $(DEPS_DIR)/edk2-ovmf

bear: \
clean
	@echo -e "$(INFO_COLOR)[INFO] Generating compilation database...$(RESET)"

	bear -- $(MAKE) \
		all CHAIN=$(CHAIN)

core:
	@echo -e "$(INFO_COLOR)[INFO] Building Core with $(CHAIN)...$(RESET)"

# call the kernel make
	@$(MAKE) \
		-C Core \
		CC="$(CC)" \
		LD="$(LD)" \
		AS="$(AS)" \
		CHAIN="$(CHAIN)"

$(ISO_IMG): core $(DEPS_DIR)/limine/limine
	@echo -e "$(INFO_COLOR)[ISO] Preparing iso_root and generating image...$(RESET)"

	@rm -rf $(ISO_ROOT)
	@mkdir -p $(ISO_ROOT)/boot/limine
	@mkdir -p $(ISO_ROOT)/EFI/BOOT
	@cp $(KERNEL) $(ISO_ROOT)/boot/

	@cp Boot/limine.conf \
		$(ISO_ROOT)/boot/limine/

	@cp $(DEPS_DIR)/limine/limine-bios.sys \
	    $(DEPS_DIR)/limine/limine-bios-cd.bin \
	    $(DEPS_DIR)/limine/limine-uefi-cd.bin \
	    $(ISO_ROOT)/boot/limine/
	@cp $(DEPS_DIR)/limine/BOOTX64.EFI \
	    $(DEPS_DIR)/limine/BOOTIA32.EFI \
	    $(ISO_ROOT)/EFI/BOOT/

	@xorriso \
		-as mkisofs \
		-R \
		-r \
		-J \
		-b boot/limine/limine-bios-cd.bin \
		-no-emul-boot \
		-boot-load-size 4 \
		-boot-info-table \
		-hfsplus \
		-apm-block-size 2048 \
		--efi-boot boot/limine/limine-uefi-cd.bin \
		-efi-boot-part \
		--efi-boot-image \
		--protective-msdos-label \
		$(ISO_ROOT) -o $@ > /dev/null 2>&1

	@$(DEPS_DIR)/limine/limine bios-install $@

	@echo -e "$(OK_COLOR)[SUCCESS] ISO created at $@$(RESET)"

$(DISK_IMG): core $(DEPS_DIR)/limine/limine
	@echo -e "$(INFO_COLOR)[HDD] Creating GPT Disk Image...$(RESET)"

	@rm -f $@
	@truncate -s 64M $@
	@sgdisk $@ -n 1:2048 -t 1:ef00 -m 1
	@$(DEPS_DIR)/limine/limine bios-install $@
	@mformat -i $@@@1M -F
	@mmd -i $@@@1M \
		::/boot \
		::/boot/limine \
		::/EFI \
		::/EFI/BOOT
	@mcopy -i $@@@1M \
		$(KERNEL) \
		::/boot
	@mcopy -i $@@@1M \
		Boot/limine.conf \
		$(DEPS_DIR)/limine/limine-bios.sys \
		::/boot/limine
	@mcopy -i $@@@1M \
		$(DEPS_DIR)/limine/BOOTX64.EFI \
	    $(DEPS_DIR)/limine/BOOTIA32.EFI \
		::/EFI/BOOT

	@echo -e "$(OK_COLOR)[SUCCESS] HDD created at $@$(RESET)"

# why the same thing twice? Run.sh already handles that
#qemu: $(ISO_IMG)
#	qemu-system-x86_64 -M q35 -m 512M -cdrom $(ISO_IMG) -boot d

#run-uefi: $(ISO_IMG) edk2-ovmf
#	qemu-system-x86_64 -M q35 -m 512M \
#		-drive if=pflash,unit=0,format=raw,file=$(DEPS_DIR)/edk2-ovmf/edk2-ovmf/ovmf-code-x86_64.fd,readonly=on \
#		-cdrom $(ISO_IMG) -boot d

clean:
	@$(MAKE) -C Core clean
	@rm -rf $(BUILD_DIR)

distclean: clean
	@rm -rf $(DEPS_DIR)
