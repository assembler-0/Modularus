
#include <System.h>
#include <VirtualFileSystem.h>
#include <DirtyHeap.h>
#include <KernelCLibrary.h>

long
SystemVfsReaddir(VFS_NODE* VFSNode, void* Buffer, uint64_t Size, SYSTEM_ERROR* Error)
{
    #define ErrorOut_SystemVfsReaddir(Code) \
        ErrorOut(Error, Code, FUNC_SystemVfsReaddir)

    if (Probe4Error(VFSNode) || !VFSNode || Probe4Error(Buffer) || !Buffer)
    {
        ErrorOut_SystemVfsReaddir(-EINVAL);
        return Error->ErrorCode;
    }
    
    SYSTEM_NODE* Node = (SYSTEM_NODE*)VFSNode->Private;
    
    if (Probe4Error(Node) || !Node || Node->Type != SYSTEM_NODE_TYPE_ENUMERATION_DIRECTORY)
    {
        ErrorOut_SystemVfsReaddir(-ENOTDIR);
        return Error->ErrorCode;
    }
    
    VFS_DIRECTORY_ENTRY* DirectoryEntry = (VFS_DIRECTORY_ENTRY*)Buffer;
    long Count = 0;
    SYSTEM_NODE* Child = Node->Child;
    
    while (Child && (Count + 1) * sizeof(VFS_DIRECTORY_ENTRY) <= Size)
    {
        strcpy(DirectoryEntry[Count].Name, Child->Name, sizeof(DirectoryEntry[Count].Name));
        DirectoryEntry[Count].Type = Child->Type;
        DirectoryEntry[Count].Inode = (long)Child;
        Count++;
        Child = Child->Next;
    }
    
    return Count * sizeof(VFS_DIRECTORY_ENTRY);
}

int
SystemVfsStat(VFS_NODE* VFSNode, VFS_STAT* Stat, SYSTEM_ERROR* Error)
{
    #define ErrorOut_SystemVfsStat(Code) \
        ErrorOut(Error, Code, FUNC_SystemVfsStat)

    if (Probe4Error(VFSNode) || !VFSNode || Probe4Error(Stat) || !Stat)
    {
        ErrorOut_SystemVfsStat(-EINVAL);
        return Error->ErrorCode;
    }
    
    SYSTEM_NODE* Node = (SYSTEM_NODE*)VFSNode->Private;
    
    if (Probe4Error(Node) || !Node)
    {
        ErrorOut_SystemVfsStat(-ENOENT);
        return Error->ErrorCode;
    }
    
    return SystemGetattr(Node, Stat, Error);
}

int
SystemStatFs(SUPER_BLOCK* SuperBlock, VFS_STAT_FILESYSTEM* Stat, SYSTEM_ERROR* Error)
{
    #define ErrorOut_SystemStatFs(Code) \
        ErrorOut(Error, Code, FUNC_SystemStatFs)

    if (Probe4Error(SuperBlock) || !SuperBlock || Probe4Error(Stat) || !Stat)
    {
        ErrorOut_SystemStatFs(-EINVAL);
        return Error->ErrorCode;
    }
    
    Stat->TypeID = 0;
    Stat->BlockSize = 4096;
    Stat->Blocks = 0;
    Stat->BlocksFree = 0;
    Stat->BlocksAvailable = 0;
    Stat->Files = SystemInstance ? SystemInstance->FileCount : 0;
    Stat->FilesFree = MaxSystemFiles - FileAllocatedCount;
    Stat->NameLength = 255;
    Stat->Flags = 0;
    
    return GeneralOK;
}

int
SystemSync(SUPER_BLOCK* SuperBlock __UNUSED, SYSTEM_ERROR* Error __UNUSED)
{
    return GeneralOK;
}

void
SystemRelease(SUPER_BLOCK* SuperBlock, SYSTEM_ERROR* Error __UNUSED)
{
    if (SystemInstance && SystemInstance->SuperBlock == SuperBlock)
    {
        SystemInstance->SuperBlock = 0;
    }
}