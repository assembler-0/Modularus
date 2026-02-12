#include <KernelCLibrary.h>
#include <VirtualFileSystem.h>
#include <__KCONF.h>
#include <DirtyHeap.h>

int
DentryInvalidate(DIRECTORY_ENTRY* DirectoryEntry, SYSTEM_ERROR* Error)
{
    #define ErrorOut_DentryInvalidate(Code) \
        ErrorOut(Error, Code, FUNC_DentryInvalidate)
    
    if (Probe4Error(DirectoryEntry) || !DirectoryEntry)
    {
        ErrorOut_DentryInvalidate(-EINVAL);
        return Error->ErrorCode;
    }

    DirectoryEntry->Flags |= 1;
    
    return GeneralOK;
}

int
DentryRevalidate(DIRECTORY_ENTRY* DirectoryEntry, SYSTEM_ERROR* Error)
{
    #define ErrorOut_DentryRevalidate(Code) \
        ErrorOut(Error, Code, FUNC_DentryRevalidate)

    if (Probe4Error(DirectoryEntry) || !DirectoryEntry)
    {
        ErrorOut_DentryRevalidate(-EINVAL);
        return Error->ErrorCode;
    }

    DirectoryEntry->Flags &= ~1;
    
    return GeneralOK;
}

int
DentryAttach(DIRECTORY_ENTRY* DirectoryEntry, VFS_NODE* Node, SYSTEM_ERROR* Error)
{
    #define ErrorOut_DentryAttach(Code) \
        ErrorOut(Error, Code, FUNC_DentryAttach)

    if (Probe4Error(DirectoryEntry) || !DirectoryEntry || Probe4Error(Node) || !Node)
    {
        ErrorOut_DentryAttach(-EINVAL);
        return Error->ErrorCode;
    }

    DirectoryEntry->Node = Node;
    
    return GeneralOK;
}

int
DentryDetach(DIRECTORY_ENTRY* DirectoryEntry, SYSTEM_ERROR* Error)
{
    #define ErrorOut_DentryDetach(Code) \
        ErrorOut(Error, Code, FUNC_DentryDetach)

    if (Probe4Error(DirectoryEntry) || !DirectoryEntry)
    {
        ErrorOut_DentryDetach(-EINVAL);
        return Error->ErrorCode;
    }

    DirectoryEntry->Node = NULL;
    
    return GeneralOK;
}

int
DentryName(DIRECTORY_ENTRY* DirectoryEntry, char* Buffer, long Length, SYSTEM_ERROR* Error)
{
    #define ErrorOut_DentryName(Code) \
        ErrorOut(Error, Code, FUNC_DentryName)

    if (Probe4Error(DirectoryEntry) || !DirectoryEntry || Probe4Error(Buffer) || !Buffer || Length <= 0)
    {
        ErrorOut_DentryName(-EINVAL);
        return Error->ErrorCode;
    }

    long Index = (long)strlen(DirectoryEntry->Name);
    if (Index >= Length)
    {
        ErrorOut_DentryName(-Limits);
        return Error->ErrorCode;
    }

    memcpy(Buffer, DirectoryEntry->Name, (size_t)(Index + 1));
    
    return GeneralOK;
}
