#include <KernelCLibrary.h>
#include <VirtualFileSystem.h>
#include <__KCONF.h>
#include <DirtyHeap.h>

int
VFS_NodePath(VFS_NODE* Node __unused, char* Buffer, long Length, SYSTEM_ERROR* Error)
{
    #define ErrorOut_VFS_NodePath(Code) \
        ErrorOut(Error, Code, FUNC_VFS_NodePath)

    if (Probe4Error(Buffer) || !Buffer || Length <= 0)
    {
        ErrorOut_VFS_NodePath(-EINVAL);
        return Error->ErrorCode;
    }

    const char* Path = "/";
    long Index = (long)strlen(Path);
    if (Index >= Length)
    {
        ErrorOut_VFS_NodePath(-Limits);
        return Error->ErrorCode;
    }

    memcpy(Buffer, Path, (size_t)(Index + 1));
    
    return GeneralOK;
}

int
VFS_NodeName(VFS_NODE* Node __unused, char* Buffer, long Length, SYSTEM_ERROR* Error)
{
    #define ErrorOut_VFS_NodeName(Code) \
        ErrorOut(Error, Code, FUNC_VFS_NodeName)

    if (Probe4Error(Buffer) || !Buffer || Length <= 0)
    {
        ErrorOut_VFS_NodeName(-EINVAL);
        return Error->ErrorCode;
    }

    const char* Path = "";
    long Index = (long)strlen(Path);
    if (Index >= Length)
    {
        ErrorOut_VFS_NodeName(-Limits);
        return Error->ErrorCode;
    }

    memcpy(Buffer, Path, (size_t)(Index + 1));
    
    return GeneralOK;
}

int
VFS_AllocateName(char** Output, long Length, SYSTEM_ERROR* Error)
{
    #define ErrorOut_VFS_AllocateName(Code) \
        ErrorOut(Error, Code, FUNC_VFS_AllocateName)

    if (Probe4Error(Output) || !Output || Length <= 0)
    {
        ErrorOut_VFS_AllocateName(-EINVAL);
        return Error->ErrorCode;
    }

    *Output = (char*)KMalloc((size_t)Length, Error);
    if (Probe4Error(*Output) || !*Output)
    {
        ErrorOut_VFS_AllocateName(-ENOMEM);
        return Error->ErrorCode;
    }

    return GeneralOK;
}

int
VFS_FreeName(char* Name, SYSTEM_ERROR* Error)
{
    #define ErrorOut_VFS_FreeName(Code) \
        ErrorOut(Error, Code, FUNC_VFS_FreeName)

    if (Probe4Error(Name) || !Name)
    {
        ErrorOut_VFS_FreeName(-EINVAL);
        return Error->ErrorCode;
    }

    KFree(Name, Error);
    
    return GeneralOK;
}

int
VFS_JoinPath(const char* FirstPath, const char* SecondPath, char* Output, long Length, SYSTEM_ERROR* Error)
{
    #define ErrorOut_VFS_JoinPath(Code) \
        ErrorOut(Error, Code, FUNC_VFS_JoinPath)

    if (Probe4Error(FirstPath) || !FirstPath || Probe4Error(SecondPath) || !SecondPath || Probe4Error(Output) || !Output || Length <= 0)
    {
        ErrorOut_VFS_JoinPath(-EINVAL);
        return Error->ErrorCode;
    }

    long FirstPathLength = (long)strlen(FirstPath);
    long SecondPathLength = (long)strlen(SecondPath);
    long Required = FirstPathLength + 1 + SecondPathLength + 1;
    if (Required > Length)
    {
        ErrorOut_VFS_JoinPath(-Limits);
        return Error->ErrorCode;
    }

    memcpy(Output, FirstPath, (size_t)FirstPathLength);
    Output[FirstPathLength] = '/';
    memcpy(Output + FirstPathLength + 1, SecondPath, (size_t)SecondPathLength);
    Output[FirstPathLength + 1 + SecondPathLength] = 0;
    
    return GeneralOK;
}

int
VFS_SetMaxName(long Length, SYSTEM_ERROR* Error)
{
    #define ErrorOut_VFS_SetMaxName(Code) \
        ErrorOut(Error, Code, FUNC_VFS_SetMaxName)

    if (Length < 1)
    {
        ErrorOut_VFS_SetMaxName(-Limits);
        return Error->ErrorCode;
    }

    MaxName = Length;
    
    return GeneralOK;
}

long
VFS_GetMaxName(SYSTEM_ERROR* Error __unused)
{
    return MaxName;
}

int
VFS_SetMaxPath(long Length, SYSTEM_ERROR* Error)
{
    #define ErrorOut_VFS_SetMaxPath(Code) \
        ErrorOut(Error, Code, FUNC_VFS_SetMaxPath)

    if (Length < 1)
    {
        ErrorOut_VFS_SetMaxPath(-Limits);
        return Error->ErrorCode;
    }

    MaxPath = Length;
    
    return GeneralOK;
}

long
VFS_GetMaxPath(SYSTEM_ERROR* Error __unused)
{
    return MaxPath;
}