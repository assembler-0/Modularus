#include <KernelCLibrary.h>
#include <VirtualFileSystem.h>
#include <__KCONF.h>
#include <DirtyHeap.h>

int
VfsNotifySubscribe(const char* Path __UNUSED, long __Mask__ __UNUSED, SYSTEM_ERROR* __UNUSED)
{
    return GeneralOK;
}

int
VfsNotifyUnsubscribe(const char* Path __UNUSED, SYSTEM_ERROR* Error __UNUSED)
{
    return GeneralOK;
}

int
VfsNotifyPoll(const char* Path __UNUSED, long* OutMask, SYSTEM_ERROR* Error)
{
    #define ErrorOut_VfsNotifyPoll(Code) \
        ErrorOut(Error, Code, FUNC_VfsNotifyPoll)

    if (Probe4Error(OutMask) || !OutMask)
    {
        ErrorOut_VfsNotifyPoll(-EINVAL);
        return Error->ErrorCode;
    }

    *OutMask = 0;
    
    return GeneralOK;
}