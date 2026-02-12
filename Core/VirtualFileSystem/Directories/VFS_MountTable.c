#include <KernelCLibrary.h>
#include <VirtualFileSystem.h>
#include <__KCONF.h>
#include <DirtyHeap.h>

int
VfsMountTableEnumerate(char* Buffer, long Length, SYSTEM_ERROR* Error)
{
    #define ErrorOut_VfsMountTableEnumerate(Code) \
        ErrorOut(Error, Code, FUNC_VfsMountTableEnumerate)
 
    if (Probe4Error(Buffer) || !Buffer || Length <= 0)
    {
        ErrorOut_VfsMountTableEnumerate(-EINVAL);
        return Error->ErrorCode;
    }

    long Offset = 0;
    for (long Iteration = 0; Iteration < MountsCount; Iteration++)
    {
        const char* Path = Mounts[Iteration].Path;
        long Index = (long)strlen(Path);
        if (Offset + Index + 2 >= Length)
        {
            break;
        }

        memcpy(Buffer + Offset, Path, (size_t)Index);
        Offset += Index;
        Buffer[Offset++] = '\n';
    }

    if (Offset < Length)
    {
        Buffer[Offset] = 0;
    }
    
    return (int)Offset;
}

int
VfsMountTableFind(const char* Path, char* Buffer, long Length, SYSTEM_ERROR* Error)
{
    #define ErrorOut_VfsMountTableFind(Code) \
        ErrorOut(Error, Code, FUNC_VfsMountTableFind)
    
    if (Probe4Error(Path) || !Path || Probe4Error(Buffer) || !Buffer || Length <= 0)
    {
        ErrorOut_VfsMountTableFind(-EINVAL);
        return Error->ErrorCode;
    }

    for (long Iteration = 0; Iteration < MountsCount; Iteration++)
    {
        if (strcmp(Mounts[Iteration].Path, Path) == 0)
        {
            long Index = (long)strlen(Mounts[Iteration].Path);
            if (Index >= Length)
            {
                ErrorOut_VfsMountTableFind(-Limits);
                return Error->ErrorCode;
            }

            memcpy(Buffer, Mounts[Iteration].Path, (size_t)(Index + 1));
            
            return GeneralOK;
        }
    }

    ErrorOut_VfsMountTableFind(-ENOENT);
    return Error->ErrorCode;
}
