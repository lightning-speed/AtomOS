del ramdisk
nasm -f  bin main.asm -o test.exe
cd ../
cd atom-lib
call build
cd ../
call acc tools/main.c tools/main.elf 0x300000
call acc tools/test.c tools/test.elf 0x700000
cd tools
echo eh
a.exe test.elf mkfs2.c main.elf ramdisk
copy ramdisk ..\disk\ramdisk
pause