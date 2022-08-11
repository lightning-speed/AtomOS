ENTRY(_start)

SECTIONS
{
    . = 0x100000;

    .boot :
    {
        *(.multiboot_header)
    }
		. = ALIGN(4K);
    .text : {
        *(.text)
    } 
     
    . += 0x1000;

    .rodata : {
        *(.rodata*)
    }

    . += 0x1000;

    .data : {
        *(.data*)
    } 

    .bss : {
        *(COMMON)
        *(.bss*)
    }
		. += 0x1000;
		heap_start = .;
		KERNEL_END = .;
		process_memory = 0x700000;   

}