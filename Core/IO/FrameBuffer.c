#include <__KCONF.h>
#include <Plugs/IO/FrameBuffer.h>

FRAMEBUFFER* ActiveFrameBuffer = NULL;

/*The main thing*/
int RegisterFrameBuffer(FRAMEBUFFER* FrameBuffer, SYSTEM_ERROR* Error)
{
    #define ErrorOut_RegisterFrameBuffer(Code) \
        ErrorOut(Error, Code, FUNC_RegisterFrameBuffer);

    if(Probe4Error(FrameBuffer)             || 
       !FrameBuffer                         || 
       Probe4Error(FrameBuffer->Operations) ||
       !FrameBuffer->Operations)
    {
        ErrorOut_RegisterFrameBuffer(-BadParameters);
        return -BadParameters;
    }
    
    ActiveFrameBuffer = FrameBuffer;
    
    if(FrameBuffer->Operations->Init)
    {
        return FrameBuffer->Operations->Init(&FrameBuffer->Info, Error);
    }
    
    return GeneralOK;
}

FRAMEBUFFER* GetFrameBuffer(SYSTEM_ERROR* Error)
{
    #define ErrorOut_GetFrameBuffer(Code) \
        ErrorOut(Error, Code, FUNC_GetFrameBuffer);

    if(Probe4Error(ActiveFrameBuffer) || !ActiveFrameBuffer)
    {
        ErrorOut_GetFrameBuffer(-NotFound);
        return Error2Pointer(-NotFound);
    }
    
    return ActiveFrameBuffer;
}

/*Wrap-ups for operations*/

int InitFrameBuffer(FRAMEBUFFER* FrameBuffer, SYSTEM_ERROR* Error)
{
    #define ErrorOut_InitFrameBuffer(Code) \
        ErrorOut(Error, Code, FUNC_InitFrameBuffer);

    if (Probe4Error(FrameBuffer)             || 
        !FrameBuffer                         ||
        Probe4Error(FrameBuffer->Operations) ||
        !FrameBuffer->Operations)
    {
        ErrorOut_InitFrameBuffer(-BadParameters);
        return -BadParameters;
    }

    return FrameBuffer->Operations->Init(&FrameBuffer->Info, Error);
}

int ExitFrameBuffer(FRAMEBUFFER* FrameBuffer, SYSTEM_ERROR* Error)
{
    #define ErrorOut_ExitFrameBuffer(Code) \
        ErrorOut(Error, Code, FUNC_ExitFrameBuffer);

    if (Probe4Error(FrameBuffer)             || 
        !FrameBuffer                         ||
        Probe4Error(FrameBuffer->Operations) ||
        !FrameBuffer->Operations)
    {
        ErrorOut_ExitFrameBuffer(-BadParameters);
        return -BadParameters;
    }

    FrameBuffer->Operations->Exit(Error);

    return GeneralOK;
}