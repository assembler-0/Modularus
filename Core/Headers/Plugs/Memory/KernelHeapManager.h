#pragma once

#include <__KCONF.h>
#include <Modules/KExports.h>
#include <Errors.h>

typedef struct HEAP_OPERATIONS
{
    int (*Init)(SYSTEM_ERROR*);
    void (*Exit)(SYSTEM_ERROR*);
    void* (*Allocate)(uint64_t, SYSTEM_ERROR*);
    void (*Free)(void*, SYSTEM_ERROR*);
} HEAP_OPERATIONS;

typedef struct
{
    uint64_t TotalAllocated;
    uint64_t TotalFreed;
    uint64_t CurrentUsage;
    uint64_t PeakUsage;
    uint64_t AllocationCount;
    uint64_t FreeCount;
} HEAP_STATS;

typedef struct
{
    HEAP_OPERATIONS* Operations;
    HEAP_STATS       Stats;
    void*            Private;
} HEAP_ALLOCATOR;

extern HEAP_ALLOCATOR* ActiveHeap;

int RegisterHeap(HEAP_ALLOCATOR*, SYSTEM_ERROR*);
HEAP_ALLOCATOR* GetHeap(SYSTEM_ERROR*);

/*Ops wrap*/
void* KAllocate(uint64_t, SYSTEM_ERROR*);
void KFree(void*, SYSTEM_ERROR*);
int InitHeap(HEAP_ALLOCATOR*, SYSTEM_ERROR*);
int ExitHeap(HEAP_ALLOCATOR*, SYSTEM_ERROR*);

KEXPORT(RegisterHeap)
KEXPORT(GetHeap)

KEXPORT(ActiveHeap)

KEXPORT(KAllocate)
KEXPORT(KFree)
KEXPORT(InitHeap)
KEXPORT(ExitHeap)
