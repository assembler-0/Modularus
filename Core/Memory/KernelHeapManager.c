#include <__KCONF.h>
#include <Plugs/Memory/KernelHeapManager.h>

HEAP_ALLOCATOR* ActiveHeap = NULL;

int RegisterHeap(HEAP_ALLOCATOR* Heap, SYSTEM_ERROR* Error)
{
    #define ErrorOut_RegisterHeap(Code) \
        ErrorOut(Error, Code, FUNC_RegisterHeap);

    if(Probe4Error(Heap)             || 
       !Heap                         || 
       Probe4Error(Heap->Operations) ||
       !Heap->Operations)
    {
        ErrorOut_RegisterHeap(-BadParameters);
        return -BadParameters;
    }
    
    ActiveHeap = Heap;
    
    if(Heap->Operations->Init)
    {
        return Heap->Operations->Init(Error);
    }
    
    return GeneralOK;
}

HEAP_ALLOCATOR* GetHeap(SYSTEM_ERROR* Error)
{
    #define ErrorOut_GetHeap(Code) \
        ErrorOut(Error, Code, FUNC_GetHeap);

    if(Probe4Error(ActiveHeap) || !ActiveHeap)
    {
        ErrorOut_GetHeap(-NotFound);
        return Error2Pointer(-NotFound);
    }
    
    return ActiveHeap;
}

void* KAllocate(uint64_t Size, SYSTEM_ERROR* Error)
{
    #define ErrorOut_KAllocate(Code) \
        ErrorOut(Error, Code, FUNC_KAllocate);

    if(Probe4Error(ActiveHeap) || !ActiveHeap || !ActiveHeap->Operations->Allocate)
    {
        ErrorOut_KAllocate(-NotFound);
        return Error2Pointer(-NotFound);
    }

    return ActiveHeap->Operations->Allocate(Size, Error);
}

void KFree(void* Pointer, SYSTEM_ERROR* Error)
{
    #define ErrorOut_KFree(Code) \
        ErrorOut(Error, Code, FUNC_KFree);

    if(Probe4Error(ActiveHeap) || !ActiveHeap || !ActiveHeap->Operations->Free)
    {
        ErrorOut_KFree(-NotFound);
        return;
    }

    ActiveHeap->Operations->Free(Pointer, Error);
}

int InitHeap(HEAP_ALLOCATOR* Heap, SYSTEM_ERROR* Error)
{
    #define ErrorOut_InitHeap(Code) \
        ErrorOut(Error, Code, FUNC_InitHeap);

    if(Probe4Error(Heap)             || 
       !Heap                         ||
       Probe4Error(Heap->Operations) ||
       !Heap->Operations)
    {
        ErrorOut_InitHeap(-BadParameters);
        return -BadParameters;
    }

    return Heap->Operations->Init(Error);
}

int ExitHeap(HEAP_ALLOCATOR* Heap, SYSTEM_ERROR* Error)
{
    #define ErrorOut_ExitHeap(Code) \
        ErrorOut(Error, Code, FUNC_ExitHeap);

    if(Probe4Error(Heap)             || 
       !Heap                         ||
       Probe4Error(Heap->Operations) ||
       !Heap->Operations)
    {
        ErrorOut_ExitHeap(-BadParameters);
        return -BadParameters;
    }

    Heap->Operations->Exit(Error);

    return GeneralOK;
}
