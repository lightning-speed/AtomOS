%define MULTIBOOT_HEADER_MAGIC 0x1badb002
%define MULTIBOOT_HEADER_FLAGS 0

extern multiboot_main
global _start

section .multiboot_header

align 4
header_start:
    dd MULTIBOOT_HEADER_MAGIC 	                                    
    dd MULTIBOOT_HEADER_FLAGS
    dd -(MULTIBOOT_HEADER_MAGIC + MULTIBOOT_HEADER_FLAGS)
header_end:



extern kmain
global _start

_start:
 mov esp,stack_end
 push ebx
 push eax
 call kmain
 jmp htr

htr:
 hlt
 jmp htr

SECTION .bss

stack_begin:
    RESB 0x6000  
stack_end:
