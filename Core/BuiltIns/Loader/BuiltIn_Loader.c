#include <__KCONF.h>
#include <Errors.h>
#include <BuiltIns/Loader/Loader.h>
#include <DirtyHeap.h>
#include <KernelCLibrary.h>
#include <System.h>

LOADED_MODULE* LoadedModules = NULL;
SYSTEM_NODE* LoaderNode = NULL;

/*Operations*/

SYSTEM_OPERATIONS LoaderOperations =
{
    .Open = Loader_Open,
    .Close = Loader_Close,
    .Read = Loader_Read,
    .Write = NULL,
    .Ioctl = Loader_Ioctl,
    .Getattr = ModuleLoader_GetAttribute,
    .Setattr = NULL
};

int Loader_Open(SYSTEM_NODE* Node __unused, SYSTEM_FILE* File __unused, SYSTEM_ERROR* Error __unused)
{
    return GeneralOK;
}

int Loader_Close(SYSTEM_FILE* File __unused, SYSTEM_ERROR* Error __unused)
{
    return GeneralOK;
}

long Loader_Read(SYSTEM_FILE* File __unused, void* Buffer, uint64_t Size, SYSTEM_ERROR* Error __unused)
{
    char List[4096];
    memset(List, 0, sizeof(List));
    uint64_t Offset = 0;
    
    if (Size > Offset)
    {
        Size = Offset;
    }

    memcpy(Buffer, List, Size);
    return Size;
}

long Loader_Ioctl(SYSTEM_FILE* File __unused, uint64_t Request, void* Arguments, SYSTEM_ERROR* Error)
{
    #define ErrorOut_Loader_Ioctl(Code) \
        ErrorOut(Error, Code, General)

    switch (Request)
    {
        case LoaderCommand_COUNT:
        {
            uint64_t* Count = (uint64_t*)Arguments;
            if (Probe4Error(Count) || !Count)
            {
                ErrorOut_Loader_Ioctl(-EINVAL);
                return Error->ErrorCode;
            }
            
            *Count = 0;
            for (LOADED_MODULE* Module = LoadedModules; Module; Module = Module->Next)
            {
                (*Count)++;
            }
            
            return GeneralOK;
        }
        
        case LoaderCommand_GET:
        {
            LOADER_COMMAND_GET_ARGUMENTS* GetCommandArguments = (LOADER_COMMAND_GET_ARGUMENTS*)Arguments;
            if (Probe4Error(GetCommandArguments) || !GetCommandArguments || Probe4Error(GetCommandArguments->Name) || !GetCommandArguments->Name || Probe4Error(GetCommandArguments->Out) || !GetCommandArguments->Out)
            {
                ErrorOut_Loader_Ioctl(-EINVAL);
                return Error->ErrorCode;
            }
        
            LOADED_MODULE* Module = Loader_FindModule(GetCommandArguments->Name, Error);
            if (Probe4Error(Module) || !Module)
            {
                return Error->ErrorCode;
            }

            /*Copy*/
            strncpy(GetCommandArguments->Out->Name, Module->Name, sizeof(GetCommandArguments->Out->Name) - 1);
            GetCommandArguments->Out->Address = Module->Address;
            GetCommandArguments->Out->Size = Module->Size;
            GetCommandArguments->Out->Next = NULL;
        
            return GeneralOK;
        }
        
        default:
        {
            ErrorOut_Loader_Ioctl(-BadRequest);
            return Error->ErrorCode;
        }
    }
}

int ModuleLoader_GetAttribute(SYSTEM_NODE* Node __unused, VFS_STAT* Stat, SYSTEM_ERROR* Error __unused)
{
    Stat->Size = 4096;
    return GeneralOK;
}

LOADED_MODULE*
Loader_GetModules(SYSTEM_ERROR* Error)
{
    #define ErrorOut_Loader_GetModules(Code) \
        ErrorOut(Error, Code, General)

    /* Request MODULE from boot services, simplest of all loaders it can be */
    BOOT_REQUEST Request = RequestBootService(RequestMODULE, Error);
    if (Probe4Error(Request.Pointer) || !Request.Pointer)
    {
        ErrorOut_Loader_GetModules(-BadRequest);
        return Error2Pointer(Error->ErrorCode);
    }

    /*LIMINE*/

    #ifdef BOOTVENDOR_Limine
        struct limine_module_request* LimineRequest = (struct limine_module_request*)Request.Pointer;
        struct limine_module_response* Response = LimineRequest->response;
        if (!Response || !Response->modules)
        {
            ErrorOut_Loader_GetModules(-BadRequest);
            return Error2Pointer(Error->ErrorCode);
        }

        if (Response->module_count > 1024 || Response->module_count == 0)
        {
            ErrorOut_Loader_GetModules(-BadRange);
            return Error2Pointer(Error->ErrorCode);
        }

        LOADED_MODULE* First = NULL;
        LOADED_MODULE* Last = NULL;

        for (uint64_t Index = 0; Index < Response->module_count; Index++)
        {
            struct limine_file* File = Response->modules[Index];

            LOADED_MODULE* Module = KMalloc(sizeof(LOADED_MODULE), Error);
            if (Probe4Error(Module) || !Module)
            {
                continue;
            }

            const char* Name = File->path;
            const char* Basename = Name;
            for (const char* Component = Name; *Component; Component++)
            {
                if (*Component == '/')
                {
                    Basename = Component + 1;
                }
            }

            strncpy(Module->Name, Basename, sizeof(Module->Name) - 1);
            Module->Name[sizeof(Module->Name) - 1] = 0;
            Module->Address = File->address;
            Module->Size = File->size;
            Module->Next = NULL;

            if (Probe4Error(First) || !First)
            {
                First = Module;
            }

            if (Last) 
            {
                Last->Next = Module;
            }

            Last = Module;
        }

        LoadedModules = First;
        
        if (LoadedModules && !LoaderNode)
        {
            LoaderNode = System_CreateNode("loader", SystemNodeTypeEnumeration_FILE, &LoaderOperations, NULL, 4096, Error);
            if (!Probe4Error(LoaderNode) && LoaderNode)
            {
                System_AttachNode(SystemRoot, LoaderNode, Error);
            }
        }

        return First;
    #endif
}

LOADED_MODULE*
Loader_FindModule(const char* Name, SYSTEM_ERROR* Error)
{
    #define ErrorOut_Loader_FindModule(Code) \
        ErrorOut(Error, Code, General)

    for (LOADED_MODULE* Modules = LoadedModules; Modules; Modules = Modules->Next)
    {
        if (strcmp(Modules->Name, Name) == 0)
        {
            return Modules;
        }
    }

    ErrorOut_Loader_FindModule(-NotFound);
    return Error2Pointer(Error->ErrorCode);
}

uint64_t
Loader_GetModuleCount(SYSTEM_ERROR* Error __unused)
{
    uint64_t Count = 0;
    for (LOADED_MODULE* Modules = LoadedModules; Modules; Modules = Modules->Next)
    {
        Count++;
    }

    return Count; /*Not Found(SYSTEM_ERROR enum btw) if 0*/
}
