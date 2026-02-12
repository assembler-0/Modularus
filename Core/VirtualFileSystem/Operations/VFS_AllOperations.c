#include <KernelCLibrary.h>
#include <VirtualFileSystem.h>
#include <__KCONF.h>
#include <DirtyHeap.h>

int
VfsReadAll(const char* Path, void* Buffer, long BufferLength, long* OutLength, SYSTEM_ERROR* Error)
{
    #define ErrorOut_VfsReadAll(Code) \
        ErrorOut(Error, Code, FUNC_VfsReadAll)
    
    FILE* FileHandle = VfsOpen(Path, VfsOpenFlag_READONLY, Error);
    if (Probe4Error(FileHandle) || !FileHandle)
    {
        ErrorOut_VfsReadAll(-ENOENT);
        return Error->ErrorCode;
    }
    
    long Total = 0;
    while (Total < BufferLength)
    {
        long ReadIndex = VfsRead(FileHandle, (char*)Buffer + Total, BufferLength - Total, Error);
        if (ReadIndex < 0)
        {
            VfsClose(FileHandle, Error);
            
            ErrorOut_VfsReadAll(-Limits);
            return Error->ErrorCode;
        }

        if (ReadIndex == 0)
        {
            break;
        }

        Total += ReadIndex;
    }

    if (OutLength)
    {
        *OutLength = Total;
    }

    VfsClose(FileHandle, Error);
    
    return GeneralOK;
}

int
VfsWriteAll(const char* Path, const void* Buffer, long Length, SYSTEM_ERROR* Error)
{
    #define ErrorOut_VfsWriteAll(Code) \
        ErrorOut(Error, Code, FUNC_VfsWriteAll)

    FILE* FileHandle = VfsOpen(Path, VfsOpenFlag_CREATE | VfsOpenFlag_WRITEONLY | VfsOpenFlag_TRUNCATE, Error);
    if (Probe4Error(FileHandle) || !FileHandle)
    {
        ErrorOut_VfsWriteAll(-ENOENT);
        return Error->ErrorCode;
    }

    long Total = 0;
    while (Total < Length)
    {
        long WriteIndex = VfsWrite(FileHandle, (const char*)Buffer + Total, Length - Total, Error);
        if (WriteIndex <= 0)
        {
            VfsClose(FileHandle, Error);
            
            ErrorOut_VfsWriteAll(-Limits);
            return Error->ErrorCode;
        }

        Total += WriteIndex;
    }

    VfsClose(FileHandle, Error);
    
    return GeneralOK;
}

int
VfsSyncAll(SYSTEM_ERROR* Error)
{
    for (long Iteration = 0; Iteration < MountsCount; Iteration++)
    {
        SUPER_BLOCK* SuperBlock = Mounts[Iteration].SuperBlock;
        if (SuperBlock && SuperBlock->Operations && SuperBlock->Operations->Sync)
        {
            SuperBlock->Operations->Sync(SuperBlock, Error);
        }
    }
    
    return GeneralOK;
}