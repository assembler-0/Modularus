#include <KernelCLibrary.h>
#include <VirtualFileSystem.h>
#include <__KCONF.h>
#include <DirtyHeap.h>

int
VfsSymlink(const char* TargetPath, const char* LinkPath, VFS_PERMISSIONS Permission, SYSTEM_ERROR* Error)
{
    #define ErrorOut_VfsSymlink(Code) \
        ErrorOut(Error, Code, FUNC_VfsSymlink)

    DIRECTORY_ENTRY* Base = NULL;
    char Name[256];
    if (Probe4Error(LinkPath) || !LinkPath || Probe4Error(TargetPath) || !TargetPath)
    {
        ErrorOut_VfsSymlink(-EINVAL);
        return Error->ErrorCode;
    }

    const char* PathComponent = LinkPath;
    if (IsSeperator(*PathComponent, Error))
    {
        PathComponent = SkipSeperator(PathComponent, Error);
    }
    
    VFS_NODE* Current = RootNode;
    DIRECTORY_ENTRY* DirectoryEntry = RootDirectoryEntry;
    while (*PathComponent)
    {
        long Index = NextCompare(PathComponent, Name, sizeof(Name), Error);
        if (Index <= 0)
        {
            break;
        }

        while (*PathComponent && !IsSeperator(*PathComponent, Error))
        {
            PathComponent++;
        }

        PathComponent = SkipSeperator(PathComponent, Error);
        if (*PathComponent == 0)
        {
            Base = DirectoryEntry;
            break;
        }

        if (Probe4Error(Current) || !Current || Probe4Error(Current->Operations) || !Current->Operations || Probe4Error(Current->Operations->Lookup) || !Current->Operations->Lookup)
        {
            ErrorOut_VfsSymlink(-ENOSYS);
            return Error->ErrorCode;
        }

        VFS_NODE* Next = Current->Operations->Lookup(Current, Name, Error);
        if (Probe4Error(Next) || !Next)
        {
            ErrorOut_VfsSymlink(-ENOENT);
            return Error->ErrorCode;
        }

        char* Dublicate = (char*)KMalloc((size_t)(Index + 1), Error);
        if (Probe4Error(Dublicate) || !Dublicate)
        {
            ErrorOut_VfsSymlink(-ENOMEM);
            return Error->ErrorCode;
        }

        memcpy(Dublicate, Name, (size_t)(Index + 1));
        DirectoryEntry = AllocateDirectoryEntry(Dublicate, DirectoryEntry, Next, Error);
        if (Probe4Error(DirectoryEntry) || !DirectoryEntry)
        {
            ErrorOut_VfsSymlink(Error->ErrorCode);
            return Error->ErrorCode;
        }

        Current = Next;
    }

    if (Probe4Error(Base) || !Base || Probe4Error(Base->Node) || !Base->Node || Probe4Error(Base->Node->Operations) || !Base->Node->Operations || Probe4Error(Base->Node->Operations->Symlink) || !Base->Node->Operations->Symlink)
    {
        ErrorOut_VfsSymlink(-ENOSYS);
        return Error->ErrorCode;
    }
    
    return Base->Node->Operations->Symlink(Base->Node, Name, TargetPath, Permission, Error);
}

int
VfsReadlink(const char* Path, char* Buffer, long Length, SYSTEM_ERROR* Error)
{
    #define ErrorOut_VfsReadlink(Code) \
        ErrorOut(Error, Code, FUNC_VfsReadlink)

    if (Probe4Error(Path) || !Path || Probe4Error(Buffer) || !Buffer || Length <= 0)
    {
        ErrorOut_VfsReadlink(-EINVAL);
        return Error->ErrorCode;
    }

    DIRECTORY_ENTRY* DirectoryEntry = VfsResolve(Path, Error);
    if (Probe4Error(DirectoryEntry) || !DirectoryEntry || Probe4Error(DirectoryEntry->Node) || !DirectoryEntry->Node)
    {
        ErrorOut_VfsReadlink(-ENOENT);
        return Error->ErrorCode;
    }

    if (Probe4Error(DirectoryEntry->Node->Operations) || !DirectoryEntry->Node->Operations || Probe4Error(DirectoryEntry->Node->Operations->Readlink) || !DirectoryEntry->Node->Operations->Readlink)
    {
        ErrorOut_VfsReadlink(-ENOSYS);
        return Error->ErrorCode;
    }

    VFS_NAME_BUFFER NameBuffer;
    NameBuffer.Buffer = Buffer;
    NameBuffer.Length = Length;
    
    return DirectoryEntry->Node->Operations->Readlink(DirectoryEntry->Node, &NameBuffer, Error);
}

