#pragma once

#include <__KCONF.h>
#include <Modules/KExports.h>
#include <Errors.h>

typedef enum IDT_GATE_TYPE
{
    IDTGate_Interrupt,
    IDTGate_Trap,
    IDTGate_Task,
    IDTGate_Call,
} IDT_GATE_TYPE;

typedef struct IDT_DESCRIPTOR
{
    uint16_t Limit;
    uint64_t Base;
} __attribute__((packed)) IDT_DESCRIPTOR;

typedef struct IDT_ENTRY
{
    void*           Handler;
    IDT_GATE_TYPE   Type;
    uint8_t         DPL;
    bool            Present;
} IDT_ENTRY;

typedef struct IDT_OPERATIONS
{
    int (*Init)(SYSTEM_ERROR*);
    void (*Exit)(SYSTEM_ERROR*);
    
    int (*SetEntry)(uint8_t, IDT_ENTRY*, SYSTEM_ERROR*);
    int (*GetEntry)(uint8_t, IDT_ENTRY*, SYSTEM_ERROR*);
    int (*Load)(SYSTEM_ERROR*);
} IDT_OPERATIONS;

typedef struct
{
    IDT_OPERATIONS* Operations;
    IDT_ENTRY       Entries[MaxIDT];
    void*           Private;
} IDT_MANAGER;

extern IDT_MANAGER* ActiveIDT;

int RegisterIDT(IDT_MANAGER*, SYSTEM_ERROR*);
IDT_MANAGER* GetIDT(SYSTEM_ERROR*);
int SetIDTEntry(uint8_t, IDT_ENTRY*, SYSTEM_ERROR*);
int GetIDTEntry(uint8_t, IDT_ENTRY*, SYSTEM_ERROR*);
int LoadIDT(SYSTEM_ERROR*);
int InitIDT(IDT_MANAGER*, SYSTEM_ERROR*);
int ExitIDT(IDT_MANAGER*, SYSTEM_ERROR*);

KEXPORT(RegisterIDT)
KEXPORT(GetIDT)

KEXPORT(ActiveIDT)

KEXPORT(SetIDTEntry)
KEXPORT(GetIDTEntry)
KEXPORT(LoadIDT)
KEXPORT(InitIDT)
KEXPORT(ExitIDT)
