#include <__KCONF.h>
#include <Errors.h>
#include <DirtyHeap.h>

DIRTY_HEAP* DirtyHeap = 0;

void* KMalloc(uint64_t Size, SYSTEM_ERROR* Error)
{
    return DirtyHeap->Operations->Alloc(Size, Error);
}

void KFree(void* Pointer, SYSTEM_ERROR* Error)
{
    DirtyHeap->Operations->Free(Pointer, Error);
}