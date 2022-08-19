SECTION .ramdisk
global ramdisk_start
global ramdisk_end

ramdisk_start db '',0x0
incbin "../disk/ramdisk"
ramdisk_end db '$$%@#',0x1
