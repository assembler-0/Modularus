#include <KernelCLibrary.h>
#include <VirtualFileSystem.h>
#include <__KCONF.h>
#include <DirtyHeap.h>

int
VFS_RegisterDeviceNode(const char* Path, void* Private, long Flags __unused, SYSTEM_ERROR* Error)
{
    #define ErrorOut_VFS_RegisterDeviceNode(Code) \
        ErrorOut(Error, Code, FUNC_VFS_RegisterDeviceNode)

    if (Probe4Error(Path) || !Path || Probe4Error(Private) || !Private)
    {
        ErrorOut_VFS_RegisterDeviceNode(-EINVAL);
        return Error->ErrorCode;
    }

    /*Parent must exist*/
    char Buffer[1024];
    VFS_RealPath(Path, Buffer, sizeof(Buffer), Error);
    const char* Name = strrchr(Buffer, '/');
    if (Probe4Error(Name) || !Name)
    {
        ErrorOut_VFS_RegisterDeviceNode(-EINVAL);
        return Error->ErrorCode;
    }

    long NodeLength = (long)strlen(Name + 1);
    char Parent[1024];
    long ParentLength = (long)(Name - Buffer);
    memcpy(Parent, Buffer, ParentLength);
    Parent[ParentLength] = 0;
    VFS_MakePath(Parent, 0, Error);

    VFS_NODE* Node = (VFS_NODE*)KMalloc(sizeof(VFS_NODE), Error);
    if (Probe4Error(Node) || !Node)
    {
        ErrorOut_VFS_RegisterDeviceNode(-ENOMEM);
        return Error->ErrorCode;
    }

    Node->Type = VFSNode_DEVICE;
    Node->Operations = (VFS_NODE_OPERATIONS*)Private; /* device ops table */
    Node->SuperBlock = RootNode->SuperBlock;
    Node->Private = Private;
    Node->ReferenceCount = 1;

    char* Dublicate = (char*)KMalloc(NodeLength + 1, Error);
    memcpy(Dublicate, Name + 1, NodeLength + 1);
    DIRECTORY_ENTRY* DirectoryEntry = AllocateDirectoryEntry(Dublicate, RootDirectoryEntry, Node, Error);
    if (Probe4Error(DirectoryEntry) || !DirectoryEntry)
    {
        KFree(Node, Error);
        
        ErrorOut_VFS_RegisterDeviceNode(Error->ErrorCode);
        return Error->ErrorCode;
    }
    
    return GeneralOK;
}

int
VFS_UnRegisterDeviceNode(const char* Path __unused, SYSTEM_ERROR* Error __unused)
{
    return GeneralOK;
}

int
VFS_RegisterPseudoFileSystem(const char* Path, SUPER_BLOCK* SuperBlock, SYSTEM_ERROR* Error)
{
    #define ErrorOut_VFS_RegisterPseudoFileSystem(Code) \
        ErrorOut(Error, Code, FUNC_VFS_RegisterPseudoFileSystem)

    if (Probe4Error(Path) || !Path || Probe4Error(SuperBlock) || !SuperBlock)
    {
        ErrorOut_VFS_RegisterPseudoFileSystem(-EINVAL);
        return Error->ErrorCode;
    }

    if (MountsCount >= MaxMounts)
    {
        ErrorOut_VFS_RegisterPseudoFileSystem(-Limits);
        return Error->ErrorCode;
    }

    long Index = (long)strlen(Path);
    MOUNT_ENTRY* MountEntry = &Mounts[MountsCount++];
    MountEntry->SuperBlock = SuperBlock;
    memcpy(MountEntry->Path, Path, (size_t)(Index + 1));
    
    return GeneralOK;
}

int
VFS_UnRegisterPseudoFileSystem(const char* Path, SYSTEM_ERROR* Error)
{
    return VFS_UnMount(Path, Error);
}

int
VFS_SetDefaultFileSystem(const char* Name, SYSTEM_ERROR* Error)
{
    #define ErrorOut_VFS_SetDefaultFileSystem(Code) \
        ErrorOut(Error, Code, FUNC_VFS_SetDefaultFileSystem)

    if (Probe4Error(Name) || !Name)
    {
        ErrorOut_VFS_SetDefaultFileSystem(-EINVAL);
        return Error->ErrorCode;
    }

    long Index = (long)strlen(Name);
    if (Index >= (long)sizeof(DefaultFileSystem))
    {
        ErrorOut_VFS_SetDefaultFileSystem(-Limits);
        return Error->ErrorCode;
    }

    memcpy(DefaultFileSystem, Name, (size_t)(Index + 1));
    
    return GeneralOK;
}

const char*
VFS_GetDefaultFileSystem(SYSTEM_ERROR* Error __unused)
{
    return DefaultFileSystem;
}
