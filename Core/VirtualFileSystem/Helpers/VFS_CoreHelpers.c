#include <KernelCLibrary.h>
#include <VirtualFileSystem.h>
#include <__KCONF.h>
#include <DirtyHeap.h>

DIRECTORY_ENTRY*
AllocateDirectoryEntry(const char* Name, DIRECTORY_ENTRY* Parent, VFS_NODE* Node, SYSTEM_ERROR* Error)
{
    #define ErrorOut_AllocateDirectoryEntry(Code) \
        ErrorOut(Error, Code, FUNC_AllocateDirectoryEntry)

    DIRECTORY_ENTRY* DirectoryEntry = (DIRECTORY_ENTRY*)KMalloc(sizeof(DIRECTORY_ENTRY), Error);
    if (Probe4Error(DirectoryEntry) || !DirectoryEntry)
    {
        ErrorOut_AllocateDirectoryEntry(-ENOMEM);
        return Error2Pointer(Error->ErrorCode);
    }

    DirectoryEntry->Name   = Name;
    DirectoryEntry->Parent = Parent;
    DirectoryEntry->Node   = Node;
    DirectoryEntry->Flags  = 0;
    return DirectoryEntry;
}

DIRECTORY_ENTRY*
Walk(VFS_NODE* StartingNode, DIRECTORY_ENTRY* StartingDirectoryEntry, const char* Path, SYSTEM_ERROR* Error)
{
    #define ErrorOut_Walk(Code) \
        ErrorOut(Error, Code, FUNC_Walk)

    if (Probe4Error(StartingNode) || !StartingNode || Probe4Error(Path) || !Path)
    {
        return Error2Pointer(-EINVAL);
    }
    const char* PathComponent = Path;
    if (IsSeperator(*PathComponent, Error))
    {
        PathComponent = SkipSeperator(PathComponent, Error);
    }
    
    VFS_NODE* Current = StartingNode;
    DIRECTORY_ENTRY* Parent = StartingDirectoryEntry;
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
        if (Probe4Error(Current) || !Current || Probe4Error(Current->Operations) || !Current->Operations ||
            Probe4Error(Current->Operations->Lookup) || !Current->Operations->Lookup)
        {
            return Error2Pointer(-EOPNOTSUPP);
        }

        VFS_NODE* Next = Current->Operations->Lookup(Current, Compare, Error);
        if (Probe4Error(Next) || !Next)
        {
            return Error2Pointer(-ENOENT);
        }

        char* Dublicate = (char*)KMalloc((size_t)(Index + 1), Error);
        if (Probe4Error(Dublicate) || !Dublicate)
        {
            return Error2Pointer(-ENOMEM);
        }

        memcpy(Dublicate, Compare, (size_t)(Index + 1));
        DIRECTORY_ENTRY* DirectoryEntry = AllocateDirectoryEntry(Dublicate, Parent, Next, Error);
        if (Probe4Error(DirectoryEntry) || !DirectoryEntry)
        {
            return Error2Pointer(-ENOENT);
        }

        Parent = DirectoryEntry;
        Current = Next;
    }
    return Parent;
}

MOUNT_ENTRY*
FindMount(const char* Path, SYSTEM_ERROR* Error __unused)
{
    long Best = -GeneralError;
    long BestLength = -GeneralError;
    for (long Iteration = 0; Iteration < MountsCount; Iteration++)
    {
        const char* MountPoint = Mounts[Iteration].Path;
        long        MountLength = (long)strlen(MountPoint);
        if (MountLength <= 0)
        {
            continue;
        }
        if (strncmp(Path, MountPoint, (size_t)MountLength) == 0)
        {
            if (MountLength > BestLength)
            {
                Best    = Iteration;
                BestLength = MountLength;
            }
        }
    }
    return Best >= 0 ? &Mounts[Best] : 0;
}