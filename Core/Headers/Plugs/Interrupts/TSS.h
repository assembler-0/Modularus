#pragma once

#include <__KCONF.h>
#include <Modules/KExports.h>
#include <Errors.h>

typedef struct TSS_OPERATIONS
{
    int (*Init)(SYSTEM_ERROR*);
    void (*Exit)(SYSTEM_ERROR*);

    int (*SetStack)(uint8_t IST, uint64_t Stack, SYSTEM_ERROR*);
    uint64_t (*GetStack)(uint8_t IST, SYSTEM_ERROR*);
    int (*Load)(SYSTEM_ERROR*);
} TSS_OPERATIONS;

typedef struct
{
    TSS_OPERATIONS* Operations;
    uint64_t        RSP0;
    uint64_t        RSP1;
    uint64_t        RSP2;
    uint64_t        IST[MaxIST];
    void*           Private;
} TSS_MANAGER;

extern TSS_MANAGER* ActiveTSS;

int RegisterTSS(TSS_MANAGER*, SYSTEM_ERROR*);
TSS_MANAGER* GetTSS(SYSTEM_ERROR*);
int SetTSSStack(uint8_t IST, uint64_t Stack, SYSTEM_ERROR*);
uint64_t GetTSSStack(uint8_t IST, SYSTEM_ERROR*);
int LoadTSS(SYSTEM_ERROR*);
int InitTSS(TSS_MANAGER*, SYSTEM_ERROR*);
int ExitTSS(TSS_MANAGER*, SYSTEM_ERROR*);

KEXPORT(RegisterTSS)
KEXPORT(GetTSS)

KEXPORT(ActiveTSS)

KEXPORT(SetTSSStack)
KEXPORT(GetTSSStack)
KEXPORT(LoadTSS)
KEXPORT(InitTSS)
KEXPORT(ExitTSS)
