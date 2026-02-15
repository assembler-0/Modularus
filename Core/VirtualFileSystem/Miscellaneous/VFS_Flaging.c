#include <KernelCLibrary.h>
#include <VirtualFileSystem.h>
#include <__KCONF.h>
#include <DirtyHeap.h>

int
VFS_SetFlag(const char* Path __unused, long __Flag__ __unused, SYSTEM_ERROR* Error __unused)
{
    return GeneralOK;
}

int
VFS_ClearFlag(const char* Path __unused, long __Flag__ __unused, SYSTEM_ERROR* Error __unused)
{
    return GeneralOK;
}

long
VFS_GetFlags(const char* Path __unused, SYSTEM_ERROR* Error __unused)
{
    return GeneralOK;
}
