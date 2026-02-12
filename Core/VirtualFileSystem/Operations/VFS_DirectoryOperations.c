#include <KernelCLibrary.h>
#include <VirtualFileSystem.h>
#include <__KCONF.h>
#include <DirtyHeap.h>

long
VfsReaddir(const char* Path, void* Buffer, long BufferLength, SYSTEM_ERROR* Error)
{
    #define ErrorOut_VfsReaddir(Code) \
        ErrorOut(Error, Code, FUNC_VfsReaddir)
    
    if (Probe4Error(Path) || !Path || Probe4Error(Buffer) || !Buffer || BufferLength <= 0)
    {
        ErrorOut_VfsReaddir(-EINVAL);
        return Error->ErrorCode;
    }

    DIRECTORY_ENTRY* DirectoryEntry = VfsResolve(Path, Error);
    if (Probe4Error(DirectoryEntry) || !DirectoryEntry || Probe4Error(DirectoryEntry->Node) || !DirectoryEntry->Node)
    {
        ErrorOut_VfsReaddir(-ENOENT);
        return Error->ErrorCode;
    }

    if (Probe4Error(DirectoryEntry->Node->Operations) || !DirectoryEntry->Node->Operations || Probe4Error(DirectoryEntry->Node->Operations->Readdir) || !DirectoryEntry->Node->Operations->Readdir)
    {
        ErrorOut_VfsReaddir(-ENOSYS);
        return Error->ErrorCode;
    }
    
    return DirectoryEntry->Node->Operations->Readdir(DirectoryEntry->Node, Buffer, BufferLength, Error);
}

long
VfsReaddirF(FILE* FileHandle, void* Buffer, long BufferLength, SYSTEM_ERROR* Error)
{
    #define ErrorOut_VfsReaddirF(Code) \
        ErrorOut(Error, Code, FUNC_VfsReaddirF)
    
    if (Probe4Error(FileHandle) || !FileHandle || Probe4Error(Buffer) || !Buffer || BufferLength <= 0)
    {
        ErrorOut_VfsReaddirF(-EINVAL);
        return Error->ErrorCode;
    }

    if (Probe4Error(FileHandle->Node) || !FileHandle->Node || Probe4Error(FileHandle->Node->Operations) || !FileHandle->Node->Operations || Probe4Error(FileHandle->Node->Operations->Readdir) || !FileHandle->Node->Operations->Readdir)
    {
        ErrorOut_VfsReaddirF(-ENOSYS);
        return Error->ErrorCode;
    }

    return FileHandle->Node->Operations->Readdir(FileHandle->Node, Buffer, BufferLength, Error);
}


int
VfsMkdir(const char* Path, VFS_PERMISSIONS Permission, SYSTEM_ERROR* Error)
{
    #define ErrorOut_VfsMkdir(Code) \
        ErrorOut(Error, Code, FUNC_VfsMkdir)

    DIRECTORY_ENTRY* Base = 0;
    char    Name[256];

    if (Probe4Error(Path) || !Path)
    {
        ErrorOut_VfsMkdir(-EINVAL);
        return Error->ErrorCode;
    }

    const char* PathComponent = Path;
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
            ErrorOut_VfsMkdir(-ENOSYS);
            return Error->ErrorCode;
        }

        VFS_NODE* Next = Current->Operations->Lookup(Current, Name, Error);
        if (Probe4Error(Next) || !Next)
        {
            ErrorOut_VfsMkdir(-ENOENT);
            return Error->ErrorCode;
        }

        char* Dublicate = (char*)KMalloc((size_t)(Index + 1), Error);
        if (Probe4Error(Dublicate) || !Dublicate)
        {
            ErrorOut_VfsMkdir(-ENOMEM);
            return Error->ErrorCode;
        }

        memcpy(Dublicate, Name, (size_t)(Index + 1));
        DirectoryEntry = AllocateDirectoryEntry(Dublicate, DirectoryEntry, Next, Error);
        if (Probe4Error(DirectoryEntry) || !DirectoryEntry)
        {
            ErrorOut_VfsMkdir(Error->ErrorCode);
            return Error->ErrorCode;
        }

        Current = Next;
    }
    if (Probe4Error(Base) || !Base || Probe4Error(Base->Node) || !Base->Node || Probe4Error(Base->Node->Operations) || !Base->Node->Operations || Probe4Error(Base->Node->Operations->Mkdir) || !Base->Node->Operations->Mkdir)
    {
        ErrorOut_VfsMkdir(-ENOSYS);
        return Error->ErrorCode;
    }
    
    return Base->Node->Operations->Mkdir(Base->Node, Name, Permission, Error);
}

int
VfsRmdir(const char* Path, SYSTEM_ERROR* Error)
{
    #define ErrorOut_VfsRmdir(Code) \
        ErrorOut(Error, Code, FUNC_VfsRmdir)

    DIRECTORY_ENTRY* Base = 0;
    char Name[256];
    if (Probe4Error(Path) || !Path)
    {
        ErrorOut_VfsRmdir(-EINVAL);
        return Error->ErrorCode;
    }
    
    const char* PathComponent = Path;
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
            ErrorOut_VfsRmdir(-ENOSYS);
            return Error->ErrorCode;
        }

        VFS_NODE* Next = Current->Operations->Lookup(Current, Name, Error);
        if (Probe4Error(Next) || !Next)
        {
            ErrorOut_VfsRmdir(-ENOENT);
            return Error->ErrorCode;
        }

        char* Dublicate = (char*)KMalloc(Index + 1, Error);
        memcpy(Dublicate, Name, Index + 1);
        DirectoryEntry  = AllocateDirectoryEntry(Dublicate, DirectoryEntry, Next, Error);
        Current = Next;
    }
    if (Probe4Error(Base) || !Base || Probe4Error(Base->Node) || !Base->Node || Probe4Error(Base->Node->Operations) || !Base->Node->Operations || Probe4Error(Base->Node->Operations->Rmdir) || !Base->Node->Operations->Rmdir)
    {
        ErrorOut_VfsRmdir(-ENOSYS);
        return Error->ErrorCode;
    }
    
    return Base->Node->Operations->Rmdir(Base->Node, Name, Error);
}