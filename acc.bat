i686-elf-gcc -m32 -c %1 -std=gnu99 -I "E:\x86_64-elf-tools-windows\include" 
ld -Ttext %3 -e _start atom-lib/crt0.a  atom-lib/atlib.a *.o  -o prog.pe
objcopy -Felf32-i386 prog.pe  %2
del *.pe
del *.o