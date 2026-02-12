#include <KernelCLibrary.h>
#include <VirtualFileSystem.h>
#include <__KCONF.h>
#include <DirtyHeap.h>

SUPER_BLOCK*
VfsMount(const char* Device, const char* Path, const char* Type, long Flags __UNUSED, const char* Options, SYSTEM_ERROR* Error)
{
    #define ErrorOut_VfsMount(Code) \
        ErrorOut(Error, Code, FUNC_VfsMount)

    const FILESYSTEM_TYPE* FileSystem = VfsFindFs(Type, Error);
    if (Probe4Error(FileSystem) || !FileSystem)
    {
        ErrorOut_VfsMount(-ENOENT);
        return Error2Pointer(-ENOENT);
    }

    if (Probe4Error(Path) || !Path || !*Path)
    {
        ErrorOut_VfsMount(-ENOENT);
        return Error2Pointer(-ENOENT);
    }

    long PathLength = (long)strlen(Path);
    if (PathLength <= 0 || PathLength >= MaxPath)
    {
        ErrorOut_VfsMount(-Limits);
        return Error2Pointer(-Limits);
    }

    if (MountsCount >= MaxMounts)
    {
        ErrorOut_VfsMount(-Limits);
        return Error2Pointer(-Limits);
    }

    SUPER_BLOCK* SuperBlock = FileSystem->Mount(Device, Options, Error);
    if (Probe4Error(SuperBlock) || !SuperBlock || Probe4Error(SuperBlock->Root) || !SuperBlock->Root)
    {
        ErrorOut_VfsMount(-ENOENT);
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
VfsUnmount(const char* Path, SYSTEM_ERROR* Error)
{
    #define ErrorOut_VfsUnmount(Code) \
        ErrorOut(Error, Code, FUNC_VfsUnmount)
        
    if (Probe4Error(Path) || !Path)
    {
        ErrorOut_VfsUnmount(-ENOENT);
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
VfsSwitchRoot(const char* NewRoot, SYSTEM_ERROR* Error)
{
    #define ErrorOut_VfsSwitchRoot(Code) \
        ErrorOut(Error, Code, FUNC_VfsSwitchRoot)

    if (Probe4Error(NewRoot) || !NewRoot)
    {
        ErrorOut_VfsSwitchRoot(-ENOENT);
        return Error->ErrorCode;
    }

    DIRECTORY_ENTRY* DirectoryEntry = VfsResolve(NewRoot, Error);
    if (Probe4Error(DirectoryEntry) || !DirectoryEntry || Probe4Error(DirectoryEntry->Node) || !DirectoryEntry->Node)
    {
        ErrorOut_VfsSwitchRoot(-ENOENT);
        return Error->ErrorCode;
    }

    RootNode = DirectoryEntry->Node;
    RootDirectoryEntry = DirectoryEntry;
    
    return GeneralOK;
}

int
VfsBindMount(const char* Source, const char* Destination, SYSTEM_ERROR* Error)
{
    #define ErrorOut_VfsBindMount(Code) \
        ErrorOut(Error, Code, FUNC_VfsBindMount)

    if (Probe4Error(Source) || !Source || Probe4Error(Destination) || !Destination)
    {
        ErrorOut_VfsBindMount(-EINVAL);
        return Error->ErrorCode;
    }

    MOUNT_ENTRY* MountEntry = FindMount(Source, Error);
    if (Probe4Error(MountEntry) || !MountEntry || Probe4Error(MountEntry->SuperBlock) || !MountEntry->SuperBlock)
    {
        ErrorOut_VfsBindMount(-ENOENT);
        return Error->ErrorCode;
    }

    if (MountsCount >= MaxMounts)
    {
        ErrorOut_VfsBindMount(-Limits);
        return Error->ErrorCode;
    }

    long Index = (long)strlen(Destination);
    if (Index <= 0 || Index >= MaxPath)
    {
        ErrorOut_VfsBindMount(-Limits);
        return Error->ErrorCode;
    }

    MOUNT_ENTRY* NewMountEntry = &Mounts[MountsCount++];
    NewMountEntry->SuperBlock = MountEntry->SuperBlock;
    memcpy(NewMountEntry->Path, Destination, (size_t)(Index + 1));

    return GeneralOK;
}

int
VfsMoveMount(const char* Source, const char* Destination, SYSTEM_ERROR* Error)
{
    #define ErrorOut_VfsMoveMount(Code) \
        ErrorOut(Error, Code, FUNC_VfsMoveMount)

    if (Probe4Error(Source) || !Source || Probe4Error(Destination) || !Destination)
    {
        ErrorOut_VfsMoveMount(-EINVAL);
        return Error->ErrorCode;
    }

    MOUNT_ENTRY* MountEntry = FindMount(Source, Error);
    if (Probe4Error(MountEntry) || !MountEntry || Probe4Error(MountEntry->SuperBlock) || !MountEntry->SuperBlock)
    {
        ErrorOut_VfsMoveMount(-ENOENT);
        return Error->ErrorCode;
    }

    long Index = (long)strlen(Destination);
    if (Index <= 0 || Index >= MaxPath)
    {
        ErrorOut_VfsMoveMount(-Limits);
        return Error->ErrorCode;
    }

    memcpy(MountEntry->Path, Destination, (size_t)(Index + 1));
 
    return GeneralOK;
}

int
VfsRemount(const char* Path, long Flags __UNUSED, const char* Options __UNUSED, SYSTEM_ERROR* Error)
{
    #define ErrorOut_VfsRemount(Code) \
        ErrorOut(Error, Code, FUNC_VfsRemount)

    MOUNT_ENTRY* MountEntry = FindMount(Path, Error);
    if (Probe4Error(MountEntry) || !MountEntry || Probe4Error(MountEntry->SuperBlock) || !MountEntry->SuperBlock)
    {
        ErrorOut_VfsRemount(-ENOENT);
        return Error->ErrorCode;
    }

    return GeneralOK;
}