#include <KernelCLibrary.h>
#include <VirtualFileSystem.h>
#include <__KCONF.h>
#include <DirtyHeap.h>

int
VfsMkpath(const char* Path, long Permission, SYSTEM_ERROR* Error)
{
    #define ErrorOut_VfsMkpath(Code) \
        ErrorOut(Error, Code, FUNC_VfsMkpath)

    if (Probe4Error(Path) || !Path)
    {
        ErrorOut_VfsMkpath(-EINVAL);
        return Error->ErrorCode;
    }

    const char* PathComponent = Path;
    if (IsSeperator(*PathComponent, Error))
    {
        PathComponent = SkipSeperator(PathComponent, Error);
    }

    VFS_NODE* Current = RootNode;
    DIRECTORY_ENTRY* DirectoryEntry = RootDirectoryEntry;
    char Compare[256];

    while (*PathComponent)
    {
        long Index = NextCompare(PathComponent, Compare, sizeof(Compare), Error);
        if (Index <= 0)
        {
            break;
        }

        while (*PathComponent && !IsSeperator(*PathComponent, Error))
        {
            PathComponent++;
        }

        PathComponent = SkipSeperator(PathComponent, Error);

        VFS_NODE* Next = Current->Operations ? Current->Operations->Lookup(Current, Compare, Error) : 0;
        if (Probe4Error(Next) || !Next)
        {
            if (Probe4Error(Current->Operations) || !Current->Operations || Probe4Error(Current->Operations->Mkdir) || !Current->Operations->Mkdir)
            {
                ErrorOut_VfsMkpath(-ENOSYS);
                return Error->ErrorCode;
            }

            VFS_PERMISSIONS Permissions = 
            {
                .Mode = Permission, .UserID = 0, .GroupID = 0
            };

            if (Current->Operations->Mkdir(Current, Compare, Permissions, Error) != GeneralOK)
            {
                ErrorOut_VfsMkpath(Error->ErrorCode);
                return Error->ErrorCode;
            }

            Next = Current->Operations->Lookup(Current, Compare, Error);
            if (Probe4Error(Next) || !Next)
            {
                ErrorOut_VfsMkpath(Error->ErrorCode);
                return Error->ErrorCode;
            }
        }

        char* Dublicate = (char*)KMalloc(Index + 1, Error);
        memcpy(Dublicate, Compare, Index + 1);
        DirectoryEntry  = AllocateDirectoryEntry(Dublicate, DirectoryEntry, Next, Error);
        Current = Next;
    }
    
    return GeneralOK;
}

int
VfsRealpath(const char* Path, char* Buffer, long Length, SYSTEM_ERROR* Error)
{
    #define ErrorOut_VfsRealpath(Code) \
        ErrorOut(Error, Code, FUNC_VfsRealpath)

    if (Probe4Error(Path) || !Path || Probe4Error(Buffer) || !Buffer || Length <= 0)
    {
        ErrorOut_VfsRealpath(-EINVAL);
        return Error->ErrorCode;
    }

    long LengthComponent = (long)strlen(Path);
    if (LengthComponent >= Length)
    {
        ErrorOut_VfsRealpath(-Limits);
        return Error->ErrorCode;
    }

    memcpy(Buffer, Path, (size_t)(LengthComponent + 1));

    return GeneralOK;
}