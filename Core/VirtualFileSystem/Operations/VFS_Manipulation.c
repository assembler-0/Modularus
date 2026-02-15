#include <KernelCLibrary.h>
#include <VirtualFileSystem.h>
#include <__KCONF.h>
#include <DirtyHeap.h>

int
VFS_ReName(const char* OldPath, const char* NewPath, long Flags, SYSTEM_ERROR* Error)
{
    #define ErrorOut_VFS_ReName(Code) \
        ErrorOut(Error, Code, FUNC_VFS_ReName)

    DIRECTORY_ENTRY* OldBase = 0;
    DIRECTORY_ENTRY* NewBase = 0;
    char OldName[256];
    char NewName[256];

    if (Probe4Error(OldPath) || !OldPath || Probe4Error(NewPath) || !NewPath)
    {
        ErrorOut_VFS_ReName(-EINVAL);
        return Error->ErrorCode;
    }

    /*Old path*/

    const char* PathComponentOld = OldPath;
    if (IsSeperator(*PathComponentOld, Error))
    {
        PathComponentOld = SkipSeperator(PathComponentOld, Error);
    }

    VFS_NODE* CurrentOldNode = RootNode;
    DIRECTORY_ENTRY* OldDirectoryEntry  = RootDirectoryEntry;
    while (*PathComponentOld)
    {
        long Index = NextCompare(PathComponentOld, OldName, sizeof(OldName), Error);
        if (Index <= 0)
        {
            break;
        }

        while (*PathComponentOld && !IsSeperator(*PathComponentOld, Error))
        {
            PathComponentOld++;
        }

        PathComponentOld = SkipSeperator(PathComponentOld, Error);
        if (*PathComponentOld == 0)
        {
            OldBase = OldDirectoryEntry;
            break;
        }

        if (Probe4Error(CurrentOldNode) || !CurrentOldNode || Probe4Error(CurrentOldNode->Operations) || !CurrentOldNode->Operations || Probe4Error(CurrentOldNode->Operations->Lookup) || !CurrentOldNode->Operations->Lookup)
        {
            ErrorOut_VFS_ReName(-ENOSYS);
            return Error->ErrorCode;
        }

        VFS_NODE* Next = CurrentOldNode->Operations->Lookup(CurrentOldNode, OldName, Error);
        if (Probe4Error(Next) || !Next)
        {
            ErrorOut_VFS_ReName(-ENOENT);
            return Error->ErrorCode;
        }

        char* Dublicate = (char*)KMalloc((size_t)(Index + 1), Error);
        if (Probe4Error(Dublicate) || !Dublicate)
        {
            ErrorOut_VFS_ReName(-ENOMEM);
            return Error->ErrorCode;
        }

        memcpy(Dublicate, OldName, (size_t)(Index + 1));
        OldDirectoryEntry = AllocateDirectoryEntry(Dublicate, OldDirectoryEntry, Next, Error);
        if (Probe4Error(OldDirectoryEntry) || !OldDirectoryEntry)
        {
            ErrorOut_VFS_ReName(Error->ErrorCode);
            return Error->ErrorCode;
        }

        CurrentOldNode = Next;
    }

    /*New path*/

    const char* PathComponentNew = NewPath;
    if (IsSeperator(*PathComponentNew, Error))
    {
        PathComponentNew = SkipSeperator(PathComponentNew, Error);
    }

    VFS_NODE*  CurrentNewNode = RootNode;
    DIRECTORY_ENTRY* NewDirectoryEntry  = RootDirectoryEntry;
    while (*PathComponentNew)
    {
        long Index = NextCompare(PathComponentNew, NewName, sizeof(NewName), Error);
        if (Index <= 0)
        {
            break;
        }

        while (*PathComponentNew && !IsSeperator(*PathComponentNew, Error))
        {
            PathComponentNew++;
        }

        PathComponentNew = SkipSeperator(PathComponentNew, Error);
        if (*PathComponentNew == 0)
        {
            NewBase = NewDirectoryEntry;
            break;
        }

        if (Probe4Error(CurrentNewNode) || !CurrentNewNode || Probe4Error(CurrentNewNode->Operations) || !CurrentNewNode->Operations || Probe4Error(CurrentNewNode->Operations->Lookup) || !CurrentNewNode->Operations->Lookup)
        {
            ErrorOut_VFS_ReName(-ENOSYS);
            return Error->ErrorCode;
        }

        VFS_NODE* Next = CurrentNewNode->Operations->Lookup(CurrentNewNode, NewName, Error);
        if (Probe4Error(Next) || !Next)
        {
            ErrorOut_VFS_ReName(-ENOENT);
            return Error->ErrorCode;
        }

        char* Dublicate = (char*)KMalloc((size_t)(Index + 1), Error);
        if (Probe4Error(Dublicate) || !Dublicate)
        {
            ErrorOut_VFS_ReName(-ENOMEM);
            return Error->ErrorCode;
        }

        memcpy(Dublicate, NewName, (size_t)(Index + 1));
        NewDirectoryEntry = AllocateDirectoryEntry(Dublicate, NewDirectoryEntry, Next, Error);
        if (Probe4Error(NewDirectoryEntry) || !NewDirectoryEntry)
        {
            ErrorOut_VFS_ReName(Error->ErrorCode);
            return Error->ErrorCode;
        }

        CurrentNewNode = Next;
    }

    if (Probe4Error(OldBase) || !OldBase || Probe4Error(NewBase) || !NewBase)
    {
        ErrorOut_VFS_ReName(-ENOENT);
        return Error->ErrorCode;
    }

    if (Probe4Error(OldBase->Node) || !OldBase->Node || Probe4Error(NewBase->Node) || !NewBase->Node)
    {
        ErrorOut_VFS_ReName(-ENOENT);
        return Error->ErrorCode;
    }

    if (Probe4Error(OldBase->Node->Operations) || !OldBase->Node->Operations || Probe4Error(OldBase->Node->Operations->Rename) || !OldBase->Node->Operations->Rename)
    {
        ErrorOut_VFS_ReName(-ENOSYS);
        return Error->ErrorCode;
    }
    
    return OldBase->Node->Operations->Rename(OldBase->Node, OldName, NewBase->Node, NewName, Flags, Error);
}

