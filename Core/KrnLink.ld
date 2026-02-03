ENTRY(_start)

SECTIONS
{
    . = 0xffffffff80000000;
    
    .text : ALIGN(4K)
    {
        *(.text .text.*)
    }
    
    .rodata : ALIGN(4K)
    {
        *(.rodata .rodata.*)
    }

	.kexports : ALIGN(4K)
	{ 
		__start_kexports = .;
		KEEP(*(.kexports))
		__stop_kexports = .;
	}
    
    .data : ALIGN(4K)
    {
        *(.data .data.*)
    }
    
    .bss : ALIGN(4K)
    {
        *(COMMON)
        *(.bss .bss.*)
    }
    
    /DISCARD/ :
    {
        *(.eh_frame)
        *(.note .note.*)
        *(.comment)
    }
}
