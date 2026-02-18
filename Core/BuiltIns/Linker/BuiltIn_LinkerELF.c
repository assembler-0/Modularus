#include <BuiltIns/Linker/LinkerELF.h>
#include <KExports.h>
#include <Errors.h>
#include <BuiltIns/Logger/Formatter.h>
#include <System.h>

/*.ko*/

typedef int (*MODULE_ENTRY)(void);
typedef int (*MODULE_EXIT)(void);

static MODULE_ENTRY ModuleStart = 0;
static MODULE_EXIT ModuleExit = 0;

SYSTEM_NODE* LinkerNode = NULL;

int Linker_Open(SYSTEM_NODE* Node __unused, SYSTEM_FILE* File __unused, SYSTEM_ERROR* Error __unused)
{
    return GeneralOK;
}

int Linker_Close(SYSTEM_FILE* File __unused, SYSTEM_ERROR* Error __unused)
{
    return GeneralOK;
}

long Linker_Ioctl(SYSTEM_FILE* File __unused, uint64_t Request, void* Arguments, SYSTEM_ERROR* Error)
{
    #define ErrorOut_Linker_Ioctl(Code) \
        ErrorOut(Error, Code, General)

    switch(Request)
    {
        case LinkerCommand_LINK:
        {
            void* ModuleImage = Arguments;
            if (!ModuleImage || Probe4Error(ModuleImage))
            {
                ErrorOut_Linker_Ioctl(-EINVAL);
                return Error->ErrorCode;
            }

            if (!Module_Link(ModuleImage, Error))
            {
                return Error->ErrorCode;
            }
            return GeneralOK;
        }

        case LinkerCommand_RUN:
        {
            Module_Run(Error);
            return GeneralOK;
        }

        case LinkerCommand_EXIT:
        {
            Module_Exit(Error);
            return GeneralOK;
        }

        default:
        {
            ErrorOut_Linker_Ioctl(-EINVAL);
            return Error->ErrorCode;
        }
    }
}

int Linker_GetAttribute(SYSTEM_NODE* Node __unused, VFS_STAT* Stat, SYSTEM_ERROR* Error __unused)
{
    Stat->Size = 4096;
    return GeneralOK;
}

SYSTEM_OPERATIONS LinkerOperations =
{
    .Open   = Linker_Open,
    .Close  = Linker_Close,
    .Read   = NULL,
    .Write  = NULL,
    .Ioctl  = Linker_Ioctl,
    .Getattr= Linker_GetAttribute,
    .Setattr= NULL
};

void Linker_Init(SYSTEM_ERROR* Error)
{
    if (!LinkerNode)
    {
        LinkerNode = System_CreateNode("linker", SystemNodeTypeEnumeration_FILE, &LinkerOperations, NULL, 4096, Error);
        if (!Probe4Error(LinkerNode) && LinkerNode)
        {
            System_AttachNode(SystemRoot, LinkerNode, Error);
        }
    }
}

static uint64_t Module_SectionToVirtualAddress(void* ImageBase, ELF64_HEADER* Header, ELF64_SECTION_HEADER* Sections, uint16_t SectionIndex)
{
    if (SectionIndex >= Header->e_shnum)
    {
        return 0;
    }

    return (uint64_t)ImageBase + Sections[SectionIndex].sh_offset;
}

