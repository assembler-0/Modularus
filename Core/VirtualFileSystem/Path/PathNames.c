#include <KernelCLibrary.h>
#include <VirtualFileSystem.h>
#include <__KCONF.h>
#include <DirtyHeap.h>

int
VfsNodePath(VFS_NODE* Node __UNUSED, char* Buffer, long Length, SYSTEM_ERROR* Error)
{
    #define ErrorOut_VfsNodePath(Code) \
        ErrorOut(Error, Code, FUNC_VfsNodePath)

    if (Probe4Error(Buffer) || !Buffer || Length <= 0)
    {
        ErrorOut_VfsNodePath(-EINVAL);
        return -EINVAL;
    }

    const char* Path = "/";
    long Index = (long)strlen(Path);
    if (Index >= Length)
    {
        ErrorOut_VfsNodePath(-Limits);
        return -Limits;
    }

    memcpy(Buffer, Path, (size_t)(Index + 1));
    
    return GeneralOK;
}

int
VfsNodeName(VFS_NODE* Node __UNUSED, char* Buffer, long Length, SYSTEM_ERROR* Error)
{
    #define ErrorOut_VfsNodeName(Code) \
        ErrorOut(Error, Code, FUNC_VfsNodeName)

    if (Probe4Error(Buffer) || !Buffer || Length <= 0)
    {
        ErrorOut_VfsNodeName(-EINVAL);
        return -EINVAL;
    }

    const char* Path = "";
    long Index = (long)strlen(Path);
    if (Index >= Length)
    {
        ErrorOut_VfsNodeName(-Limits);
        return -Limits;
    }

    memcpy(Buffer, Path, (size_t)(Index + 1));
    
    return GeneralOK;
}

int
VfsAllocName(char** Output, long Length, SYSTEM_ERROR* Error)
{
    #define ErrorOut_VfsAllocName(Code) \
        ErrorOut(Error, Code, FUNC_VfsAllocName)

    if (Probe4Error(Output) || !Output || Length <= 0)
    {
        ErrorOut_VfsAllocName(-EINVAL);
        return -EINVAL;
    }

    *Output = (char*)KMalloc((size_t)Length, Error);
    if (Probe4Error(*Output) || !*Output)
    {
        ErrorOut_VfsAllocName(-ENOMEM);
        return -ENOMEM;
    }

    return GeneralOK;
}

int
VfsFreeName(char* Name, SYSTEM_ERROR* Error)
{
    #define ErrorOut_VfsFreeName(Code) \
        ErrorOut(Error, Code, FUNC_VfsFreeName)

    if (Probe4Error(Name) || !Name)
    {
        ErrorOut_VfsFreeName(-EINVAL);
        return -EINVAL;
    }

    KFree(Name, Error);
    
    return GeneralOK;
}

int
VfsJoinPath(const char* FirstPath, const char* SecondPath, char* Output, long Length, SYSTEM_ERROR* Error)
{
    #define ErrorOut_VfsJoinPath(Code) \
        ErrorOut(Error, Code, FUNC_VfsJoinPath)

    if (Probe4Error(FirstPath) || !FirstPath || Probe4Error(SecondPath) || !SecondPath || Probe4Error(Output) || !Output || Length <= 0)
    {
        ErrorOut_VfsJoinPath(-EINVAL);
        return -EINVAL;
    }

    long FirstPathLength = (long)strlen(FirstPath);
    long SecondPathLength = (long)strlen(SecondPath);
    long Required = FirstPathLength + 1 + SecondPathLength + 1;
    if (Required > Length)
    {
        ErrorOut_VfsJoinPath(-Limits);
        return -Limits;
    }

    memcpy(Output, FirstPath, (size_t)FirstPathLength);
    Output[FirstPathLength] = '/';
    memcpy(Output + FirstPathLength + 1, SecondPath, (size_t)SecondPathLength);
    Output[FirstPathLength + 1 + SecondPathLength] = 0;
    
    return GeneralOK;
}

int
VfsSetMaxName(long Length, SYSTEM_ERROR* Error)
{
    #define ErrorOut_VfsSetMaxName(Code) \
        ErrorOut(Error, Code, FUNC_VfsSetMaxName)

    if (Length < 1)
    {
        ErrorOut_VfsSetMaxName(-Limits);
        return -Limits;
    }

    MaxName = Length;
    
    return GeneralOK;
}

long
VfsGetMaxName(SYSTEM_ERROR* Error __UNUSED)
{
    return MaxName;
}

int
VfsSetMaxPath(long Length, SYSTEM_ERROR* Error)
{
    #define ErrorOut_VfsSetMaxPath(Code) \
        ErrorOut(Error, Code, FUNC_VfsSetMaxPath)

    if (Length < 1)
    {
        ErrorOut_VfsSetMaxPath(-Limits);
        return -Limits;
    }

    MaxPath = Length;
    
    return GeneralOK;
}

long
VfsGetMaxPath(SYSTEM_ERROR* Error __UNUSED)
{
    return MaxPath;
}