#color
RED    := \033[0;31m
YELLOW := \033[0;33m
GREEN  := \033[0;32m
RESET  := \033[0m

BuildDirectory := .Build
EFIRoot        := $(BuildDirectory)/EFI
DiskImg        := $(BuildDirectory)/hotswap.img
IsoImg := $(BuildDirectory)/hotswap.iso

Limine := limine
CoreHotKernel := Core/.Build/bin/corehotkrnl

.PHONY: \
Build \
clean \
CoreHotKernel \
FinalImg \
FinalISO

Build: CoreHotKernel FinalImg FinalISO
	@echo "$(GREEN)[SUCCESS] build completed successfully$(RESET)"

CoreHotKernel:
	@mkdir -p $(BuildDirectory) $(EFIRoot)
	@echo "$(YELLOW)[INFO] Building Core $(RESET)"
	@$(MAKE) -C Core || (echo "$(RED)[ERROR] Core build failed$(RESET)" && exit 1)
	@cp $(CoreHotKernel) $(EFIRoot)/corehotkrnl
	@echo "$(GREEN)[SUCCESS] kernel copied to EFI root$(RESET)"

#.img
FinalImg: CoreHotKernel
	@echo "$(YELLOW)[INFO] creating test disk image...$(RESET)"
	@rm -f "$(DiskImg)"
	@mkdir -p $(EFIRoot)/EFI/BOOT
	@cp $(Limine)/BOOTX64.EFI $(EFIRoot)/EFI/BOOT/
	@cp Boot/limine.cfg $(EFIRoot)/
	@dd if=/dev/zero of=$(DiskImg) bs=1M count=64
	@parted $(DiskImg) mklabel gpt
	@parted $(DiskImg) mkpart primary fat32 1MiB 100%
	@parted $(DiskImg) set 1 esp on
	@sudo losetup -P /dev/loop0 $(DiskImg)
	@sudo mkfs.fat -F32 /dev/loop0p1
	@sudo mount /dev/loop0p1 /mnt
	@sudo cp -r $(EFIRoot)/* /mnt/
	@sudo umount /mnt
	@sudo losetup -d /dev/loop0
	@echo "$(GREEN)[SUCCESS] disk image created at: $(DiskImg)$(RESET)"

#.iso
FinalISO: CoreHotKernel
	@echo "$(YELLOW)[INFO] creating bootable ISO image with Limine...$(RESET)"
	@rm -f "$(IsoImg)"
	@mkdir -p $(EFIRoot)/EFI/BOOT
	@cp $(Limine)/BOOTX64.EFI $(EFIRoot)/EFI/BOOT/
	@cp Boot/limine.cfg $(EFIRoot)/
	@cp $(Limine)/limine-bios-cd.bin $(EFIRoot)/
	@cp $(Limine)/limine-uefi-cd.bin $(EFIRoot)/
	@xorriso -as mkisofs \
		-b limine-bios-cd.bin \
		-no-emul-boot -boot-load-size 4 -boot-info-table \
		--efi-boot limine-uefi-cd.bin \
		--protective-msdos-label \
		-o "$(IsoImg)" \
		"$(EFIRoot)"
	@echo "$(GREEN)[SUCCESS] ISO image created at: $(IsoImg)$(RESET)"

clean:
	@echo "$(YELLOW)[INFO] cleaning build...$(RESET)"
	@rm -rf $(BuildDirectory)
	@$(MAKE) -C Core clean
	@echo "$(GREEN)[SUCCESS] clean complete$(RESET)"