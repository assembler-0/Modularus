#include <__KCONF.h>
#include <Plugs/Interrupts/ISR.h>

ISR_MANAGER* ActiveISR = NULL;

int RegisterISR(ISR_MANAGER* ISR, SYSTEM_ERROR* Error)
{
    #define ErrorOut_RegisterISR(Code) \
        ErrorOut(Error, Code, FUNC_RegisterISR);

    if(Probe4Error(ISR) || !ISR || Probe4Error(ISR->Operations) || !ISR->Operations)
    {
        ErrorOut_RegisterISR(-BadParameters);
        return -BadParameters;
    }
    
    ActiveISR = ISR;
    
    if(ISR->Operations->Init)
    {
        return ISR->Operations->Init(Error);
    }
    
    return GeneralOK;
}

ISR_MANAGER* GetISR(SYSTEM_ERROR* Error)
{
    #define ErrorOut_GetISR(Code) \
        ErrorOut(Error, Code, FUNC_GetISR);

    if(Probe4Error(ActiveISR) || !ActiveISR)
    {
        ErrorOut_GetISR(-NotFound);
        return Error2Pointer(-NotFound);
    }
    
    return ActiveISR;
}

int RegisterISRHandler(uint8_t Vector, ISR_HANDLER Handler, SYSTEM_ERROR* Error)
{
    #define ErrorOut_RegisterISRHandler(Code) \
        ErrorOut(Error, Code, FUNC_RegisterISRHandler);

    if(Probe4Error(ActiveISR) || !ActiveISR || !ActiveISR->Operations->RegisterHandler)
    {
        ErrorOut_RegisterISRHandler(-NotFound);
        return -NotFound;
    }

    return ActiveISR->Operations->RegisterHandler(Vector, Handler, Error);
}

int UnregisterISRHandler(uint8_t Vector, SYSTEM_ERROR* Error)
{
    #define ErrorOut_UnregisterISRHandler(Code) \
        ErrorOut(Error, Code, FUNC_UnregisterISRHandler);

    if(Probe4Error(ActiveISR) || !ActiveISR || !ActiveISR->Operations->UnregisterHandler)
    {
        ErrorOut_UnregisterISRHandler(-NotFound);
        return -NotFound;
    }

    return ActiveISR->Operations->UnregisterHandler(Vector, Error);
}

int InitISR(ISR_MANAGER* ISR, SYSTEM_ERROR* Error)
{
    #define ErrorOut_InitISR(Code) \
        ErrorOut(Error, Code, FUNC_InitISR);

    if(Probe4Error(ISR) || !ISR || Probe4Error(ISR->Operations) || !ISR->Operations)
    {
        ErrorOut_InitISR(-BadParameters);
        return -BadParameters;
    }

    return ISR->Operations->Init(Error);
}

int ExitISR(ISR_MANAGER* ISR, SYSTEM_ERROR* Error)
{
    #define ErrorOut_ExitISR(Code) \
        ErrorOut(Error, Code, FUNC_ExitISR);

    if(Probe4Error(ISR) || !ISR || Probe4Error(ISR->Operations) || !ISR->Operations)
    {
        ErrorOut_ExitISR(-BadParameters);
        return -BadParameters;
    }

    ISR->Operations->Exit(Error);
    return GeneralOK;
}
