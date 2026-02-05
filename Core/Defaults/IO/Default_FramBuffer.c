#include <__KCONF.h>
#include <Plugs/IO/FrameBuffer.h>
#include <Boot/PreBoot.h>

static FRAMEBUFFER DefaultFrameBuffer;
static struct limine_framebuffer* DefaultFrameBufferRequest;

static int Default_FrameBufferInit(SYSTEM_ERROR* Error)
{
    BOOT_REQUEST Request = RequestBootService(RequestFRAMEBUFFER, Error);
    if(Probe4Error(Request.Pointer))
    {
        return Pointer2Error(Request.Pointer);
    }
    
    #ifdef BOOTVENDOR_Limine
        struct limine_framebuffer_response* Response = ((struct limine_framebuffer_request*)Request.Pointer)->response;
    
        if(!Response || Response->framebuffer_count == 0)
        {
            ErrorOut(Error, -NotFound, General);
            return -NotFound;
        }

        DefaultFrameBufferRequest = Response->framebuffers[0];

        DefaultFrameBuffer.Information.Address = DefaultFrameBufferRequest->address;
        DefaultFrameBuffer.Information.Width   = DefaultFrameBufferRequest->width;
        DefaultFrameBuffer.Information.Height  = DefaultFrameBufferRequest->height;
        DefaultFrameBuffer.Information.Pitch   = DefaultFrameBufferRequest->pitch;
        DefaultFrameBuffer.Information.BPP     = DefaultFrameBufferRequest->bpp;
        DefaultFrameBuffer.Information.Format  = PixelFormat_RGB;
    #endif
    
    return GeneralOK;
}

static void Default_FrameBufferExit(SYSTEM_ERROR* Error)
{
    (void)Error;
    DefaultFrameBufferRequest = NULL;
}

static FRAMEBUFFER_OPERATIONS DefaultOps=
{
    .Init = Default_FrameBufferInit,
    .Exit = Default_FrameBufferExit
};

int InitDefaultFrameBuffer(SYSTEM_ERROR* Error)
{
    DefaultFrameBuffer.Operations = &DefaultOps;
    DefaultFrameBuffer.Private = NULL;
    return RegisterFrameBuffer(&DefaultFrameBuffer, Error);
}
