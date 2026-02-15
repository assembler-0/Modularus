
#include <System.h>
#include <VirtualFileSystem.h>
#include <DirtyHeap.h>
#include <KernelCLibrary.h>

long
System_ReadDirectory(VFS_NODE* VFSNode, void* Buffer, long Size, SYSTEM_ERROR* Error)
{
    #define ErrorOut_System_ReadDirectory(Code) \
        ErrorOut(Error, Code, FUNC_System_ReadDirectory)

    if (Probe4Error(VFSNode) || !VFSNode || Probe4Error(Buffer) || !Buffer)
    {
        ErrorOut_System_ReadDirectory(-EINVAL);
        return Error->ErrorCode;
    }
    
    SYSTEM_NODE* Node = (SYSTEM_NODE*)VFSNode->Private;
    
    if (Probe4Error(Node) || !Node || Node->Type != SystemNodeTypeEnumeration_DIRECTORY)
    {
        ErrorOut_System_ReadDirectory(-ENOTDIR);
        return Error->ErrorCode;
    }
    
    VFS_DIRECTORY_ENTRY* DirectoryEntry = (VFS_DIRECTORY_ENTRY*)Buffer;
    long Count = 0;
    SYSTEM_NODE* Child = Node->Child;
    
    while (Child && (Count + 1) * sizeof(VFS_DIRECTORY_ENTRY) <= (uint64_t)Size)
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
System_Stat(VFS_NODE* VFSNode, VFS_STAT* Stat, SYSTEM_ERROR* Error)
{
    #define ErrorOut_System_Stat(Code) \
        ErrorOut(Error, Code, FUNC_System_Stat)

    if (Probe4Error(VFSNode) || !VFSNode || Probe4Error(Stat) || !Stat)
    {
        ErrorOut_System_Stat(-EINVAL);
        return Error->ErrorCode;
    }
    
    SYSTEM_NODE* Node = (SYSTEM_NODE*)VFSNode->Private;
    
    if (Probe4Error(Node) || !Node)
    {
        ErrorOut_System_Stat(-ENOENT);
        return Error->ErrorCode;
    }
    
    return System_GetAttribute(Node, Stat, Error);
}

int
System_StatFileSystem(SUPER_BLOCK* SuperBlock, VFS_STAT_FILESYSTEM* Stat, SYSTEM_ERROR* Error)
{
    #define ErrorOut_System_StatFileSystem(Code) \
        ErrorOut(Error, Code, FUNC_System_StatFileSystem)

    if (Probe4Error(SuperBlock) || !SuperBlock || Probe4Error(Stat) || !Stat)
    {
        ErrorOut_System_StatFileSystem(-EINVAL);
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
System_Sync(SUPER_BLOCK* SuperBlock __unused, SYSTEM_ERROR* Error __unused)
{
    return GeneralOK;
}

void
System_Release(SUPER_BLOCK* SuperBlock, SYSTEM_ERROR* Error __unused)
{
    if (SystemInstance && SystemInstance->SuperBlock == SuperBlock)
    {
        SystemInstance->SuperBlock = 0;
    }
}