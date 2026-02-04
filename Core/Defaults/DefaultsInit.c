#include <__KCONF.h>
#include <Defaults/IO/Default_FrameBuffer.h>

int LoadDefaults(SYSTEM_ERROR* Error)
{
    InitDefaultFrameBuffer(Error);
    return GeneralOK;
}