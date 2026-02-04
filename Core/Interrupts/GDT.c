#include <__KCONF.h>
#include <Plugs/Interrupts/GDT.h>

GDT_MANAGER* ActiveGDT = NULL;

int RegisterGDT(GDT_MANAGER* GDT, SYSTEM_ERROR* Error)
{
    #define ErrorOut_RegisterGDT(Code) \
        ErrorOut(Error, Code, FUNC_RegisterGDT);

    if(Probe4Error(GDT) || !GDT || Probe4Error(GDT->Operations) || !GDT->Operations)
    {
        ErrorOut_RegisterGDT(-BadParameters);
        return -BadParameters;
    }
    
    ActiveGDT = GDT;
    
    if(GDT->Operations->Init)
    {
        return GDT->Operations->Init(Error);
    }
    
    return GeneralOK;
}

GDT_MANAGER* GetGDT(SYSTEM_ERROR* Error)
{
    #define ErrorOut_GetGDT(Code) \
        ErrorOut(Error, Code, FUNC_GetGDT);

    if(Probe4Error(ActiveGDT) || !ActiveGDT)
    {
        ErrorOut_GetGDT(-NotFound);
        return Error2Pointer(-NotFound);
    }
    
    return ActiveGDT;
}

int SetGDTEntry(uint16_t Index, GDT_ENTRY* Entry, SYSTEM_ERROR* Error)
{
    #define ErrorOut_SetGDTEntry(Code) \
        ErrorOut(Error, Code, FUNC_SetGDTEntry);

    if(Probe4Error(ActiveGDT) || !ActiveGDT || !ActiveGDT->Operations->SetEntry)
    {
        ErrorOut_SetGDTEntry(-NotFound);
        return -NotFound;
    }

    return ActiveGDT->Operations->SetEntry(Index, Entry, Error);
}

int GetGDTEntry(uint16_t Index, GDT_ENTRY* Entry, SYSTEM_ERROR* Error)
{
    #define ErrorOut_GetGDTEntry(Code) \
        ErrorOut(Error, Code, FUNC_GetGDTEntry);

    if(Probe4Error(ActiveGDT) || !ActiveGDT || !ActiveGDT->Operations->GetEntry)
    {
        ErrorOut_GetGDTEntry(-NotFound);
        return -NotFound;
    }

    return ActiveGDT->Operations->GetEntry(Index, Entry, Error);
}

int LoadGDT(SYSTEM_ERROR* Error)
{
    #define ErrorOut_LoadGDT(Code) \
        ErrorOut(Error, Code, FUNC_LoadGDT);

    if(Probe4Error(ActiveGDT) || !ActiveGDT || !ActiveGDT->Operations->Load)
    {
        ErrorOut_LoadGDT(-NotFound);
        return -NotFound;
    }

    return ActiveGDT->Operations->Load(Error);
}

int InitGDT(GDT_MANAGER* GDT, SYSTEM_ERROR* Error)
{
    #define ErrorOut_InitGDT(Code) \
        ErrorOut(Error, Code, FUNC_InitGDT);

    if(Probe4Error(GDT) || !GDT || Probe4Error(GDT->Operations) || !GDT->Operations)
    {
        ErrorOut_InitGDT(-BadParameters);
        return -BadParameters;
    }

    return GDT->Operations->Init(Error);
}

int ExitGDT(GDT_MANAGER* GDT, SYSTEM_ERROR* Error)
{
    #define ErrorOut_ExitGDT(Code) \
        ErrorOut(Error, Code, FUNC_ExitGDT);

    if(Probe4Error(GDT) || !GDT || Probe4Error(GDT->Operations) || !GDT->Operations)
    {
        ErrorOut_ExitGDT(-BadParameters);
        return -BadParameters;
    }

    GDT->Operations->Exit(Error);
    return GeneralOK;
}
