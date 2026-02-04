#pragma once

#include <__KCONF.h>
#include <Modules/KExports.h>
#include <Errors.h>

typedef struct ISR_CONTEXT
{
    uint64_t R15, R14, R13, R12, R11, R10, R9, R8;
    uint64_t RBP, RDI, RSI, RDX, RCX, RBX, RAX;
    uint64_t IntNo, ErrorCode;
    uint64_t RIP, CS, RFLAGS, RSP, SS;
} __attribute__((packed)) ISR_CONTEXT;

typedef void (*ISR_HANDLER)(ISR_CONTEXT*);

typedef struct ISR_OPERATIONS
{
    int (*Init)(SYSTEM_ERROR*);
    void (*Exit)(SYSTEM_ERROR*);
    
    int (*RegisterHandler)(uint8_t, ISR_HANDLER, SYSTEM_ERROR*);
    int (*UnregisterHandler)(uint8_t, SYSTEM_ERROR*);
} ISR_OPERATIONS;

typedef struct ISR_MANAGER
{
    ISR_OPERATIONS* Operations;
    ISR_HANDLER     Handlers[256];
    void*           Private;
} ISR_MANAGER;

extern ISR_MANAGER* ActiveISR;

int RegisterISR(ISR_MANAGER*, SYSTEM_ERROR*);
ISR_MANAGER* GetISR(SYSTEM_ERROR*);
int RegisterISRHandler(uint8_t Vector, ISR_HANDLER, SYSTEM_ERROR*);
int UnregisterISRHandler(uint8_t Vector, SYSTEM_ERROR*);
int InitISR(ISR_MANAGER*, SYSTEM_ERROR*);
int ExitISR(ISR_MANAGER*, SYSTEM_ERROR*);

KEXPORT(RegisterISR)
KEXPORT(GetISR)

KEXPORT(ActiveISR)

KEXPORT(RegisterISRHandler)
KEXPORT(UnregisterISRHandler)
KEXPORT(InitISR)
KEXPORT(ExitISR)
