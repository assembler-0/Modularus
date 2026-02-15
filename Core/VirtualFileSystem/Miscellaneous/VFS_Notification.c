#include <KernelCLibrary.h>
#include <VirtualFileSystem.h>
#include <__KCONF.h>
#include <DirtyHeap.h>

int
VFS_NotifySubscribe(const char* Path __unused, long __Mask__ __unused, SYSTEM_ERROR* __unused)
{
    return GeneralOK;
}

int
VFS_NotifyUnSubscribe(const char* Path __unused, SYSTEM_ERROR* Error __unused)
{
    return GeneralOK;
}

int
VFS_NotifyPoll(const char* Path __unused, long* OutMask, SYSTEM_ERROR* Error)
{
    #define ErrorOut_VFS_NotifyPoll(Code) \
        ErrorOut(Error, Code, FUNC_VFS_NotifyPoll)

    if (Probe4Error(OutMask) || !OutMask)
    {
        ErrorOut_VFS_NotifyPoll(-EINVAL);
        return Error->ErrorCode;
    }

    *OutMask = 0;
    
    return GeneralOK;
}