#include <KernelCLibrary.h>
#include <VirtualFileSystem.h>
#include <__KCONF.h>
#include <DirtyHeap.h>

int
VfsRegisterDevNode(const char* Path, void* Private, long Flags __UNUSED, SYSTEM_ERROR* Error)
{
    #define ErrorOut_VfsRegisterDevNode(Code) \
        ErrorOut(Error, Code, FUNC_VfsRegisterDevNode)

    if (Probe4Error(Path) || !Path || Probe4Error(Private) || !Private)
    {
        ErrorOut_VfsRegisterDevNode(-EINVAL);
        return -EINVAL;
    }

    /*Parent must exist*/
    char Buffer[1024];
    VfsRealpath(Path, Buffer, sizeof(Buffer), Error);
    const char* Name = strrchr(Buffer, '/');
    if (Probe4Error(Name) || !Name)
    {
        ErrorOut_VfsRegisterDevNode(-EINVAL);
        return -EINVAL;
    }

    long NodeLength = (long)strlen(Name + 1);
    char Parent[1024];
    long ParentLength = (long)(Name - Buffer);
    memcpy(Parent, Buffer, ParentLength);
    Parent[ParentLength] = 0;
    VfsMkpath(Parent, 0, Error);

    VFS_NODE* Node = (VFS_NODE*)KMalloc(sizeof(VFS_NODE), Error);
    if (Probe4Error(Node) || !Node)
    {
        ErrorOut_VfsRegisterDevNode(-ENOMEM);
        return -ENOMEM;
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
        
        ErrorOut_VfsRegisterDevNode(Error->ErrorCode);
        return Error->ErrorCode;
    }
    
    return GeneralOK;
}

int
VfsUnregisterDevNode(const char* Path __UNUSED, SYSTEM_ERROR* Error __UNUSED)
{
    return GeneralOK;
}

int
VfsRegisterPseudoFs(const char* Path, SUPER_BLOCK* SuperBlock, SYSTEM_ERROR* Error)
{
    #define ErrorOut_VfsRegisterPseudoFs(Code) \
        ErrorOut(Error, Code, FUNC_VfsRegisterPseudoFs)

    if (Probe4Error(Path) || !Path || Probe4Error(SuperBlock) || !SuperBlock)
    {
        ErrorOut_VfsRegisterPseudoFs(-EINVAL);
        return -EINVAL;
    }

    if (MountsCount >= MaxMounts)
    {
        ErrorOut_VfsRegisterPseudoFs(-Limits);
        return -Limits;
    }

    long Index = (long)strlen(Path);
    MOUNT_ENTRY* MountEntry = &Mounts[MountsCount++];
    MountEntry->SuperBlock = SuperBlock;
    memcpy(MountEntry->Path, Path, (size_t)(Index + 1));
    
    return GeneralOK;
}

int
VfsUnregisterPseudoFs(const char* Path, SYSTEM_ERROR* Error)
{
    return VfsUnmount(Path, Error);
}

int
VfsSetDefaultFs(const char* Name, SYSTEM_ERROR* Error)
{
    #define ErrorOut_VfsSetDefaultFs(Code) \
        ErrorOut(Error, Code, FUNC_VfsSetDefaultFs)

    if (Probe4Error(Name) || !Name)
    {
        ErrorOut_VfsSetDefaultFs(-EINVAL);
        return -EINVAL;
    }

    long Index = (long)strlen(Name);
    if (Index >= (long)sizeof(DefaultFileSystem))
    {
        ErrorOut_VfsSetDefaultFs(-Limits);
        return -Limits;
    }

    memcpy(DefaultFileSystem, Name, (size_t)(Index + 1));
    
    return GeneralOK;
}

const char*
VfsGetDefaultFs(SYSTEM_ERROR* Error __UNUSED)
{
    return DefaultFileSystem;
}
