#include <KernelCLibrary.h>
#include <VirtualFileSystem.h>
#include <__KCONF.h>
#include <DirtyHeap.h>

int
VFS_SetWorkingDirectory(const char* Path __unused, SYSTEM_ERROR* Error __unused)
{
    /*Defaulted to ROOT (/)*/
    /*TODO: Make it actually do something at least :sob:*/
    return GeneralOK;
}

int
VFS_GetWorkingDirectory(char* Buffer, long Length, SYSTEM_ERROR* Error)
{
    #define ErrorOut_VFS_GetWorkingDirectory(Code) \
        ErrorOut(Error, Code, FUNC_VFS_GetWorkingDirectory)

    if (Probe4Error(Buffer) || !Buffer || Length <= 0)
    {
        ErrorOut_VFS_GetWorkingDirectory(-EINVAL);
        return Error->ErrorCode;
    }

    const char* Path = "/";
    long Index = (long)strlen(Path);
    if (Index >= Length)
    {
        ErrorOut_VFS_GetWorkingDirectory(-Limits);
        return Error->ErrorCode;
    }

    memcpy(Buffer, Path, (size_t)(Index + 1));
    
    return GeneralOK;
}

int
VFS_SetRoot(const char* Path, SYSTEM_ERROR* Error)
{
    return VFS_ChangeRoot(Path, Error);
}

int
VFS_GetRoot(char* Buffer, long Length, SYSTEM_ERROR* Error)
{
    #define ErrorOut_VFS_GetRoot(Code) \
        ErrorOut(Error, Code, FUNC_VFS_GetRoot)

    if (Probe4Error(Buffer) || !Buffer || Length <= 0)
    {
        ErrorOut_VFS_GetRoot(-EINVAL);
        return Error->ErrorCode;
    }

    const char* Path = "/";
    long Index = (long)strlen(Path);
    if (Index >= Length)
    {
        ErrorOut_VFS_GetRoot(-Limits);
        return Error->ErrorCode;
    }

    memcpy(Buffer, Path, (size_t)(Index + 1));
    
    return GeneralOK;
}
