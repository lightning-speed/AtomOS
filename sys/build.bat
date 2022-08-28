cd ..\atom-lib
call build
cd ..
call acc sys/sys/cmd.c sys/sys/cmd.exe 0x700000
call acc sys/sys/cat.c sys/sys/cat.exe 0x900000
call acc sys/sys/sw.c sys/sys/sw.exe 0x900000
call acc sys/sys/brainf.c sys/sys/bf.exe 0x900000

call acc sys/sys/imv.c sys/sys/imv.exe 0x900000





cd sys
..\tools\mkfs  sys/bf.exe code.bf sys/gnome.exe lib/unifont.bin car.gnm sys/sw.exe sys/cmd.exe sys/cat.exe sys/imv.exe lib/icon.bmp ramdisk
copy ramdisk ..\disk\ramdisk
pause