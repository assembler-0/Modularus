#include <KernelCLibrary.h>
#include <VirtualFileSystem.h>
#include <__KCONF.h>
#include <DirtyHeap.h>

int
VfsRegisterFs(const FILESYSTEM_TYPE* FileSystemType, SYSTEM_ERROR* Error)
{
    #define ErrorOut_VfsRegisterFs(Code) \
        ErrorOut(Error, Code, FUNC_VfsRegisterFs)

    if (Probe4Error(FileSystemType) || !FileSystemType || Probe4Error(FileSystemType->Name) ||
        !FileSystemType->Name || Probe4Error(FileSystemType->Mount) || !FileSystemType->Mount)
    {
        ErrorOut_VfsRegisterFs(-EINVAL);
        return Error->ErrorCode;
    }

    if (FileSystemCount >= MaxFileSystemTypes)
    {
        ErrorOut_VfsRegisterFs(-Limits);
        return Error->ErrorCode;
    }

    for (long Iteration = 0; Iteration < FileSystemCount; Iteration++)
    {
        if (strcmp(FileSystemRegistry[Iteration]->Name, FileSystemType->Name) == 0)
        {
            return -EEXIST;
        }
    }

    FileSystemRegistry[FileSystemCount++] = FileSystemType;

    return GeneralOK;
}

int
VfsUnregisterFs(const char* Name, SYSTEM_ERROR* Error)
{
    #define ErrorOut_VfsUnregisterFs(Code) \
        ErrorOut(Error, Code, FUNC_VfsUnregisterFs)

    if (Probe4Error(Name) || !Name)
    {
        ErrorOut_VfsUnregisterFs(-EINVAL);
        return Error->ErrorCode;
    }

    for (long Iteration = 0; Iteration < FileSystemCount; Iteration++)
    {
        if (strcmp(FileSystemRegistry[Iteration]->Name, Name) == 0)
        {
            for (long Index = Iteration; Index < FileSystemCount - 1; Index++)
            {
                FileSystemRegistry[Index] = FileSystemRegistry[Index + 1];
            }
            FileSystemRegistry[--FileSystemCount] = 0;
            return GeneralOK;
        }
    }

    ErrorOut_VfsUnregisterFs(-ENOENT);
    return Error->ErrorCode;
}

const FILESYSTEM_TYPE*
VfsFindFs(const char* Name, SYSTEM_ERROR* Error)
{
    #define ErrorOut_VfsFindFs(Code) \
        ErrorOut(Error, Code, FUNC_VfsFindFs)

    if (Probe4Error(Name) || !Name)
    {
        ErrorOut_VfsFindFs(-EINVAL);
        return Error2Pointer(-EINVAL);
    }

    for (long Iteration = 0; Iteration < FileSystemCount; Iteration++)
    {
        if (strcmp(FileSystemRegistry[Iteration]->Name, Name) == 0)
        {
            return FileSystemRegistry[Iteration];
        }
    }

    ErrorOut_VfsFindFs(-ENOENT);
    return Error2Pointer(-ENOENT);
}

long
VfsListFs(const char** Output, long Capacity, SYSTEM_ERROR* Error)
{
    #define ErrorOut_VfsListFs(Code) \
        ErrorOut(Error, Code, FUNC_VfsListFs)

    if (Probe4Error(Output) || !Output || Capacity <= 0)
    {
        ErrorOut_VfsListFs(-EINVAL);
        return Error->ErrorCode;
    }

    long Index = (FileSystemCount < Capacity) ? FileSystemCount : Capacity;
    for (long Iteration = 0; Iteration < Index; Iteration++)
    {
        Output[Iteration] = FileSystemRegistry[Iteration]->Name;
    }

    return Index;
}
