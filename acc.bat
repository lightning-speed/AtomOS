i686-elf-gcc -m32 -c %1 -std=gnu99 -I atom-lib/li  -O2 -nostdlib -fno-exceptions  -mno-red-zone -pipe
ld -Ttext %3 -e _start   atom-lib/*.o *.o  -o prog.pe -nostdlib
objcopy -Felf32-i386 prog.pe  %2
del *.pe
del *.o