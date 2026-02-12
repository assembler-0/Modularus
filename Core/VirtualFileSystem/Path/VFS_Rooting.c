#include <KernelCLibrary.h>
#include <VirtualFileSystem.h>
#include <__KCONF.h>
#include <DirtyHeap.h>

int
VfsSetCwd(const char* Path __UNUSED, SYSTEM_ERROR* Error __UNUSED)
{
    /*Defaulted to ROOT (/)*/
    /*TODO: Make it actually do something at least :sob:*/
    return GeneralOK;
}

int
VfsGetCwd(char* Buffer, long Length, SYSTEM_ERROR* Error)
{
    #define ErrorOut_VfsGetCwd(Code) \
        ErrorOut(Error, Code, FUNC_VfsGetCwd)

    if (Probe4Error(Buffer) || !Buffer || Length <= 0)
    {
        ErrorOut_VfsGetCwd(-EINVAL);
        return Error->ErrorCode;
    }

    const char* Path = "/";
    long Index = (long)strlen(Path);
    if (Index >= Length)
    {
        ErrorOut_VfsGetCwd(-Limits);
        return Error->ErrorCode;
    }

    memcpy(Buffer, Path, (size_t)(Index + 1));
    
    return GeneralOK;
}

int
VfsSetRoot(const char* Path, SYSTEM_ERROR* Error)
{
    return VfsSwitchRoot(Path, Error);
}

int
VfsGetRoot(char* Buffer, long Length, SYSTEM_ERROR* Error)
{
    #define ErrorOut_VfsGetRoot(Code) \
        ErrorOut(Error, Code, FUNC_VfsGetRoot)

    if (Probe4Error(Buffer) || !Buffer || Length <= 0)
    {
        ErrorOut_VfsGetRoot(-EINVAL);
        return Error->ErrorCode;
    }

    const char* Path = "/";
    long Index = (long)strlen(Path);
    if (Index >= Length)
    {
        ErrorOut_VfsGetRoot(-Limits);
        return Error->ErrorCode;
    }

    memcpy(Buffer, Path, (size_t)(Index + 1));
    
    return GeneralOK;
}
