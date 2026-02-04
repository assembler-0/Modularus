#include <__KCONF.h>
#include <Plugs/Interrupts/IRQ.h>

IRQ_MANAGER* ActiveIRQ = NULL;

int RegisterIRQ(IRQ_MANAGER* IRQ, SYSTEM_ERROR* Error)
{
    #define ErrorOut_RegisterIRQ(Code) \
        ErrorOut(Error, Code, FUNC_RegisterIRQ);

    if(Probe4Error(IRQ) || !IRQ || Probe4Error(IRQ->Operations) || !IRQ->Operations)
    {
        ErrorOut_RegisterIRQ(-BadParameters);
        return -BadParameters;
    }
    
    ActiveIRQ = IRQ;
    
    if(IRQ->Operations->Init)
    {
        return IRQ->Operations->Init(Error);
    }
    
    return GeneralOK;
}

IRQ_MANAGER* GetIRQ(SYSTEM_ERROR* Error)
{
    #define ErrorOut_GetIRQ(Code) \
        ErrorOut(Error, Code, FUNC_GetIRQ);

    if(Probe4Error(ActiveIRQ) || !ActiveIRQ)
    {
        ErrorOut_GetIRQ(-NotFound);
        return Error2Pointer(-NotFound);
    }
    
    return ActiveIRQ;
}

int RegisterIRQHandler(uint8_t IRQ, IRQ_HANDLER Handler, SYSTEM_ERROR* Error)
{
    #define ErrorOut_RegisterIRQHandler(Code) \
        ErrorOut(Error, Code, FUNC_RegisterIRQHandler);

    if(Probe4Error(ActiveIRQ) || !ActiveIRQ || !ActiveIRQ->Operations->RegisterHandler)
    {
        ErrorOut_RegisterIRQHandler(-NotFound);
        return -NotFound;
    }

    return ActiveIRQ->Operations->RegisterHandler(IRQ, Handler, Error);
}

int UnregisterIRQHandler(uint8_t IRQ, SYSTEM_ERROR* Error)
{
    #define ErrorOut_UnregisterIRQHandler(Code) \
        ErrorOut(Error, Code, FUNC_UnregisterIRQHandler);

    if(Probe4Error(ActiveIRQ) || !ActiveIRQ || !ActiveIRQ->Operations->UnregisterHandler)
    {
        ErrorOut_UnregisterIRQHandler(-NotFound);
        return -NotFound;
    }

    return ActiveIRQ->Operations->UnregisterHandler(IRQ, Error);
}

int EnableIRQ(uint8_t IRQ, SYSTEM_ERROR* Error)
{
    #define ErrorOut_EnableIRQ(Code) \
        ErrorOut(Error, Code, FUNC_EnableIRQ);

    if(Probe4Error(ActiveIRQ) || !ActiveIRQ || !ActiveIRQ->Operations->Enable)
    {
        ErrorOut_EnableIRQ(-NotFound);
        return -NotFound;
    }

    return ActiveIRQ->Operations->Enable(IRQ, Error);
}

int DisableIRQ(uint8_t IRQ, SYSTEM_ERROR* Error)
{
    #define ErrorOut_DisableIRQ(Code) \
        ErrorOut(Error, Code, FUNC_DisableIRQ);

    if(Probe4Error(ActiveIRQ) || !ActiveIRQ || !ActiveIRQ->Operations->Disable)
    {
        ErrorOut_DisableIRQ(-NotFound);
        return -NotFound;
    }

    return ActiveIRQ->Operations->Disable(IRQ, Error);
}

int SendEOI(uint8_t IRQ, SYSTEM_ERROR* Error)
{
    #define ErrorOut_SendEOI(Code) \
        ErrorOut(Error, Code, FUNC_SendEOI);

    if(Probe4Error(ActiveIRQ) || !ActiveIRQ || !ActiveIRQ->Operations->SendEOI)
    {
        ErrorOut_SendEOI(-NotFound);
        return -NotFound;
    }

    return ActiveIRQ->Operations->SendEOI(IRQ, Error);
}

int InitIRQ(IRQ_MANAGER* IRQ, SYSTEM_ERROR* Error)
{
    #define ErrorOut_InitIRQ(Code) \
        ErrorOut(Error, Code, FUNC_InitIRQ);

    if(Probe4Error(IRQ) || !IRQ || Probe4Error(IRQ->Operations) || !IRQ->Operations)
    {
        ErrorOut_InitIRQ(-BadParameters);
        return -BadParameters;
    }

    return IRQ->Operations->Init(Error);
}

int ExitIRQ(IRQ_MANAGER* IRQ, SYSTEM_ERROR* Error)
{
    #define ErrorOut_ExitIRQ(Code) \
        ErrorOut(Error, Code, FUNC_ExitIRQ);

    if(Probe4Error(IRQ) || !IRQ || Probe4Error(IRQ->Operations) || !IRQ->Operations)
    {
        ErrorOut_ExitIRQ(-BadParameters);
        return -BadParameters;
    }

    IRQ->Operations->Exit(Error);
    return GeneralOK;
}
