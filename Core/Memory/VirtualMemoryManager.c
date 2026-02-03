#include <__KCONF.h>
#include <Plugs/Memory/VirtualMemoryManager.h>

VMM_MANAGER* ActiveVMM = NULL;

int RegisterVMM(VMM_MANAGER* VMM, SYSTEM_ERROR* Error)
{
    #define ErrorOut_RegisterVMM(Code) \
        ErrorOut(Error, Code, FUNC_RegisterVMM);

    if(Probe4Error(VMM)             || 
       !VMM                         || 
       Probe4Error(VMM->Operations) ||
       !VMM->Operations)
    {
        ErrorOut_RegisterVMM(-BadParameters);
        return -BadParameters;
    }
    
    ActiveVMM = VMM;
    
    if(VMM->Operations->Init)
    {
        return VMM->Operations->Init(Error);
    }
    
    return GeneralOK;
}

VMM_MANAGER* GetVMM(SYSTEM_ERROR* Error)
{
    #define ErrorOut_GetVMM(Code) \
        ErrorOut(Error, Code, FUNC_GetVMM);

    if(Probe4Error(ActiveVMM) || !ActiveVMM)
    {
        ErrorOut_GetVMM(-NotFound);
        return Error2Pointer(-NotFound);
    }
    
    return ActiveVMM;
}

int MapPage(uint64_t Virtual, uint64_t Physical, uint64_t Flags, SYSTEM_ERROR* Error)
{
    #define ErrorOut_MapPage(Code) \
        ErrorOut(Error, Code, FUNC_MapPage);

    if(Probe4Error(ActiveVMM) || !ActiveVMM || !ActiveVMM->Operations->MapPage)
    {
        ErrorOut_MapPage(-NotFound);
        return -NotFound;
    }

    return ActiveVMM->Operations->MapPage(Virtual, Physical, Flags, Error);
}

int UnMapPage(uint64_t Virtual, SYSTEM_ERROR* Error)
{
    #define ErrorOut_UnMapPage(Code) \
        ErrorOut(Error, Code, FUNC_UnMapPage);

    if(Probe4Error(ActiveVMM) || !ActiveVMM || !ActiveVMM->Operations->UnmapPage)
    {
        ErrorOut_UnMapPage(-NotFound);
        return -NotFound;
    }

    return ActiveVMM->Operations->UnmapPage(Virtual, Error);
}

uint64_t GetPhysicalAddress(uint64_t Virtual, SYSTEM_ERROR* Error)
{
    #define ErrorOut_GetPhysicalAddress(Code) \
        ErrorOut(Error, Code, FUNC_GetPhysicalAddress);

    if(Probe4Error(ActiveVMM) || !ActiveVMM || !ActiveVMM->Operations->GetPhysical)
    {
        ErrorOut_GetPhysicalAddress(-NotFound);
        return (uint64_t)Error2Pointer(-NotFound);
    }

    return ActiveVMM->Operations->GetPhysical(Virtual, Error);
}

int SetPageFlags(uint64_t Virtual, uint64_t Flags, SYSTEM_ERROR* Error)
{
    #define ErrorOut_SetPageFlags(Code) \
        ErrorOut(Error, Code, FUNC_SetPageFlags);

    if(Probe4Error(ActiveVMM) || !ActiveVMM || !ActiveVMM->Operations->SetFlags)
    {
        ErrorOut_SetPageFlags(-NotFound);
        return -NotFound;
    }

    return ActiveVMM->Operations->SetFlags(Virtual, Flags, Error);
}

uint64_t GetPageFlags(uint64_t Virtual, SYSTEM_ERROR* Error)
{
    #define ErrorOut_GetPageFlags(Code) \
        ErrorOut(Error, Code, FUNC_GetPageFlags);

    if(Probe4Error(ActiveVMM) || !ActiveVMM || !ActiveVMM->Operations->GetFlags)
    {
        ErrorOut_GetPageFlags(-NotFound);
        return (uint64_t)Error2Pointer(-NotFound);
    }

    return ActiveVMM->Operations->GetFlags(Virtual, Error);
}

void* GetPageTable(SYSTEM_ERROR* Error)
{
    #define ErrorOut_GetPageTable(Code) \
        ErrorOut(Error, Code, FUNC_GetPageTable);

    if(Probe4Error(ActiveVMM) || !ActiveVMM || !ActiveVMM->Operations->GetPageTable)
    {
        ErrorOut_GetPageTable(-NotFound);
        return Error2Pointer(-NotFound);
    }

    return ActiveVMM->Operations->GetPageTable(Error);
}

int SwitchPageTable(void* PageTable, SYSTEM_ERROR* Error)
{
    #define ErrorOut_SwitchPageTable(Code) \
        ErrorOut(Error, Code, FUNC_SwitchPageTable);

    if(Probe4Error(ActiveVMM) || !ActiveVMM || !ActiveVMM->Operations->SwitchPageTable)
    {
        ErrorOut_SwitchPageTable(-NotFound);
        return -NotFound;
    }

    return ActiveVMM->Operations->SwitchPageTable(PageTable, Error);
}

int InitVMM(VMM_MANAGER* VMM, SYSTEM_ERROR* Error)
{
    #define ErrorOut_InitVMM(Code) \
        ErrorOut(Error, Code, FUNC_InitVMM);

    if(Probe4Error(VMM)             || 
       !VMM                         ||
       Probe4Error(VMM->Operations) ||
       !VMM->Operations)
    {
        ErrorOut_InitVMM(-BadParameters);
        return -BadParameters;
    }

    return VMM->Operations->Init(Error);
}

int ExitVMM(VMM_MANAGER* VMM, SYSTEM_ERROR* Error)
{
    #define ErrorOut_ExitVMM(Code) \
        ErrorOut(Error, Code, FUNC_ExitVMM);

    if(Probe4Error(VMM)             || 
       !VMM                         ||
       Probe4Error(VMM->Operations) ||
       !VMM->Operations)
    {
        ErrorOut_ExitVMM(-BadParameters);
        return -BadParameters;
    }

    VMM->Operations->Exit(Error);

    return GeneralOK;
}
