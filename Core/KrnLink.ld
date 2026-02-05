OUTPUT_FORMAT("elf64-x86-64", "elf64-x86-64", "elf64-x86-64")
OUTPUT_ARCH(i386:x86-64)

ENTRY(KernelBoot)

PHDRS {
    requests    PT_LOAD    FLAGS((1 << 1) | (1 << 2)); /* Write + Read */
    text        PT_LOAD    FLAGS((1 << 0) | (1 << 2)); /* Execute + Read */
    rodata      PT_LOAD    FLAGS((1 << 2));            /* Read only */
    data        PT_LOAD    FLAGS((1 << 1) | (1 << 2)); /* Write + Read */
}

SECTIONS
{
    . = 0xffffffff80000000;
    
    .requests : {
        KEEP(*(.requests_start))
        KEEP(*(.requests))
        KEEP(*(.requests_end))
    } :requests

    . = ALIGN(CONSTANT(MAXPAGESIZE));

    .text : {
        *(.text .text.*)
    } :text

    . = ALIGN(CONSTANT(MAXPAGESIZE));
    
    .rodata : ALIGN(4K)
    {
        *(.rodata .rodata.*)
    } :rodata

	.kexports : ALIGN(4K)
	{ 
		__start_kexports = .;
		KEEP(*(.kexports))
		__stop_kexports = .;
	} :rodata

    . = ALIGN(CONSTANT(MAXPAGESIZE));
    
    .data : ALIGN(4K)
    {
        *(.data .data.*)
    } :data
    
    .bss : ALIGN(4K)
    {
        *(COMMON)
        *(.bss .bss.*)
    } :data
    
    /DISCARD/ :
    {
        *(.eh_frame)
        *(.note .note.*)
        *(.comment)
    }
}