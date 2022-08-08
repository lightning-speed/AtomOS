cd ..\atom-lib
call build
cd ..
call acc sys/bin/cmd.c sys/bin/cmd.exe 0x700000
.call acc sys/bin/cat.c sys/bin/cat.exe 0x900000
.call acc sys/bin/sw.c sys/bin/sw.exe 0x900000
.call acc sys/brainf.c sys/bf.exe 0x900000





cd sys
..\tools\mkfs  bf.exe code.bf gnome.exe unifont.bin car.gnm bin/sw.exe bin/cmd.exe bin/cat.exe ramdisk
copy ramdisk ..\disk\ramdisk
pause