void* Module_Link(void* ImageBase, SYSTEM_ERROR* Error)
{
    #define ErrorOut_Module_Link(Code) \
        ErrorOut(Error, Code, General)

    ELF64_HEADER* ELFHeader = (ELF64_HEADER*)ImageBase;

    ELF64_SECTION_HEADER* SectionHeaders = (ELF64_SECTION_HEADER*)((uint8_t*)ImageBase + ELFHeader->e_shoff);

    ELF64_SYMBOL* SymbolTable = 0;
    const char* StringTable = 0;
    ELF64_SECTION_HEADER* SymbolSection = 0;

    for (uint16_t Index = 0; Index < ELFHeader->e_shnum; Index++)
    {
        ELF64_SECTION_HEADER* Section = &SectionHeaders[Index];

        if (Section->sh_type == SHT_SYMTAB)
        {
            SymbolSection = Section;
            SymbolTable = (ELF64_SYMBOL*)((uint8_t*)ImageBase + Section->sh_offset);
        }

        if (Section->sh_type == SHT_STRTAB && Index != ELFHeader->e_shstrndx)
        {
            StringTable = (const char*)((uint8_t*)ImageBase + Section->sh_offset);
        }
    }

    if (!SymbolTable || !StringTable)
    {
        ErrorOut_Module_Link(-ENOENT);
        return Error2Pointer(Error->ErrorCode);
    }

    uint32_t SymbolCount = SymbolSection->sh_size / sizeof(ELF64_SYMBOL);

    for (uint32_t Index = 0; Index < SymbolCount; Index++)
    {
        ELF64_SYMBOL* Symbol = &SymbolTable[Index];
        const char* Name = StringTable + Symbol->st_name;

        if (Symbol->st_shndx == SHN_UNDEF)
        {
            void* Address = LookUpKExport(Name, Error);

            if (!Address)
            {
                ErrorOut_Module_Link(-ENOENT);
                return Error2Pointer(Error->ErrorCode);
            }

            Symbol->st_value = (uint64_t)Address;
            Symbol->st_shndx = SHN_ABS;
        }
    }

    for (uint16_t SectionIndex = 0; SectionIndex < ELFHeader->e_shnum; SectionIndex++)
    {
        ELF64_SECTION_HEADER* RelocSection = &SectionHeaders[SectionIndex];

        if (RelocSection->sh_type != SHT_RELA)
        {
            continue;
        }

        ELF64_RELOCATION* Relocations = (ELF64_RELOCATION*)((uint8_t*)ImageBase + RelocSection->sh_offset);
        uint32_t RelocationCount = RelocSection->sh_size / sizeof(ELF64_RELOCATION);

        /*
        ELF64_SECTION_HEADER* TargetSection =
            &SectionHeaders[RelocSection->sh_info];
        */

        for (uint32_t Index = 0; Index < RelocationCount; Index++)
        {
            ELF64_RELOCATION* Relocation = &Relocations[Index];

            uint64_t Type = ELF64_R_TYPE(Relocation->r_info);
            uint64_t SymbolIndex = ELF64_R_SYM(Relocation->r_info);

            uint64_t TargetAddress = Module_SectionToVirtualAddress(ImageBase, ELFHeader, SectionHeaders, RelocSection->sh_info) + Relocation->r_offset;

            uint64_t SymbolValue;

            if (SymbolTable[SymbolIndex].st_shndx == SHN_ABS)
            {
                SymbolValue = SymbolTable[SymbolIndex].st_value;
            }
            else
            {
                SymbolValue = Module_SectionToVirtualAddress(ImageBase, ELFHeader, SectionHeaders, SymbolTable[SymbolIndex].st_shndx) + SymbolTable[SymbolIndex].st_value;
            }

            uint64_t Addend = Relocation->r_addend;

            switch (Type)
            {
                case R_X86_64_64:
                {
                    *(uint64_t*)TargetAddress = SymbolValue + Addend;
                    break;
                }

                case R_X86_64_RELATIVE:
                {
                    *(uint64_t*)TargetAddress = (uint64_t)ImageBase + Addend;
                    break;
                }

                case R_X86_64_32:
                {
                    *(uint32_t*)TargetAddress = (uint32_t)(SymbolValue + Addend);
                    break;
                }

                case R_X86_64_32S:
                {
                    *(int32_t*)TargetAddress = (int32_t)(SymbolValue + Addend);
                    break;
                }

                case R_X86_64_PC32:
                case R_X86_64_PLT32:
                {
                    *(int32_t*)TargetAddress = (int32_t)(SymbolValue + Addend - TargetAddress);
                    break;
                }

                default:
                {
                    ErrorOut_Module_Link(-EINVAL);
                    return Error2Pointer(Error->ErrorCode);
                }
            }
        }
    }

    for (uint32_t Index = 0; Index < SymbolCount; Index++)
    {
        const char* Name = StringTable + SymbolTable[Index].st_name;

        if (!strcmp(Name, "_start"))
        {
            ModuleStart = (MODULE_ENTRY)(Module_SectionToVirtualAddress(ImageBase, ELFHeader, SectionHeaders, SymbolTable[Index].st_shndx) + SymbolTable[Index].st_value);
        }

        if (!strcmp(Name, "_exit"))
        {
            ModuleExit = (MODULE_EXIT)(Module_SectionToVirtualAddress(ImageBase, ELFHeader, SectionHeaders, SymbolTable[Index].st_shndx) + SymbolTable[Index].st_value);
        }
    }

    if (!ModuleStart)
    {
        ErrorOut_Module_Link(-ENOENT);
        return Error2Pointer(Error->ErrorCode);
    }

    return ImageBase;
}

void Module_Run(SYSTEM_ERROR* Error __unused)
{
    ModuleStart();
}


void Module_Exit(SYSTEM_ERROR* Error __unused)
{
    ModuleExit();
}