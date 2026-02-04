#pragma once

#include <__KCONF.h>
#include <Modules/KExports.h>
#include <Errors.h>
#include <Plugs/Interrupts/ISR.h>

typedef void (*IRQ_HANDLER)(ISR_CONTEXT*);

typedef struct IRQ_OPERATIONS
{
    int (*Init)(SYSTEM_ERROR*);
    void (*Exit)(SYSTEM_ERROR*);
    
    int (*RegisterHandler)(uint8_t, IRQ_HANDLER, SYSTEM_ERROR*);
    int (*UnregisterHandler)(uint8_t, SYSTEM_ERROR*);
    int (*Enable)(uint8_t, SYSTEM_ERROR*);
    int (*Disable)(uint8_t, SYSTEM_ERROR*);
    int (*SendEOI)(uint8_t, SYSTEM_ERROR*);
} IRQ_OPERATIONS;

typedef struct IRQ_MANAGER
{
    IRQ_OPERATIONS* Operations;
    IRQ_HANDLER     Handlers[MaxIRQHandlers];
    bool            Enabled[MaxIRQHandlers];
    void*           Private;
} IRQ_MANAGER;

extern IRQ_MANAGER* ActiveIRQ;

int RegisterIRQ(IRQ_MANAGER*, SYSTEM_ERROR*);
IRQ_MANAGER* GetIRQ(SYSTEM_ERROR*);
int RegisterIRQHandler(uint8_t, IRQ_HANDLER, SYSTEM_ERROR*);
int UnregisterIRQHandler(uint8_t, SYSTEM_ERROR*);
int EnableIRQ(uint8_t, SYSTEM_ERROR*);
int DisableIRQ(uint8_t, SYSTEM_ERROR*);
int SendEOI(uint8_t, SYSTEM_ERROR*);
int InitIRQ(IRQ_MANAGER*, SYSTEM_ERROR*);
int ExitIRQ(IRQ_MANAGER*, SYSTEM_ERROR*);

KEXPORT(RegisterIRQ)
KEXPORT(GetIRQ)

KEXPORT(ActiveIRQ)

KEXPORT(RegisterIRQHandler)
KEXPORT(UnregisterIRQHandler)
KEXPORT(EnableIRQ)
KEXPORT(DisableIRQ)
KEXPORT(SendEOI)
KEXPORT(InitIRQ)
KEXPORT(ExitIRQ)
