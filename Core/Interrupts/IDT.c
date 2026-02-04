#include <__KCONF.h>
#include <Plugs/Interrupts/IDT.h>

IDT_MANAGER* ActiveIDT = NULL;

int RegisterIDT(IDT_MANAGER* IDT, SYSTEM_ERROR* Error)
{
    #define ErrorOut_RegisterIDT(Code) \
        ErrorOut(Error, Code, FUNC_RegisterIDT);

    if(Probe4Error(IDT)             || 
       !IDT                         || 
       Probe4Error(IDT->Operations) ||
       !IDT->Operations)
    {
        ErrorOut_RegisterIDT(-BadParameters);
        return -BadParameters;
    }
    
    ActiveIDT = IDT;
    
    if(IDT->Operations->Init)
    {
        return IDT->Operations->Init(Error);
    }
    
    return GeneralOK;
}

IDT_MANAGER* GetIDT(SYSTEM_ERROR* Error)
{
    #define ErrorOut_GetIDT(Code) \
        ErrorOut(Error, Code, FUNC_GetIDT);

    if(Probe4Error(ActiveIDT) || !ActiveIDT)
    {
        ErrorOut_GetIDT(-NotFound);
        return Error2Pointer(-NotFound);
    }
    
    return ActiveIDT;
}

int SetIDTEntry(uint8_t Vector, IDT_ENTRY* Entry, SYSTEM_ERROR* Error)
{
    #define ErrorOut_SetIDTEntry(Code) \
        ErrorOut(Error, Code, FUNC_SetIDTEntry);

    if(Probe4Error(ActiveIDT) || !ActiveIDT || !ActiveIDT->Operations->SetEntry)
    {
        ErrorOut_SetIDTEntry(-NotFound);
        return -NotFound;
    }

    return ActiveIDT->Operations->SetEntry(Vector, Entry, Error);
}

int GetIDTEntry(uint8_t Vector, IDT_ENTRY* Entry, SYSTEM_ERROR* Error)
{
    #define ErrorOut_GetIDTEntry(Code) \
        ErrorOut(Error, Code, FUNC_GetIDTEntry);

    if(Probe4Error(ActiveIDT) || !ActiveIDT || !ActiveIDT->Operations->GetEntry)
    {
        ErrorOut_GetIDTEntry(-NotFound);
        return -NotFound;
    }

    return ActiveIDT->Operations->GetEntry(Vector, Entry, Error);
}

int LoadIDT(SYSTEM_ERROR* Error)
{
    #define ErrorOut_LoadIDT(Code) \
        ErrorOut(Error, Code, FUNC_LoadIDT);

    if(Probe4Error(ActiveIDT) || !ActiveIDT || !ActiveIDT->Operations->Load)
    {
        ErrorOut_LoadIDT(-NotFound);
        return -NotFound;
    }

    return ActiveIDT->Operations->Load(Error);
}

int InitIDT(IDT_MANAGER* IDT, SYSTEM_ERROR* Error)
{
    #define ErrorOut_InitIDT(Code) \
        ErrorOut(Error, Code, FUNC_InitIDT);

    if(Probe4Error(IDT)             || 
       !IDT                         ||
       Probe4Error(IDT->Operations) ||
       !IDT->Operations)
    {
        ErrorOut_InitIDT(-BadParameters);
        return -BadParameters;
    }

    return IDT->Operations->Init(Error);
}

int ExitIDT(IDT_MANAGER* IDT, SYSTEM_ERROR* Error)
{
    #define ErrorOut_ExitIDT(Code) \
        ErrorOut(Error, Code, FUNC_ExitIDT);

    if(Probe4Error(IDT)             || 
       !IDT                         ||
       Probe4Error(IDT->Operations) ||
       !IDT->Operations)
    {
        ErrorOut_ExitIDT(-BadParameters);
        return -BadParameters;
    }

    IDT->Operations->Exit(Error);

    return GeneralOK;
}
