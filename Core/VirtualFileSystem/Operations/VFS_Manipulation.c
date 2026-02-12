#include <KernelCLibrary.h>
#include <VirtualFileSystem.h>
#include <__KCONF.h>
#include <DirtyHeap.h>

int
VfsRename(const char* OldPath, const char* NewPath, long Flags, SYSTEM_ERROR* Error)
{
    #define ErrorOut_VfsRename(Code) \
        ErrorOut(Error, Code, FUNC_VfsRename)

    DIRECTORY_ENTRY* OldBase = 0;
    DIRECTORY_ENTRY* NewBase = 0;
    char OldName[256];
    char NewName[256];

    if (Probe4Error(OldPath) || !OldPath || Probe4Error(NewPath) || !NewPath)
    {
        ErrorOut_VfsRename(-EINVAL);
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
            ErrorOut_VfsRename(-ENOSYS);
            return Error->ErrorCode;
        }

        VFS_NODE* Next = CurrentOldNode->Operations->Lookup(CurrentOldNode, OldName, Error);
        if (Probe4Error(Next) || !Next)
        {
            ErrorOut_VfsRename(-ENOENT);
            return Error->ErrorCode;
        }

        char* Dublicate = (char*)KMalloc((size_t)(Index + 1), Error);
        if (Probe4Error(Dublicate) || !Dublicate)
        {
            ErrorOut_VfsRename(-ENOMEM);
            return Error->ErrorCode;
        }

        memcpy(Dublicate, OldName, (size_t)(Index + 1));
        OldDirectoryEntry = AllocateDirectoryEntry(Dublicate, OldDirectoryEntry, Next, Error);
        if (Probe4Error(OldDirectoryEntry) || !OldDirectoryEntry)
        {
            ErrorOut_VfsRename(Error->ErrorCode);
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
            ErrorOut_VfsRename(-ENOSYS);
            return Error->ErrorCode;
        }

        VFS_NODE* Next = CurrentNewNode->Operations->Lookup(CurrentNewNode, NewName, Error);
        if (Probe4Error(Next) || !Next)
        {
            ErrorOut_VfsRename(-ENOENT);
            return Error->ErrorCode;
        }

        char* Dublicate = (char*)KMalloc((size_t)(Index + 1), Error);
        if (Probe4Error(Dublicate) || !Dublicate)
        {
            ErrorOut_VfsRename(-ENOMEM);
            return Error->ErrorCode;
        }

        memcpy(Dublicate, NewName, (size_t)(Index + 1));
        NewDirectoryEntry = AllocateDirectoryEntry(Dublicate, NewDirectoryEntry, Next, Error);
        if (Probe4Error(NewDirectoryEntry) || !NewDirectoryEntry)
        {
            ErrorOut_VfsRename(Error->ErrorCode);
            return Error->ErrorCode;
        }

        CurrentNewNode = Next;
    }

    if (Probe4Error(OldBase) || !OldBase || Probe4Error(NewBase) || !NewBase)
    {
        ErrorOut_VfsRename(-ENOENT);
        return Error->ErrorCode;
    }

    if (Probe4Error(OldBase->Node) || !OldBase->Node || Probe4Error(NewBase->Node) || !NewBase->Node)
    {
        ErrorOut_VfsRename(-ENOENT);
        return Error->ErrorCode;
    }

    if (Probe4Error(OldBase->Node->Operations) || !OldBase->Node->Operations || Probe4Error(OldBase->Node->Operations->Rename) || !OldBase->Node->Operations->Rename)
    {
        ErrorOut_VfsRename(-ENOSYS);
        return Error->ErrorCode;
    }
    
    return OldBase->Node->Operations->Rename(OldBase->Node, OldName, NewBase->Node, NewName, Flags, Error);
}

int
VfsTruncate(const char* Path, long Length, SYSTEM_ERROR* Error)
{
    #define ErrorOut_VfsTruncate(Code) \
        ErrorOut(Error, Code, FUNC_VfsTruncate)

    DIRECTORY_ENTRY* DirectoryEntry = VfsResolve(Path, Error);
    if (Probe4Error(DirectoryEntry) || !DirectoryEntry || Probe4Error(DirectoryEntry->Node) || !DirectoryEntry->Node)
    {
        ErrorOut_VfsTruncate(-ENOENT);
        return Error->ErrorCode;
    }

    if (Probe4Error(DirectoryEntry->Node->Operations) || !DirectoryEntry->Node->Operations || Probe4Error(DirectoryEntry->Node->Operations->Truncate) || !DirectoryEntry->Node->Operations->Truncate)
    {
        ErrorOut_VfsTruncate(-ENOSYS);
        return Error->ErrorCode;
    }
    
    return DirectoryEntry->Node->Operations->Truncate(DirectoryEntry->Node, Length, Error);
}

int
VfsCopy(const char* Source, const char* Destination, long Flags __UNUSED, SYSTEM_ERROR* Error)
{
    #define ErrorOut_VfsCopy(Code) \
        ErrorOut(Error, Code, FUNC_VfsCopy)

    FILE* SourceFileHandle = VfsOpen(Source, VfsOpenFlag_READONLY, Error);
    if (Probe4Error(SourceFileHandle) || !SourceFileHandle)
    {
        ErrorOut_VfsCopy(-ENOENT);
        return Error->ErrorCode;
    }

    FILE* DestinationFileHandle = VfsOpen(Destination, VfsOpenFlag_CREATE | VfsOpenFlag_WRITEONLY | VfsOpenFlag_TRUNCATE, Error);
    if (Probe4Error(DestinationFileHandle) || !DestinationFileHandle)
    {
        VfsClose(SourceFileHandle, Error);

        ErrorOut_VfsCopy(-ENOENT);
        return Error->ErrorCode;
    }

    char Buffer[4096];
    for (;;)
    {
        long ReadIndex = VfsRead(SourceFileHandle, Buffer, sizeof(Buffer), Error);
        if (ReadIndex < 0)
        {
            VfsClose(SourceFileHandle, Error);
            VfsClose(DestinationFileHandle, Error);

            ErrorOut_VfsCopy(-Limits);
            return Error->ErrorCode;
        }

        if (ReadIndex == 0)
        {
            break;
        }

        long WriteIndex = VfsWrite(DestinationFileHandle, Buffer, ReadIndex, Error);
        if (WriteIndex != ReadIndex)
        {
            VfsClose(SourceFileHandle, Error);
            VfsClose(DestinationFileHandle, Error);
            
            ErrorOut_VfsCopy(-Limits);
            return Error->ErrorCode;
        }
    }

    VfsClose(SourceFileHandle, Error);
    VfsClose(DestinationFileHandle, Error);
    
    return GeneralOK;
}

int
VfsMove(const char* Source, const char* Destination, long Flags, SYSTEM_ERROR* Error)
{
    #define ErrorOut_VfsMove(Code) \
        ErrorOut(Error, Code, FUNC_VfsMove)

    int Return = VfsRename(Source, Destination, Flags, Error);
    if (Return == GeneralOK)
    {
        return GeneralOK;
    }

    Return = VfsCopy(Source, Destination, Flags, Error);
    if (Return != GeneralOK)
    {
        ErrorOut_VfsMove(Error->ErrorCode);
        return Error->ErrorCode;
    }
    
    return VfsUnlink(Source, Error);
}
