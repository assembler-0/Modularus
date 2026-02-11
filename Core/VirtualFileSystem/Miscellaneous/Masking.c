#include <KernelCLibrary.h>
#include <VirtualFileSystem.h>
#include <__KCONF.h>
#include <DirtyHeap.h>

int
VfsSetUmask(long Mode, SYSTEM_ERROR* Error __UNUSED)
{
    UnMask = Mode;
    
    return GeneralOK;
}

long
VfsGetUmask(SYSTEM_ERROR* Error __UNUSED)
{
    return UnMask;
}