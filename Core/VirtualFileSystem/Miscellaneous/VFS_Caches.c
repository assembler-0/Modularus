#include <KernelCLibrary.h>
#include <VirtualFileSystem.h>
#include <__KCONF.h>
#include <DirtyHeap.h>

int
VFS_PruneCaches(SYSTEM_ERROR* Error __unused)
{
    return GeneralOK;
}

int
VFS_SetDirectoryCacheLimit(long Value, SYSTEM_ERROR* Error __unused)
{   
    DirectoryCacheLimit = Value;
    
    return GeneralOK;
}

long
VFS_GetDirectoryCacheLimit(SYSTEM_ERROR* Error __unused)
{
    return DirectoryCacheLimit;
}

int
VFS_SetFileCacheLimit(long Value, SYSTEM_ERROR* Error __unused)
{
    FileCacheLimit = Value;
    
    return GeneralOK;
}

long
VFS_GetFileCacheLimit(SYSTEM_ERROR* Error __unused)
{
    return FileCacheLimit;
}

int
VFS_SetIOBlockSize(long Value, SYSTEM_ERROR* Error __unused)
{
    IOBlockSize = Value;
    
    return GeneralOK;
}

long
VFS_GetIOBlockSize(SYSTEM_ERROR* Error __unused)
{
    return IOBlockSize;
}