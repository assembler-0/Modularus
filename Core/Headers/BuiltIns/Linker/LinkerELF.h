#pragma once

#include <__KCONF.h>
#include <Errors.h>

/*Mostly all are standard*/

/*.ko*/

#define EI_NIDENT 16

typedef struct ELF64_HEADER
{
    unsigned char e_ident[EI_NIDENT];
    uint16_t e_type;
    uint16_t e_machine;
    uint32_t e_version;
    uint64_t e_entry;
    uint64_t e_phoff;
    uint64_t e_shoff;
    uint32_t e_flags;
    uint16_t e_ehsize;
    uint16_t e_phentsize;
    uint16_t e_phnum;
    uint16_t e_shentsize;
    uint16_t e_shnum;
    uint16_t e_shstrndx;
} ELF64_HEADER;

typedef struct ELF64_SECTION_HEADER
{
    uint32_t sh_name;
    uint32_t sh_type;
    uint64_t sh_flags;
    uint64_t sh_addr;
    uint64_t sh_offset;
    uint64_t sh_size;
    uint32_t sh_link;
    uint32_t sh_info;
    uint64_t sh_addralign;
    uint64_t sh_entsize;
} ELF64_SECTION_HEADER;

typedef struct ELF64_SYMBOL
{
    uint32_t st_name;
    unsigned char st_info;
    unsigned char st_other;
    uint16_t st_shndx;
    uint64_t st_value;
    uint64_t st_size;
} ELF64_SYMBOL;

typedef struct ELF64_RELOCATION
{
    uint64_t r_offset;
    uint64_t r_info;
    int64_t  r_addend;
} ELF64_RELOCATION;

typedef struct ELF64_PROGRAM_HEADER
{
    uint32_t p_type;  
    uint32_t p_flags; 
    uint64_t p_offset;
    uint64_t p_vaddr; 
    uint64_t p_paddr; 
    uint64_t p_filesz;
    uint64_t p_memsz; 
    uint64_t p_align; 
} ELF64_PROGRAM_HEADER;

#define PT_NULL 0
#define PT_LOAD 1
#define PT_DYNAMIC 2
#define PT_INTERP 3
#define PT_NOTE 4
#define PT_PHDR 6

#define ELF64_R_SYM(i) ((i) >> 32)
#define ELF64_R_TYPE(i) ((i) & 0xffffffff)

#define SHT_SYMTAB 2
#define SHT_STRTAB 3
#define SHT_RELA 4

#define SHN_UNDEF 0

#define SHN_ABS 0xFFF1

#define R_X86_64_64 1
#define R_X86_64_PC32 2
#define R_X86_64_RELATIVE 8
#define R_X86_64_32S 11
#define R_X86_64_PLT32 4
#define R_X86_64_32 10

void* Module_Link(void*, SYSTEM_ERROR*);
void Module_Run(SYSTEM_ERROR*);