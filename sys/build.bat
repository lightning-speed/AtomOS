cd ..\atom-lib
call build
cd ..
call acc sys/cmd.c sys/cmd.exe 0x700000
call acc sys/brainf.c sys/bf.exe 0x900000
call acc sys/bin/cat.c sys/bin/cat.exe 0x900000

cd sys
..\tools\mkfs test.c cmd.exe bf.exe code.bf bin/cat.exe  ramdisk
copy ramdisk ..\disk\ramdisk
pause