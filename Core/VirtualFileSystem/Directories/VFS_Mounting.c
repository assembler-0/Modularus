#include <KernelCLibrary.h>
#include <VirtualFileSystem.h>
#include <__KCONF.h>
#include <DirtyHeap.h>

SUPER_BLOCK*
VFS_Mount(const char* Device, const char* Path, const char* Type, long Flags __unused, const char* Options, SYSTEM_ERROR* Error)
{
    #define ErrorOut_VFS_Mount(Code) \
        ErrorOut(Error, Code, FUNC_VFS_Mount)

    const FILESYSTEM_TYPE* FileSystem = VFS_FindFileSystem(Type, Error);
    if (Probe4Error(FileSystem) || !FileSystem)
    {
        ErrorOut_VFS_Mount(-ENOENT);
        return Error2Pointer(-ENOENT);
    }

    if (Probe4Error(Path) || !Path || !*Path)
    {
        ErrorOut_VFS_Mount(-ENOENT);
        return Error2Pointer(-ENOENT);
    }

    long PathLength = (long)strlen(Path);
    if (PathLength <= 0 || PathLength >= MaxPath)
    {
        ErrorOut_VFS_Mount(-Limits);
        return Error2Pointer(-Limits);
    }

    if (MountsCount >= MaxMounts)
    {
        ErrorOut_VFS_Mount(-Limits);
        return Error2Pointer(-Limits);
    }

    SUPER_BLOCK* SuperBlock = FileSystem->Mount(Device, Options, Error);
    if (Probe4Error(SuperBlock) || !SuperBlock || Probe4Error(SuperBlock->Root) || !SuperBlock->Root)
    {
        ErrorOut_VFS_Mount(-ENOENT);
        return Error2Pointer(-ENOENT);
    }

    MOUNT_ENTRY* MountEntry = &Mounts[MountsCount++];
    MountEntry->SuperBlock = SuperBlock;
    memcpy(MountEntry->Path, Path, (size_t)(PathLength + 1));

    if (!RootNode && strcmp(Path, "/") == 0)
    {
        RootNode = SuperBlock->Root;
        RootDirectoryEntry = AllocateDirectoryEntry("/", 0, RootNode, Error);
    }

    return SuperBlock;
}

int
VFS_UnMount(const char* Path, SYSTEM_ERROR* Error)
{
    #define ErrorOut_VFS_UnMount(Code) \
        ErrorOut(Error, Code, FUNC_VFS_UnMount)
        
    if (Probe4Error(Path) || !Path)
    {
        ErrorOut_VFS_UnMount(-ENOENT);
        return Error->ErrorCode;
    }

    for (long Iteration = 0; Iteration < MountsCount; Iteration++)
    {
        if (strcmp(Mounts[Iteration].Path, Path) == 0)
        {
            SUPER_BLOCK* SuperBlock = Mounts[Iteration].SuperBlock;
            if (SuperBlock && SuperBlock->Operations && SuperBlock->Operations->Umount)
            {
                SuperBlock->Operations->Umount(SuperBlock, Error);
            }

            if (SuperBlock && SuperBlock->Operations && SuperBlock->Operations->Release)
            {
                SuperBlock->Operations->Release(SuperBlock, Error);
            }

            for (long Index = Iteration; Index < MountsCount - 1; Index++)
            {
                Mounts[Index] = Mounts[Index + 1];
            }

            Mounts[--MountsCount].SuperBlock = 0;
            Mounts[MountsCount].Path[0] = 0;

            if (strcmp(Path, "/") == 0)
            {
                RootNode = 0;
                RootDirectoryEntry = 0;
            }

            return GeneralOK;
        }
    }

    return Error->ErrorCode;
}

