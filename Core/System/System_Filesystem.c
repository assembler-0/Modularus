#include <System.h>
#include <VirtualFileSystem.h>
#include <DirtyHeap.h>
#include <KernelCLibrary.h>

FILESYSTEM_TYPE SystemFileSystemType =
{
    .Name = "sysfs++",
    .Mount = 0,
    .Private = 0
};

SUPER_OPERATIONS SystemSuperOperations =
{
    .Sync = System_Sync,
    .StatFs = System_StatFileSystem,
    .Release = System_Release,
    .Umount = System_UnMount
};

VFS_NODE_OPERATIONS SystemVfsOperations =
{
    .Open = System_Open,
    .Close = System_Close,
    .Read = System_Read,
    .Write = System_Write,
    .Lseek = 0,
    .Ioctl = 0,
    .Stat = System_Stat,
    .Readdir = System_ReadDirectory,
    .Lookup = System_LookUp,
    .Create = 0,
    .Unlink = 0,
    .Mkdir = 0,
    .Rmdir = 0,
    .Symlink = 0,
    .Readlink = 0,
    .Link = 0,
    .Rename = 0,
    .Chmod = 0,
    .Chown = 0,
    .Truncate = 0,
    .Sync = 0,
    .Map = 0,
    .Unmap = 0
};

SUPER_BLOCK*
System_Mount(const char* Device __unused, const char* Options __unused, SYSTEM_ERROR* Error)
{
    #define ErrorOut_System_Mount(Code) \
        ErrorOut(Error, Code, FUNC_System_Mount)

    SUPER_BLOCK* SuperBlock = KMalloc(sizeof(SUPER_BLOCK), Error);
    if (Probe4Error(SuperBlock) || !SuperBlock)
    {
        ErrorOut_System_Mount(-ENOMEM);
        return Error2Pointer(Error->ErrorCode);
    }

    VFS_NODE* RootNode = KMalloc(sizeof(VFS_NODE), Error);
    if (Probe4Error(RootNode) || !RootNode)
    {
        KFree(SuperBlock, Error);

        ErrorOut_System_Mount(-ENOMEM);
        return Error2Pointer(Error->ErrorCode);
    }

    SuperBlock->Type = &SystemFileSystemType;
    SuperBlock->Device = 0;
    SuperBlock->Flags = 0;
    SuperBlock->Root = RootNode;
    SuperBlock->Operations = &SystemSuperOperations;
    SuperBlock->Private = SystemInstance;

    memset(RootNode, 0, sizeof(VFS_NODE));
    RootNode->Type = VFSNode_DIRECTORY;
    RootNode->Operations = &SystemVfsOperations;
    RootNode->SuperBlock = SuperBlock;
    RootNode->Private = SystemRoot;  /* SYSTEM_NODE* */
    RootNode->ReferenceCount = 1;

    if (SystemInstance)
    {
        SystemInstance->SuperBlock = SuperBlock;
    }

    return SuperBlock;
}

int
System_UnMount(SUPER_BLOCK* SuperBlock, SYSTEM_ERROR* Error)
{
    #define ErrorOut_System_UnMount(Code) \
        ErrorOut(Error, Code, FUNC_System_UnMount)

    if (Probe4Error(SuperBlock) || !SuperBlock)
    {
        ErrorOut_System_UnMount(-EINVAL);
        return Error->ErrorCode;
    }

    if (SystemInstance && SystemInstance->SuperBlock == SuperBlock)
    {
        SystemInstance->SuperBlock = 0;
    }

    if (SuperBlock->Root)
    {
        KFree(SuperBlock->Root, Error);
    }

    KFree(SuperBlock, Error);

    return GeneralOK;
}

int
System_RegisterFileSystem(SYSTEM_ERROR* Error)
{
    #define ErrorOut_System_RegisterFileSystem(Code) \
        ErrorOut(Error, Code, FUNC_System_RegisterFileSystem)

    SystemFileSystemType.Mount = System_Mount;

    int Result = VFS_RegisterFileSystem(&SystemFileSystemType, Error);
    if (Result != GeneralOK)
    {
        ErrorOut_System_RegisterFileSystem(Result);
        return Result;
    }

    return GeneralOK;
}

int
System_UnRegisterFileSystem(SYSTEM_ERROR* Error)
{
    #define ErrorOut_System_UnRegisterFileSystem(Code) \
        ErrorOut(Error, Code, FUNC_System_UnRegisterFileSystem)

    int Result = VFS_UnRegisterFileSystem(SystemFileSystemType.Name, Error);
    if (Result != GeneralOK)
    {
        ErrorOut_System_UnRegisterFileSystem(Result);
        return Result;
    }

    SystemFileSystemType.Mount = 0;
    return GeneralOK;
}

