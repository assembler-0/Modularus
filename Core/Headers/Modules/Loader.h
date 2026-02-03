#pragma once

#include <__KCONF.h>
#include <Modules/KExports.h>
#include <Errors.h>

typedef struct
{
    const char*  Path;
    void*        Address;
    uint64_t     Size;
    void*        Private;
} LOADED_FILE;

typedef struct LOADER_OPERATIONS
{
    LOADED_FILE* (*Load)(const char*, SYSTEM_ERROR*);
    int (*Unload)(LOADED_FILE*, SYSTEM_ERROR*);
} LOADER_OPERATIONS;

typedef struct
{
    LOADER_OPERATIONS* Operations;
    void*              Private;
} LOADER;

extern LOADER* ActiveLoader;

int RegisterLoader(LOADER*, SYSTEM_ERROR*);
LOADER* GetLoader(SYSTEM_ERROR*);

LOADED_FILE* LoadFile(const char*, SYSTEM_ERROR*);
int UnLoadFile(LOADED_FILE*, SYSTEM_ERROR*);

KEXPORT(RegisterLoader)
KEXPORT(GetLoader)

KEXPORT(ActiveLoader)

KEXPORT(LoadFile)
KEXPORT(UnLoadFile)