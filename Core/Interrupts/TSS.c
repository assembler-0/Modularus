#include <__KCONF.h>
#include <Plugs/Interrupts/TSS.h>

TSS_MANAGER* ActiveTSS = NULL;

int RegisterTSS(TSS_MANAGER* TSS, SYSTEM_ERROR* Error)
{
    #define ErrorOut_RegisterTSS(Code) \
        ErrorOut(Error, Code, FUNC_RegisterTSS);

    if(Probe4Error(TSS) || !TSS || Probe4Error(TSS->Operations) || !TSS->Operations)
    {
        ErrorOut_RegisterTSS(-BadParameters);
        return -BadParameters;
    }
    
    ActiveTSS = TSS;
    
    if(TSS->Operations->Init)
    {
        return TSS->Operations->Init(Error);
    }
    
    return GeneralOK;
}

TSS_MANAGER* GetTSS(SYSTEM_ERROR* Error)
{
    #define ErrorOut_GetTSS(Code) \
        ErrorOut(Error, Code, FUNC_GetTSS);

    if(Probe4Error(ActiveTSS) || !ActiveTSS)
    {
        ErrorOut_GetTSS(-NotFound);
        return Error2Pointer(-NotFound);
    }
    
    return ActiveTSS;
}

int SetTSSStack(uint8_t IST, uint64_t Stack, SYSTEM_ERROR* Error)
{
    #define ErrorOut_SetTSSStack(Code) \
        ErrorOut(Error, Code, FUNC_SetTSSStack);

    if(Probe4Error(ActiveTSS) || !ActiveTSS || !ActiveTSS->Operations->SetStack)
    {
        ErrorOut_SetTSSStack(-NotFound);
        return -NotFound;
    }

    return ActiveTSS->Operations->SetStack(IST, Stack, Error);
}

uint64_t GetTSSStack(uint8_t IST, SYSTEM_ERROR* Error)
{
    #define ErrorOut_GetTSSStack(Code) \
        ErrorOut(Error, Code, FUNC_GetTSSStack);

    if(Probe4Error(ActiveTSS) || !ActiveTSS || !ActiveTSS->Operations->GetStack)
    {
        ErrorOut_GetTSSStack(-NotFound);
        return (uint64_t)Error2Pointer(-NotFound);
    }

    return ActiveTSS->Operations->GetStack(IST, Error);
}

int LoadTSS(SYSTEM_ERROR* Error)
{
    #define ErrorOut_LoadTSS(Code) \
        ErrorOut(Error, Code, FUNC_LoadTSS);

    if(Probe4Error(ActiveTSS) || !ActiveTSS || !ActiveTSS->Operations->Load)
    {
        ErrorOut_LoadTSS(-NotFound);
        return -NotFound;
    }

    return ActiveTSS->Operations->Load(Error);
}

int InitTSS(TSS_MANAGER* TSS, SYSTEM_ERROR* Error)
{
    #define ErrorOut_InitTSS(Code) \
        ErrorOut(Error, Code, FUNC_InitTSS);

    if(Probe4Error(TSS) || !TSS || Probe4Error(TSS->Operations) || !TSS->Operations)
    {
        ErrorOut_InitTSS(-BadParameters);
        return -BadParameters;
    }

    return TSS->Operations->Init(Error);
}

int ExitTSS(TSS_MANAGER* TSS, SYSTEM_ERROR* Error)
{
    #define ErrorOut_ExitTSS(Code) \
        ErrorOut(Error, Code, FUNC_ExitTSS);

    if(Probe4Error(TSS) || !TSS || Probe4Error(TSS->Operations) || !TSS->Operations)
    {
        ErrorOut_ExitTSS(-BadParameters);
        return -BadParameters;
    }

    TSS->Operations->Exit(Error);
    return GeneralOK;
}
