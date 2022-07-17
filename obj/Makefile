CC = i686-elf-g++
CFLAGS = -c -m32 -std=c++11 -I ../include -w -fno-exceptions -O2 -mno-red-zone -pipe
ASM = nasm
AFLAGS  = -felf32
CSRC = $(wildcard ../kernel/*.cpp)
DSRC = $(wildcard ../lib/*.cpp)
LSRC = $(wildcard ../drivers/*.cpp)
SSRC = $(wildcard ../syscall/*.cpp)
GSRC = $(wildcard ../Glib/*.cpp)


LD = i686-elf-ld
LFLAGS = -T ../linker.ld


all: 
	$(MAKE) Abuild
	$(MAKE) Cbuild
	$(MAKE) Dbuild
	$(MAKE) Lbuild
	$(MAKE) Sbuild 
	$(MAKE) Gbuild

	$(MAKE) Link
	$(MAKE) clean

Cbuild: $(CSRC)
	$(CC) $(CFLAGS) $^ 

Lbuild: $(LSRC)
	$(CC) $(CFLAGS) $^ 

Dbuild: $(DSRC)
	$(CC) $(CFLAGS) $^ 

Sbuild: $(SSRC)
	$(CC)  $(CFLAGS) $^ 	

Gbuild: $(GSRC)
	$(CC) $(CFLAGS) $^ 	


Abuild: 
	$(ASM) $(AFLAGS) ../kernel/entry.asm -o entry.po
	$(ASM) $(AFLAGS) ../kernel/gdt.asm -o gdt_a.o
	$(ASM) $(AFLAGS) ../kernel/Interrupts.asm -o int_a.o


Link:
	$(LD) $(LFLAGS)  -melf_i386 *.po *.o  -o ../kernel.elf
 
clean:
