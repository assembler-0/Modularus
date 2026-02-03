#pragma once

#include <__KCONF.h>
#include <Errors.h>

typedef struct KEXPORT
{
    const char* Name;    /* Symbol name (string) */
    void*       Address; /* Symbol address */
} KEXPORT;

#define KEXPORT(Symbol)                        \
    __attribute__((used, section(".kexports")))\
    static const KEXPORT _kexp_##Symbol=       \
    {                                          \
        #Symbol, (void*)&Symbol                \
    };                                         \

extern const KEXPORT __start_kexports[];
extern const KEXPORT __stop_kexports[];

void* LookUpKExport(const char*, SYSTEM_ERROR*);