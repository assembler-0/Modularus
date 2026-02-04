#pragma once

#include <__KCONF.h>
#include <Modules/KExports.h>
#include <Errors.h>

typedef enum GDT_ACCESS_FLAGS
{
    GDTAccess_PRESENT      = (1 << 7),
    GDTAccess_RING0        = (0 << 5),
    GDTAccess_RING1        = (1 << 5),
    GDTAccess_RING2        = (2 << 5),
    GDTAccess_RING3        = (3 << 5),
    GDTAccess_SYSTEM       = (1 << 4),
    GDTAccess_EXECUTABLE   = (1 << 3),
    GDTAccess_DC           = (1 << 2),
    GDTAccess_RW           = (1 << 1),
    GDTAccess_ACCESSED     = (1 << 0)
} GDT_ACCESS_FLAGS;

typedef enum GDT_FLAGS
{
    GDTFlag_GRANULARITY    = (1 << 3),
    GDTFlag_SIZE           = (1 << 2),
    GDTFlag_LONG_MODE      = (1 << 1)
} GDT_FLAGS;

typedef struct GDT_DESCRIPTOR
{
    uint16_t Limit;
    uint64_t Base;
} __attribute__((packed)) GDT_DESCRIPTOR;

typedef struct GDT_ENTRY
{
    uint32_t Base;
    uint32_t Limit;
    uint8_t  Access;
    uint8_t  Flags;
} GDT_ENTRY;

typedef struct GDT_OPERATIONS
{
    int (*Init)(SYSTEM_ERROR*);
    void (*Exit)(SYSTEM_ERROR*);

    int (*SetEntry)(uint16_t, GDT_ENTRY*, SYSTEM_ERROR*);
    int (*GetEntry)(uint16_t, GDT_ENTRY*, SYSTEM_ERROR*);
    int (*Load)(SYSTEM_ERROR*);
} GDT_OPERATIONS;

typedef struct GDT_MANAGER
{
    GDT_OPERATIONS* Operations;
    GDT_ENTRY       Entries[MaxGDT];
    uint16_t        EntryCount;
    void*           Private;
} GDT_MANAGER;

extern GDT_MANAGER* ActiveGDT;

int RegisterGDT(GDT_MANAGER*, SYSTEM_ERROR*);
GDT_MANAGER* GetGDT(SYSTEM_ERROR*);
int SetGDTEntry(uint16_t, GDT_ENTRY*, SYSTEM_ERROR*);
int GetGDTEntry(uint16_t, GDT_ENTRY*, SYSTEM_ERROR*);
int LoadGDT(SYSTEM_ERROR*);
int InitGDT(GDT_MANAGER*, SYSTEM_ERROR*);
int ExitGDT(GDT_MANAGER*, SYSTEM_ERROR*);

KEXPORT(RegisterGDT)
KEXPORT(GetGDT)

KEXPORT(ActiveGDT)

KEXPORT(SetGDTEntry)
KEXPORT(GetGDTEntry)
KEXPORT(LoadGDT)
KEXPORT(InitGDT)
KEXPORT(ExitGDT)