int /*chroot???*/
VFS_ChangeRoot(const char* NewRoot, SYSTEM_ERROR* Error)
{
    #define ErrorOut_VFS_ChangeRoot(Code) \
        ErrorOut(Error, Code, FUNC_VFS_ChangeRoot)

    if (Probe4Error(NewRoot) || !NewRoot)
    {
        ErrorOut_VFS_ChangeRoot(-ENOENT);
        return Error->ErrorCode;
    }

    DIRECTORY_ENTRY* DirectoryEntry = VFS_Resolve(NewRoot, Error);
    if (Probe4Error(DirectoryEntry) || !DirectoryEntry || Probe4Error(DirectoryEntry->Node) || !DirectoryEntry->Node)
    {
        ErrorOut_VFS_ChangeRoot(-ENOENT);
        return Error->ErrorCode;
    }

    RootNode = DirectoryEntry->Node;
    RootDirectoryEntry = DirectoryEntry;
    
    return GeneralOK;
}

int
VFS_BindMount(const char* Source, const char* Destination, SYSTEM_ERROR* Error)
{
    #define ErrorOut_VFS_BindMount(Code) \
        ErrorOut(Error, Code, FUNC_VFS_BindMount)

    if (Probe4Error(Source) || !Source || Probe4Error(Destination) || !Destination)
    {
        ErrorOut_VFS_BindMount(-EINVAL);
        return Error->ErrorCode;
    }

    MOUNT_ENTRY* MountEntry = FindMount(Source, Error);
    if (Probe4Error(MountEntry) || !MountEntry || Probe4Error(MountEntry->SuperBlock) || !MountEntry->SuperBlock)
    {
        ErrorOut_VFS_BindMount(-ENOENT);
        return Error->ErrorCode;
    }

    if (MountsCount >= MaxMounts)
    {
        ErrorOut_VFS_BindMount(-Limits);
        return Error->ErrorCode;
    }

    long Index = (long)strlen(Destination);
    if (Index <= 0 || Index >= MaxPath)
    {
        ErrorOut_VFS_BindMount(-Limits);
        return Error->ErrorCode;
    }

    MOUNT_ENTRY* NewMountEntry = &Mounts[MountsCount++];
    NewMountEntry->SuperBlock = MountEntry->SuperBlock;
    memcpy(NewMountEntry->Path, Destination, (size_t)(Index + 1));

    return GeneralOK;
}

int
VFS_MoveMount(const char* Source, const char* Destination, SYSTEM_ERROR* Error)
{
    #define ErrorOut_VFS_MoveMount(Code) \
        ErrorOut(Error, Code, FUNC_VFS_MoveMount)

    if (Probe4Error(Source) || !Source || Probe4Error(Destination) || !Destination)
    {
        ErrorOut_VFS_MoveMount(-EINVAL);
        return Error->ErrorCode;
    }

    MOUNT_ENTRY* MountEntry = FindMount(Source, Error);
    if (Probe4Error(MountEntry) || !MountEntry || Probe4Error(MountEntry->SuperBlock) || !MountEntry->SuperBlock)
    {
        ErrorOut_VFS_MoveMount(-ENOENT);
        return Error->ErrorCode;
    }

    long Index = (long)strlen(Destination);
    if (Index <= 0 || Index >= MaxPath)
    {
        ErrorOut_VFS_MoveMount(-Limits);
        return Error->ErrorCode;
    }

    memcpy(MountEntry->Path, Destination, (size_t)(Index + 1));
 
    return GeneralOK;
}

int
VFS_ReMount(const char* Path, long Flags __unused, const char* Options __unused, SYSTEM_ERROR* Error)
{
    #define ErrorOut_VFS_ReMount(Code) \
        ErrorOut(Error, Code, FUNC_VFS_ReMount)

    MOUNT_ENTRY* MountEntry = FindMount(Path, Error);
    if (Probe4Error(MountEntry) || !MountEntry || Probe4Error(MountEntry->SuperBlock) || !MountEntry->SuperBlock)
    {
        ErrorOut_VFS_ReMount(-ENOENT);
        return Error->ErrorCode;
    }

    return GeneralOK;
}