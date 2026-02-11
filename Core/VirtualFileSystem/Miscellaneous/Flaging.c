#include <KernelCLibrary.h>
#include <VirtualFileSystem.h>
#include <__KCONF.h>
#include <DirtyHeap.h>

int
VfsSetFlag(const char* Path __UNUSED, long __Flag__ __UNUSED, SYSTEM_ERROR* Error __UNUSED)
{
    return GeneralOK;
}

int
VfsClearFlag(const char* Path __UNUSED, long __Flag__ __UNUSED, SYSTEM_ERROR* Error __UNUSED)
{
    return GeneralOK;
}

long
VfsGetFlags(const char* Path __UNUSED, SYSTEM_ERROR* Error __UNUSED)
{
    return GeneralOK;
}
