#pragma once

#include <__KCONF.h>
#include <Modules/KExports.h>
#include <Errors.h>

typedef enum
{
    PMMRegion_USABLE,
    PMMRegion_RESERVED,
    PMMRegion_ACPI_RECLAIMABLE,
    PMMRegion_ACPI_NVS,
    PMMRegion_BAD,
    PMMRegion_BOOTLOADER_RECLAIMABLE,
    PMMRegion_KERNEL_AND_MODULES,
    PMMRegion_FRAMEBUFFER,
    PMMRegion_PERSISTENT,
    PMMRegion_DEFECTIVE,
    PMMRegion_MMIO,
    PMMRegion_MMIO_PORT_SPACE,
    PMMRegion_PAL_CODE,
    PMMRegion_UNACCEPTED,
    PMMRegion_FIRMWARE_RUNTIME,
    PMMRegion_FIRMWARE_BOOT,
    PMMRegion_LOADER_CODE,
    PMMRegion_LOADER_DATA,
    PMMRegion_CONVENTIONAL,
    PMMRegion_UNUSABLE,
    PMMRegion_DISABLED,
    PMMRegion_HIBERNATION,
    PMMRegion_RAMDISK,
    PMMRegion_INITRD,
    PMMRegion_DEVICE,
    PMMRegion_HOLE,
    PMMRegion_UNKNOWN
} PMM_REGION_TYPE;

typedef struct
{
    uint64_t         Base;
    uint64_t         Length;
    PMM_REGION_TYPE  Type;
} PMM_REGION;

typedef struct
{
    uint64_t TotalPages;
    uint64_t UsedPages;
    uint64_t FreePages;
    uint64_t ReservedPages;
} PMM_STATS;

typedef struct PMM_OPERATIONS
{
    int (*Init)(SYSTEM_ERROR*);
    void (*Exit)(SYSTEM_ERROR*);
    
    uint64_t (*AllocatePages)(uint64_t, SYSTEM_ERROR*);
    void (*FreePages)(uint64_t, uint64_t, SYSTEM_ERROR*);
} PMM_OPERATIONS;

typedef struct
{
    PMM_OPERATIONS* Operations;
    PMM_STATS       Stats;
    PMM_REGION      Regions[PMM_MaxRegions];
    uint64_t        RegionCount;
    uint64_t        Offset;
    void*           Private;
} PMM_MANAGER;

extern PMM_MANAGER* ActivePMM;

int RegisterPMM(PMM_MANAGER*, SYSTEM_ERROR*);
PMM_MANAGER* GetPMM(SYSTEM_ERROR*);

/*Ops wrap*/
uint64_t AllocatePages(uint64_t, SYSTEM_ERROR*);
void FreePages(uint64_t, uint64_t, SYSTEM_ERROR*);
int InitPMM(PMM_MANAGER*, SYSTEM_ERROR*);
int ExitPMM(PMM_MANAGER*, SYSTEM_ERROR*);

/*Helpers*/
#define PMM_PhysicalToVirtual(PMM, Physical) \
    ((void*)((Physical) + (PMM)->HHDMOffset))

#define PMM_VirtualToPhysical(PMM, Virtual) \
    ((uint64_t)((uintptr_t)(Virtual) - (PMM)->HHDMOffset))

KEXPORT(RegisterPMM)
KEXPORT(GetPMM)

KEXPORT(ActivePMM)

KEXPORT(AllocatePages)
KEXPORT(FreePages)
KEXPORT(InitPMM)
KEXPORT(ExitPMM)
