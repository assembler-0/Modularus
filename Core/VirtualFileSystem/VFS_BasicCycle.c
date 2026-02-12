#include <KernelCLibrary.h>
#include <VirtualFileSystem.h>
#include <__KCONF.h>
#include <DirtyHeap.h>
#include <BuiltIns/Heap/StaticPool.h>

/*extremely unix-like VFS*/

const long MaxFileSystemTypes = 32;
const long MaxMounts  = 64;
const FILESYSTEM_TYPE* FileSystemRegistry[32];
long FileSystemCount = 0;
MOUNT_ENTRY Mounts[64];
long MountsCount = 0;
VFS_NODE* RootNode = 0;
DIRECTORY_ENTRY* RootDirectoryEntry = 0;
long  UnMask = 0;
long  MaxName = 256;
long  MaxPath = 1024;
long  DirectoryCacheLimit = 0;
long  FileCacheLimit = 0;
long  IOBlockSize = 0;
char  DefaultFileSystem[64] = {0};

int
VfsInit(SYSTEM_ERROR* Error)
{
    FileSystemCount = 0;
    MountsCount = 0;
    RootNode = 0;
    RootDirectoryEntry = 0;
    UnMask = 0;
    MaxName = 256;
    MaxPath = 1024;
    DirectoryCacheLimit = 0;
    FileCacheLimit = 0;
    IOBlockSize = 0;
    DefaultFileSystem[0] = 0;

    /*KickStart the heap*/
    KickStartStaticPool(Error);

    return GeneralOK;
}

int
VfsShutdown(SYSTEM_ERROR* Error)
{
    for (long Iteration = 0; Iteration < MountsCount; Iteration++)
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

        Mounts[Iteration].SuperBlock = 0;
        Mounts[Iteration].Path[0] = 0;
    }

    MountsCount = 0;
    FileSystemCount = 0;
    RootNode = 0;
    RootDirectoryEntry = 0;

    return GeneralOK;
}
