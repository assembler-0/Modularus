#include <__KCONF.h>
#include <BuiltIns/Heap/StaticPool.h>
#include <DirtyHeap.h>

uint8_t Pool[64 * 1024 * 1024];  /* 64 MB pool */
uint8_t* Pointer = Pool;
uint8_t* Ending = Pool + sizeof(Pool);

static DIRTY_HEAP StaticHeap;

void KickStartStaticPool(SYSTEM_ERROR* Error __unused)
{
    static DIRTY_HEAP_OPERATIONS StaticOperations=
    {
        .Alloc = StaticKMalloc,
        .Free  = StaticKFree
    };

    StaticHeap.Operations = &StaticOperations;
    StaticHeap.Name = "StaticPool";
    DirtyHeap = &StaticHeap;
}

void* StaticKMalloc(uint64_t Size, SYSTEM_ERROR* Error)
{
    Size = (Size + 7) & ~7;
    
    if(Pointer + Size > Ending)
    {
        ErrorOut(Error, -ENOMEM, General);
        return Error2Pointer(-ENOMEM);
    }

    uint8_t* Allocation = Pointer;
    Pointer += Size;

    return Allocation;
}

void StaticKFree(void* Pointer __unused, SYSTEM_ERROR* Error __unused)
{
    /* No-op */
}