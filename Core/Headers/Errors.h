#pragma once

/*Error codes with trace backs*/

typedef
enum
{
    GeneralOK,
    GeneralError,

    /*Main stuff*/
    BadRequest,
    BadParameters,
    NotFound,

    MaxErrors,
} ERROR_CODES;

typedef
enum
{
    General,

    /*Main stuff*/
    FUNC_Limine_REQUEST,
    
    FUNC_LookUpKExport,

    FUNC_RegisterFrameBuffer,
    FUNC_GetFrameBuffer,
    FUNC_InitFrameBuffer,
    FUNC_ExitFrameBuffer,

    FUNC_RegisterLoader,
    FUNC_GetLoader,
    FUNC_LoadFile,
    FUNC_UnLoadFile,

    FUNC_RegisterPMM,
    FUNC_GetPMM,
    FUNC_AllocatePages,
    FUNC_FreePages,
    FUNC_InitPMM,
    FUNC_ExitPMM,

    FUNC_RegisterVMM,
    FUNC_GetVMM,
    FUNC_MapPage,
    FUNC_UnMapPage,
    FUNC_InitVMM,
    FUNC_ExitVMM,
    FUNC_GetPhysicalAddress,
    FUNC_SetPageFlags,
    FUNC_GetPageFlags,
    FUNC_GetPageTable,
    FUNC_SwitchPageTable,   
    
    FUNC_RegisterHeap,
    FUNC_GetHeap,
    FUNC_KAllocate,
    FUNC_KFree,
    FUNC_InitHeap,
    FUNC_ExitHeap,

    MaxTraceback,
} TRACEBACK_ID; /*Can also be called function IDs*/

/*Core Error stuff*/
typedef
struct
{
    int ErrorCode;
    int TraceBack;
} SYSTEM_ERROR;

/*Main reporter*/
#define ErrorOut(ErrorSlot, Code, Trace)      \
    do                                        \
    {                                         \
        if ((ErrorSlot) != NULL)              \
        {                                     \
            (ErrorSlot)->ErrorCode = (Code);  \
            (ErrorSlot)->TraceBack = (Trace); \
        }                                     \
    } while (0)

/*For Pointers ofc*/
#define Error2Pointer(Code)((void*)(intptr_t)(Code))
#define Pointer2Error(Pointer)((int)(intptr_t)(Pointer))
#define Probe4Error(Pointer)((intptr_t)(Pointer) < 0 && (intptr_t)(Pointer) >= -MaxErrors)

/*String maps for the error code enum*/
static const char*
ErrorMap[MaxErrors]=
{
    "OK",
    "Ordinary/Unknown Error",
    "Bad Request",
    "Bad Parameters",
    "Not Found",

};

static const char*
TraceBackMap[MaxTraceback]=
{
    "General",
    
    "Core/Headers/PreBoot.h->Limine_REQUEST",
    
    "Core/Modules/KExports.c->LookUpKExport",
    
    "Core/IO/FrameBuffer.c->RegisterFrameBuffer",
    "Core/IO/FrameBuffer.c->GetFrameBuffer",
    "Core/IO/FrameBuffer.c->InitFrameBuffer",
    "Core/IO/FrameBuffer.c->ExitFrameBuffer",
    
    "Core/Modules/Loader.c->RegisterLoader",
    "Core/Modules/Loader.c->GetLoader",
    "Core/Modules/Loader.c->LoadFile",
    "Core/Modules/Loader.c->UnLoadFile",
    
    "Core/Memory/PhysicalMemoryManager.c->RegisterPMM",
    "Core/Memory/PhysicalMemoryManager.c->GetPMM",
    "Core/Memory/PhysicalMemoryManager.c->AllocatePages",
    "Core/Memory/PhysicalMemoryManager.c->FreePages",
    "Core/Memory/PhysicalMemoryManager.c->InitPMM",
    "Core/Memory/PhysicalMemoryManager.c->ExitPMM",

    "Core/Memory/VirtualMemoryManager.c->RegisterVMM",
    "Core/Memory/VirtualMemoryManager.c->GetVMM",
    "Core/Memory/VirtualMemoryManager.c->MapPage",
    "Core/Memory/VirtualMemoryManager.c->UnMapPage",
    "Core/Memory/VirtualMemoryManager.c->InitVMM",
    "Core/Memory/VirtualMemoryManager.c->ExitVMM",
    "Core/Memory/VirtualMemoryManager.c->GetPhysicalAddress",
    "Core/Memory/VirtualMemoryManager.c->SetPageFlags",
    "Core/Memory/VirtualMemoryManager.c->GetPageFlags",
    "Core/Memory/VirtualMemoryManager.c->GetPageTable",
    "Core/Memory/VirtualMemoryManager.c->SwitchPageTable",

    "Core/Memory/KernelHeapManager.c->RegisterHeap",
    "Core/Memory/KernelHeapManager.c->GetHeap",
    "Core/Memory/KernelHeapManager.c->KAllocate",
    "Core/Memory/KernelHeapManager.c->KFree",
    "Core/Memory/KernelHeapManager.c->InitHeap",
    "Core/Memory/KernelHeapManager.c->ExitHeap",
};

/*Another helper but to map the enum code to a error string map*/
static inline const char*
MapError(SYSTEM_ERROR* Error)
{
    if (Error->ErrorCode < 0)
    {
        Error->ErrorCode = -Error->ErrorCode;
    }
    if (Error->ErrorCode >= MaxErrors)
    {
        return "Bad Error";
    }
    return ErrorMap[Error->ErrorCode];
}

/*Another helper but to map the traceback code to the location of the error from map*/
static inline const char*
TraceError(SYSTEM_ERROR* Error)
{
    if (Error->ErrorCode < 0)
    {
        Error->ErrorCode = -Error->ErrorCode;
    }
    if (Error->ErrorCode >= MaxErrors)
    {
        return "Bad Error";
    }
    if (Error->TraceBack >= MaxTraceback)
    {
        return "Bad TraceBack";
    }
    return TraceBackMap[Error->TraceBack];
}