int
VfsLink(const char* OldPath, const char* NewPath, SYSTEM_ERROR* Error)
{
    #define ErrorOut_VfsLink(Code) \
        ErrorOut(Error, Code, FUNC_VfsLink)

    if (Probe4Error(OldPath) || !OldPath || Probe4Error(NewPath) || !NewPath)
    {
        ErrorOut_VfsLink(-EINVAL);
        return Error->ErrorCode;
    }

    DIRECTORY_ENTRY* OldDirectoryEntry = VfsResolve(OldPath, Error);
    DIRECTORY_ENTRY* NewBase = 0;
    char Name[256];

    if (Probe4Error(OldDirectoryEntry) || !OldDirectoryEntry || Probe4Error(OldDirectoryEntry->Node) || !OldDirectoryEntry->Node)
    {
        ErrorOut_VfsLink(-ENOENT);
        return Error->ErrorCode;
    }

    const char* PathComponent = NewPath;
    if (IsSeperator(*PathComponent, Error))
    {
        PathComponent = SkipSeperator(PathComponent, Error);
    }

    VFS_NODE* Current = RootNode;
    DIRECTORY_ENTRY* DirectoryEntry = RootDirectoryEntry;
    while (*PathComponent)
    {
        long Index = NextCompare(PathComponent, Name, sizeof(Name), Error);
        if (Index <= 0)
        {
            break;
        }

        while (*PathComponent && !IsSeperator(*PathComponent, Error))
        {
            PathComponent++;
        }

        PathComponent = SkipSeperator(PathComponent, Error);
        if (*PathComponent == 0)
        {
            NewBase = DirectoryEntry;
            break;
        }

        if (Probe4Error(Current) || !Current || Probe4Error(Current->Operations) || !Current->Operations || Probe4Error(Current->Operations->Lookup) || !Current->Operations->Lookup)
        {
            ErrorOut_VfsLink(-ENOSYS);
            return Error->ErrorCode;
        }

        VFS_NODE* Next = Current->Operations->Lookup(Current, Name, Error);
        if (Probe4Error(Next) || !Next)
        {
            ErrorOut_VfsLink(-ENOENT);
            return Error->ErrorCode;
        }

        char* Dublicate = (char*)KMalloc((size_t)(Index + 1), Error);
        if (Probe4Error(Dublicate) || !Dublicate)
        {
            ErrorOut_VfsLink(-ENOMEM);
            return Error->ErrorCode;
        }

        memcpy(Dublicate, Name, (size_t)(Index + 1));
        DirectoryEntry = AllocateDirectoryEntry(Dublicate, DirectoryEntry, Next, Error);
        if (Probe4Error(DirectoryEntry) || !DirectoryEntry)
        {
            ErrorOut_VfsLink(Error->ErrorCode);
            return Error->ErrorCode;
        }
        Current = Next;
    }

    if (Probe4Error(NewBase) || !NewBase || Probe4Error(NewBase->Node) || !NewBase->Node || Probe4Error(NewBase->Node->Operations) || !NewBase->Node->Operations || Probe4Error(NewBase->Node->Operations->Link) || !NewBase->Node->Operations->Link)
    {
        ErrorOut_VfsLink(-ENOSYS);
        return Error->ErrorCode;
    }
    
    return NewBase->Node->Operations->Link(NewBase->Node, OldDirectoryEntry->Node, Name, Error);
}
