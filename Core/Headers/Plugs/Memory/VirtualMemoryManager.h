#pragma once

#include <__KCONF.h>
#include <Modules/KExports.h>
#include <Errors.h>

#define VMMFLAG_PRESENT      (1ULL << 0)
#define VMMFLAG_WRITABLE     (1ULL << 1)
#define VMMFLAG_USER         (1ULL << 2)
#define VMMFLAG_WRITETHROUGH (1ULL << 3)
#define VMMFLAG_CACHEDISABLE (1ULL << 4)
#define VMMFLAG_ACCESSED     (1ULL << 5)
#define VMMFLAG_DIRTY        (1ULL << 6)
#define VMMFLAG_HUGEPAGE     (1ULL << 7)
#define VMMFLAG_GLOBAL       (1ULL << 8)
#define VMMFLAG_COPYONWRITE  (1ULL << 9)
#define VMMFLAG_SHARED       (1ULL << 10)
#define VMMFLAG_LOCKED       (1ULL << 11)
#define VMMFLAG_UNCACHEABLE  (1ULL << 12)
#define VMMFLAG_WRITECOMBINE (1ULL << 13)
#define VMMFLAG_WRITEPROTECT (1ULL << 14)
#define VMMFLAG_EXECUTABLE   (1ULL << 15)
#define VMMFLAG_DEVICE       (1ULL << 16)
#define VMMFLAG_MMIO         (1ULL << 17)
#define VMMFLAG_DMA          (1ULL << 18)
#define VMMFLAG_PERSISTENT   (1ULL << 19)
#define VMMFLAG_SUPERVISOR   (1ULL << 20)
#define VMMFLAG_PRIVILEGED   (1ULL << 21)
#define VMMFLAG_SECURE       (1ULL << 22)
#define VMMFLAG_ENCRYPTED    (1ULL << 23)
#define VMMFLAG_SHAREABLE    (1ULL << 24)
#define VMMFLAG_INNERCACHE   (1ULL << 25)
#define VMMFLAG_OUTERCACHE   (1ULL << 26)
#define VMMFLAG_ATOMICOPS    (1ULL << 27)
#define VMMFLAG_PREFETCH     (1ULL << 28)
#define VMMFLAG_SPECULATIVE  (1ULL << 29)
#define VMMFLAG_GUARDED      (1ULL << 30)
#define VMMFLAG_PAT          (1ULL << 31)
#define VMMFLAG_NOEXECUTE    (1ULL << 63)

typedef enum
{
    VMM_PageSize_4KiB    = 0x1000,        /* 4 KiB - Standard */
    VMM_PageSize_8KiB    = 0x2000,        /* 8 KiB - Some ARM */
    VMM_PageSize_16KiB   = 0x4000,        /* 16 KiB - ARM64, PowerPC */
    VMM_PageSize_64KiB   = 0x10000,       /* 64 KiB - ARM64, IA-64 */
    VMM_PageSize_2MiB    = 0x200000,      /* 2 MiB - x86_64 huge */
    VMM_PageSize_4MiB    = 0x400000,      /* 4 MiB - x86 PSE */
    VMM_PageSize_16MiB   = 0x1000000,     /* 16 MiB - ARM */
    VMM_PageSize_32MiB   = 0x2000000,     /* 32 MiB - SPARC */
    VMM_PageSize_1GiB    = 0x40000000,    /* 1 GiB - x86_64 huge */
    VMM_PageSize_2GiB    = 0x80000000,    /* 2 GiB - Some systems */
    VMM_PageSize_16GiB   = 0x400000000,   /* 16 GiB - POWER */
    VMM_PageSize_512GiB  = 0x8000000000,  /* 512 GiB - Future */
} VMM_PAGESIZE;

typedef struct VMM_OPERATIONS
{
    int (*Init)(SYSTEM_ERROR*);
    void (*Exit)(SYSTEM_ERROR*);
    
    int (*MapPage)(uint64_t, uint64_t, uint64_t, SYSTEM_ERROR*);
    int (*UnmapPage)(uint64_t, SYSTEM_ERROR*);
    uint64_t (*GetPhysical)(uint64_t, SYSTEM_ERROR*);
    int (*SetFlags)(uint64_t, uint64_t, SYSTEM_ERROR*);
    uint64_t (*GetFlags)(uint64_t, SYSTEM_ERROR*);
    void* (*GetPageTable)(SYSTEM_ERROR*);
    int (*SwitchPageTable)(void*, SYSTEM_ERROR*);
} VMM_OPERATIONS;

typedef struct
{
    uint64_t TotalMapped;
    uint64_t KernelMapped;
    uint64_t UserMapped;
} VMM_STATS;

typedef struct
{
    VMM_OPERATIONS* Operations;
    VMM_STATS       Stats;
    VMM_PAGESIZE    PageSize;
    void*           PageTable;
    void*           Private;
} VMM_MANAGER;

extern VMM_MANAGER* ActiveVMM;

int RegisterVMM(VMM_MANAGER*, SYSTEM_ERROR*);
VMM_MANAGER* GetVMM(SYSTEM_ERROR*);

/*Ops wrap*/
int MapPage(uint64_t, uint64_t, uint64_t, SYSTEM_ERROR*);
int UnMapPage(uint64_t, SYSTEM_ERROR*);
uint64_t GetPhysicalAddress(uint64_t, SYSTEM_ERROR*);
int SetPageFlags(uint64_t, uint64_t, SYSTEM_ERROR*);
uint64_t GetPageFlags(uint64_t, SYSTEM_ERROR*);
void* GetPageTable(SYSTEM_ERROR*);
int SwitchPageTable(void*, SYSTEM_ERROR*);
int InitVMM(VMM_MANAGER*, SYSTEM_ERROR*);
int ExitVMM(VMM_MANAGER*, SYSTEM_ERROR*);

KEXPORT(RegisterVMM)
KEXPORT(GetVMM)

KEXPORT(ActiveVMM)

KEXPORT(MapPage)
KEXPORT(UnMapPage)
KEXPORT(GetPhysicalAddress)
KEXPORT(SetPageFlags)
KEXPORT(GetPageFlags)
KEXPORT(GetPageTable)
KEXPORT(SwitchPageTable)
KEXPORT(InitVMM)
KEXPORT(ExitVMM)