int
VFS_Truncate(const char* Path, long Length, SYSTEM_ERROR* Error)
{
    #define ErrorOut_VFS_Truncate(Code) \
        ErrorOut(Error, Code, FUNC_VFS_Truncate)

    DIRECTORY_ENTRY* DirectoryEntry = VFS_Resolve(Path, Error);
    if (Probe4Error(DirectoryEntry) || !DirectoryEntry || Probe4Error(DirectoryEntry->Node) || !DirectoryEntry->Node)
    {
        ErrorOut_VFS_Truncate(-ENOENT);
        return Error->ErrorCode;
    }

    if (Probe4Error(DirectoryEntry->Node->Operations) || !DirectoryEntry->Node->Operations || Probe4Error(DirectoryEntry->Node->Operations->Truncate) || !DirectoryEntry->Node->Operations->Truncate)
    {
        ErrorOut_VFS_Truncate(-ENOSYS);
        return Error->ErrorCode;
    }
    
    return DirectoryEntry->Node->Operations->Truncate(DirectoryEntry->Node, Length, Error);
}

int
VFS_Copy(const char* Source, const char* Destination, long Flags __unused, SYSTEM_ERROR* Error)
{
    #define ErrorOut_VFS_Copy(Code) \
        ErrorOut(Error, Code, FUNC_VFS_Copy)

    FILE* SourceFileHandle = VFS_Open(Source, VFS_OpenFlag_READONLY, Error);
    if (Probe4Error(SourceFileHandle) || !SourceFileHandle)
    {
        ErrorOut_VFS_Copy(-ENOENT);
        return Error->ErrorCode;
    }

    FILE* DestinationFileHandle = VFS_Open(Destination, VFS_OpenFlag_CREATE | VFS_OpenFlag_WRITEONLY | VFS_OpenFlag_TRUNCATE, Error);
    if (Probe4Error(DestinationFileHandle) || !DestinationFileHandle)
    {
        VFS_Close(SourceFileHandle, Error);

        ErrorOut_VFS_Copy(-ENOENT);
        return Error->ErrorCode;
    }

    char Buffer[4096];
    for (;;)
    {
        long ReadIndex = VFS_Read(SourceFileHandle, Buffer, sizeof(Buffer), Error);
        if (ReadIndex < 0)
        {
            VFS_Close(SourceFileHandle, Error);
            VFS_Close(DestinationFileHandle, Error);

            ErrorOut_VFS_Copy(-Limits);
            return Error->ErrorCode;
        }

        if (ReadIndex == 0)
        {
            break;
        }

        long WriteIndex = VFS_Write(DestinationFileHandle, Buffer, ReadIndex, Error);
        if (WriteIndex != ReadIndex)
        {
            VFS_Close(SourceFileHandle, Error);
            VFS_Close(DestinationFileHandle, Error);
            
            ErrorOut_VFS_Copy(-Limits);
            return Error->ErrorCode;
        }
    }

    VFS_Close(SourceFileHandle, Error);
    VFS_Close(DestinationFileHandle, Error);
    
    return GeneralOK;
}

int
VFS_Move(const char* Source, const char* Destination, long Flags, SYSTEM_ERROR* Error)
{
    #define ErrorOut_VFS_Move(Code) \
        ErrorOut(Error, Code, FUNC_VFS_Move)

    int Return = VFS_ReName(Source, Destination, Flags, Error);
    if (Return == GeneralOK)
    {
        return GeneralOK;
    }

    Return = VFS_Copy(Source, Destination, Flags, Error);
    if (Return != GeneralOK)
    {
        ErrorOut_VFS_Move(Error->ErrorCode);
        return Error->ErrorCode;
    }
    
    return VFS_UnLink(Source, Error);
}
