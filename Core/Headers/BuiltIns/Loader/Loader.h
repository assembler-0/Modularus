#pragma once

#include <__KCONF.h>
#include <Errors.h>

#include <System.h>
#include <Boot/PreBoot.h>

typedef struct LOADED_MODULE
{
    char Name[256];
    void* Address;
    uint64_t Size;
    struct LOADED_MODULE* Next; /*List*/
} LOADED_MODULE;

#define LoaderCommand_LIST 1

#define LoaderCommand_GET 2
typedef struct
{
    const char* Name;
    LOADED_MODULE* Out;
} LOADER_COMMAND_GET_ARGUMENTS;

#define LoaderCommand_COUNT 3

extern LOADED_MODULE* LoadedModules;
extern SYSTEM_OPERATIONS LoaderOperations;
extern SYSTEM_NODE* LoaderNode;

LOADED_MODULE* Loader_GetModules(SYSTEM_ERROR*);
LOADED_MODULE* Loader_FindModule(const char*, SYSTEM_ERROR*);
uint64_t Loader_GetModuleCount(SYSTEM_ERROR*);

/*Operations*/
int Loader_Open(SYSTEM_NODE*, SYSTEM_FILE*, SYSTEM_ERROR*);
int Loader_Close(SYSTEM_FILE*, SYSTEM_ERROR*);
long Loader_Read(SYSTEM_FILE*, void*, uint64_t, SYSTEM_ERROR*);
long Loader_Ioctl(SYSTEM_FILE*, uint64_t, void*, SYSTEM_ERROR*);
int ModuleLoader_GetAttribute(SYSTEM_NODE*, VFS_STAT*, SYSTEM_ERROR*);