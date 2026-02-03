#include <__KCONF.h>
#include <Plugs/Memory/PhysicalMemoryManager.h>

PMM_MANAGER* ActivePMM = NULL;

int RegisterPMM(PMM_MANAGER* PMM, SYSTEM_ERROR* Error)
{
    #define ErrorOut_RegisterPMM(Code) \
        ErrorOut(Error, Code, FUNC_RegisterPMM);

    if(Probe4Error(PMM)             || 
       !PMM                         || 
       Probe4Error(PMM->Operations) ||
       !PMM->Operations)
    {
        ErrorOut_RegisterPMM(-BadParameters);
        return -BadParameters;
    }
    
    ActivePMM = PMM;
    
    if(PMM->Operations->Init)
    {
        return PMM->Operations->Init(Error);
    }
    
    return GeneralOK;
}

PMM_MANAGER* GetPMM(SYSTEM_ERROR* Error)
{
    #define ErrorOut_GetPMM(Code) \
        ErrorOut(Error, Code, FUNC_GetPMM);

    if(Probe4Error(ActivePMM) || !ActivePMM)
    {
        ErrorOut_GetPMM(-NotFound);
        return Error2Pointer(-NotFound);
    }
    
    return ActivePMM;
}

/*Ops*/

uint64_t AllocatePages(uint64_t Count, SYSTEM_ERROR* Error)
{
    #define ErrorOut_AllocatePages(Code) \
        ErrorOut(Error, Code, FUNC_AllocatePages);

    if(Probe4Error(ActivePMM) || !ActivePMM || !ActivePMM->Operations->AllocatePages)
    {
        ErrorOut_AllocatePages(-NotFound);
        return (uint64_t)Error2Pointer(-NotFound);
    }

    return ActivePMM->Operations->AllocatePages(Count, Error);
}

void FreePages(uint64_t Address, uint64_t Count, SYSTEM_ERROR* Error)
{
    #define ErrorOut_FreePages(Code) \
        ErrorOut(Error, Code, FUNC_FreePages);

    if(Probe4Error(ActivePMM) || !ActivePMM || !ActivePMM->Operations->FreePages)
    {
        ErrorOut_FreePages(-NotFound);
        return;
    }

    ActivePMM->Operations->FreePages(Address, Count, Error);
}

int InitPMM(PMM_MANAGER* PMM, SYSTEM_ERROR* Error)
{
    #define ErrorOut_InitPMM(Code) \
        ErrorOut(Error, Code, FUNC_InitPMM);

    if(Probe4Error(PMM)             || 
       !PMM                         ||
       Probe4Error(PMM->Operations) ||
       !PMM->Operations)
    {
        ErrorOut_InitPMM(-BadParameters);
        return -BadParameters;
    }

    return PMM->Operations->Init(Error);
}

int ExitPMM(PMM_MANAGER* PMM, SYSTEM_ERROR* Error)
{
    #define ErrorOut_ExitPMM(Code) \
        ErrorOut(Error, Code, FUNC_ExitPMM);

    if(Probe4Error(PMM)             || 
       !PMM                         ||
       Probe4Error(PMM->Operations) ||
       !PMM->Operations)
    {
        ErrorOut_ExitPMM(-BadParameters);
        return -BadParameters;
    }

    PMM->Operations->Exit(Error);

    return GeneralOK;
}
