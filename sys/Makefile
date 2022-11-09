CC = ../tools/compiler 
MKFS = ../tools/mkfs

all:
	$(CC) a.c -o f.exe -e 0x700000
	$(MKFS) f.exe ../disk/ramdisk
	
