#include <KernelCLibrary.h>
#include <VirtualFileSystem.h>
#include <__KCONF.h>
#include <DirtyHeap.h>

int
VfsFsync(FILE* FileHandle, SYSTEM_ERROR* Error)
{
    #define ErrorOut_VfsFsync(Code) \
        ErrorOut(Error, Code, FUNC_VfsFsync)
    
    if (Probe4Error(FileHandle) || !FileHandle || Probe4Error(FileHandle->Node) || !FileHandle->Node || Probe4Error(FileHandle->Node->Operations) || !FileHandle->Node->Operations)
    {
        ErrorOut_VfsFsync(-EINVAL);
        return Error->ErrorCode;
    }

    if (Probe4Error(FileHandle->Node->Operations->Sync) || !FileHandle->Node->Operations->Sync)
    {
        return GeneralOK;
    }

    return FileHandle->Node->Operations->Sync(FileHandle->Node, Error);
}