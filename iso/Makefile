FM = mkisofs
KERNEL_PATH = ../kernel.elf
LIMINE_PATH = ../limine
OUTPUT_FILE = Atom.iso
FM_FLAGS = -U --iso-level 3 
LID = ../tools/limine-deploy
CFG = ../limine.cfg
QEMU = qemu-system-x86_64
all:
	cp $(KERNEL_PATH) ./
	cp $(CFG)  ./
	cp ../disk/ramdisk  ./
	-rm -f $(OUTPUT_FILE)
	$(FM) $(FM_FLAGS) -o $(OUTPUT_FILE) ./ $(LIMINE_PATH)
	$(LID) $(OUTPUT_FILE) --force-mbr
	$(QEMU) $(OUTPUT_FILE)