BUILD_DIR = obj
ISO_DIR = iso

all:
	cd $(BUILD_DIR)&&make
	qemu-system-x86_64 -kernel kernel.elf -monitor stdio -initrd disk/ramdisk -serial file:serial.log
iso:
	cd $(ISO_DIR)&&make	
