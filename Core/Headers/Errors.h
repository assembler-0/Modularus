#pragma once

/*Error codes with trace backs*/

/*Global*/
typedef
enum
{
    GeneralOK,
    GeneralError,

    /*Main stuff*/
    BadRequest,
    BadParameters,
    NotFound,
    Limits,
    BadRange,
    BadType,

    /*VFS Related*/
    ENOENT,          /* No such file or directory */
    ENOTDIR,         /* Not a directory */
    EISDIR,          /* Is a directory */
    ENAMETOOLONG,    /* Filename too long */
    ELOOP,           /* Too many symbolic links */
    ENOTEMPTY,       /* Directory not empty */
    EACCES,          /* Permission denied */
    EPERM,           /* Operation not permitted */
    EROFS,           /* Read-only filesystem */
    ENOSPC,          /* No space left on device */
    ENOMEM,          /* Out of memory */
    EMFILE,          /* Too many open files */
    ENFILE,          /* File table overflow */
    EDQUOT,          /* Disk quota exceeded */
    EEXIST,          /* File exists */
    ENODEV,          /* No such device */
    EXDEV,           /* Cross-device link */
    EBUSY,           /* Device or resource busy */
    ETXTBSY,         /* Text file busy */
    EIO,             /* I/O error */
    EFBIG,           /* File too large */
    ENOSYS,          /* Function not implemented */
    EOPNOTSUPP,      /* Operation not supported */
    EINVAL,          /* Invalid argument */
    EBADF,           /* Bad file descriptor */
    ESPIPE,          /* Illegal seek */
    EFAULT,          /* Bad address */
    ENOTMNT,         /* Not mounted */
    EMNTFAILED,      /* Mount failed */
    ECORRUPTED,      /* Filesystem corrupted */
    EFSTYPE,         /* Wrong filesystem type */
    EMLINK,          /* Too many links */
    ENOLINK,         /* Link has been severed */
    EDEADLK,         /* Resource deadlock avoided */
    EINTR,           /* Interrupted system call */
    ESTALE,          /* Stale file handle */

    MaxErrors,
} ERROR_CODES;

/*Local*/
typedef
enum
{
    General,

    /*Main stuff*/
    FUNC_Limine_REQUEST,
    
    FUNC_LookUpKExport,

    FUNC_RegisterErrorKeys,

    /*VFS*/
    FUNC_VFS_KickStart,
    FUNC_VFS_PowerOff,
    FUNC_VFS_RegisterFileSystem,
    FUNC_VFS_UnRegisterFileSystem,
    FUNC_VFS_FindFileSystem,
    FUNC_VfsListFs,
    FUNC_VFS_Mount,
    FUNC_VFS_UnMount,
    FUNC_VFS_ChangeRoot,
    FUNC_VFS_BindMount,
    FUNC_VFS_MoveMount,
    FUNC_VFS_ReMount,
    FUNC_VFS_Resolve,
    FUNC_VFS_ResolveAt,
    FUNC_VFS_LookUp,
    FUNC_VFS_MakePath,
    FUNC_VFS_RealPath,
    FUNC_VFS_Open,
    FUNC_VFS_OpenAt,
    FUNC_VFS_Close,
    FUNC_VFS_Read,
    FUNC_VFS_Write,
    FUNC_VFS_LSeek,
    FUNC_VFS_IOControl,
    FUNC_VFS_FSync,
    FUNC_VFS_FStats,
    FUNC_VFS_Stats,
    FUNC_VFS_ReadDirectory,
    FUNC_VFS_ReadDirectoryF,
    FUNC_VFS_Create,
    FUNC_VFS_UnLink,
    FUNC_VFS_MakeDirectory,
    FUNC_VFS_RemoveDirectory,
    FUNC_VFS_SymbolLink,
    FUNC_VFS_ReadLink,
    FUNC_VFS_Link,
    FUNC_VFS_ReName,
    FUNC_VFS_ChangeMode,
    FUNC_VFS_ChangeOwner,
    FUNC_VFS_Truncate,
    FUNC_VFS_NodeReferenceIncrement,
    FUNC_VFS_NodeReferenceDecrement,
    FUNC_VFS_NodeGetAttribute,
    FUNC_VFS_NodeSetAttribute,
    FUNC_VFS_DentryInvalidate,
    FUNC_VFS_DentryRevalidate,
    FUNC_VFS_DentryAttach,
    FUNC_VFS_DentryDetach,
    FUNC_VFS_DentryName,
    FUNC_VFS_SetWorkingDirectory,
    FUNC_VFS_GetWorkingDirectory,
    FUNC_VFS_SetRoot,
    FUNC_VFS_GetRoot,
    FUNC_VFS_SetUnMask,
    FUNC_VFS_GetUnMask,
    FUNC_VFS_NotifySubscribe,
    FUNC_VFS_NotifyUnSubscribe,
    FUNC_VFS_NotifyPoll,
    FUNC_VFS_Access,
    FUNC_VFS_Exists,
    FUNC_VFS_IsDirectory,
    FUNC_VFS_IsFile,
    FUNC_VFS_IsSymbolLink,
    FUNC_VFS_Copy,
    FUNC_VFS_Move,
    FUNC_VFS_ReadAll,
    FUNC_VFS_WriteAll,
    FUNC_VFS_MountTableEnumerate,
    FUNC_VFS_MountTableFind,
    FUNC_VFS_NodePath,
    FUNC_VFS_NodeName,
    FUNC_VFS_AllocateName,
    FUNC_VFS_FreeName,
    FUNC_VFS_JoinPath,
    FUNC_VFS_SetFlag,
    FUNC_VFS_ClearFlag,
    FUNC_VFS_GetFlags,
    FUNC_VFS_SyncAll,
    FUNC_VFS_PruneCaches,
    FUNC_VFS_RegisterDeviceNode,
    FUNC_VFS_UnRegisterDeviceNode,
    FUNC_VFS_RegisterPseudoFileSystem,
    FUNC_VFS_UnRegisterPseudoFileSystem,
    FUNC_VFS_SetDefaultFileSystem,
    FUNC_VFS_GetDefaultFileSystem,
    FUNC_VFS_SetMaxName,
    FUNC_VFS_GetMaxName,
    FUNC_VFS_SetMaxPath,
    FUNC_VFS_GetMaxPath,
    FUNC_VFS_SetDirectoryCacheLimit,
    FUNC_VFS_GetDirectoryCacheLimit,
    FUNC_VFS_SetFileCacheLimit,
    FUNC_VFS_GetFileCacheLimit,
    FUNC_VFS_SetIOBlockSize,
    FUNC_VFS_GetIOBlockSize,
    FUNC_AllocateDirectoryEntry,
    FUNC_Walk,

    /*System*/
    FUNC_System_KickStart,
    FUNC_System_PowerOff,
    FUNC_System_CreateNode,
    FUNC_System_DeleteNode,
    FUNC_System_FindNode,
    FUNC_System_AttachNode,
    FUNC_System_DetachNode,
    FUNC_System_GetRoot,
    FUNC_System_CreateFile,
    FUNC_System_CreateDirectory,
    FUNC_System_Open,
    FUNC_System_Close,
    FUNC_System_Read,
    FUNC_System_Write,
    FUNC_System_ReadDirectory,
    FUNC_System_LookUp,
    FUNC_System_Stat,
    FUNC_System_GetAttribute,
    FUNC_System_SetAttribute,
    FUNC_System_StatFileSystem,
    FUNC_System_Sync,
    FUNC_System_Release,
    FUNC_System_SetContext,
    FUNC_System_GetContext,
    FUNC_System_GetContextSize,
    FUNC_System_Walk,
    FUNC_System_Mount,
    FUNC_System_UnMount,
    FUNC_System_RegisterFileSystem,
    FUNC_System_UnRegisterFileSystem,
    FUNC_System_Ioctl,

    MaxTraceback = 10000,
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
/*Global*/
static const char*
ErrorMap[MaxErrors]=
{
    "OK",
    "Ordinary/Unknown Error",
    "Bad Request",
    "Bad Parameters",
    "Not Found",
    "Bad Range",
    "Bad Type",

    "No such file or directory",
    "Not a directory",
    "Is a directory",
    "Filename too long",
    "Too many symbolic links",
    "Directory not empty",
    "Permission denied",
    "Operation not permitted",
    "Read-only filesystem",
    "No space left on device",
    "Out of memory",
    "Too many open files",
    "File table overflow",
    "Disk quota exceeded",
    "File exists",
    "No such device",
    "Cross-device link",
    "Device or resource busy",
    "Text file busy",
    "I/O error",
    "File too large",
    "Function not implemented",
    "Operation not supported",
    "Invalid argument",
    "Bad file descriptor",
    "Illegal seek",
    "Bad address",
    "Not mounted",
    "Mount failed",
    "Filesystem corrupted",
    "Wrong filesystem type",
    "Too many links",
    "Link has been severed",
    "Resource deadlock avoided",
    "Interrupted system call",
    "Stale file handle",
};

/*Core Local*/
static const char*
TraceBackMap[MaxTraceback]=
{
    "General",
    
    "Core/Headers/Boot/PreBoot.h->Limine_REQUEST",
    
    "Core/Main_KExports.c->LookUpKExport",
    
    "Core/Main_Errors.c->RegisterErrorContext",

    "Core/VirtualFileSystem/*->VFS_KickStart",
    "Core/VirtualFileSystem/*->VFS_PowerOff",
    "Core/VirtualFileSystem/*->VFS_RegisterFileSystem",
    "Core/VirtualFileSystem/*->VFS_UnRegisterFileSystem",
    "Core/VirtualFileSystem/*->VFS_FindFileSystem",
    "Core/VirtualFileSystem/*->VfsListFs",
    "Core/VirtualFileSystem/*->VFS_Mount",
    "Core/VirtualFileSystem/*->VFS_UnMount",
    "Core/VirtualFileSystem/*->VFS_ChangeRoot",
    "Core/VirtualFileSystem/*->VFS_BindMount",
    "Core/VirtualFileSystem/*->VFS_MoveMount",
    "Core/VirtualFileSystem/*->VFS_ReMount",
    "Core/VirtualFileSystem/*->VFS_Resolve",
    "Core/VirtualFileSystem/*->VFS_ResolveAt",
    "Core/VirtualFileSystem/*->VFS_LookUp",
    "Core/VirtualFileSystem/*->VFS_MakePath",
    "Core/VirtualFileSystem/*->VFS_RealPath",
    "Core/VirtualFileSystem/*->VFS_Open",
    "Core/VirtualFileSystem/*->VFS_OpenAt",
    "Core/VirtualFileSystem/*->VFS_Close",
    "Core/VirtualFileSystem/*->VFS_Read",
    "Core/VirtualFileSystem/*->VFS_Write",
    "Core/VirtualFileSystem/*->VFS_LSeek",
    "Core/VirtualFileSystem/*->VFS_IOControl",
    "Core/VirtualFileSystem/*->VFS_FSync",
    "Core/VirtualFileSystem/*->VFS_FStats",
    "Core/VirtualFileSystem/*->VFS_Stats",
    "Core/VirtualFileSystem/*->VFS_ReadDirectory",
    "Core/VirtualFileSystem/*->VFS_ReadDirectoryF",
    "Core/VirtualFileSystem/*->VFS_Create",
    "Core/VirtualFileSystem/*->VFS_UnLink",
    "Core/VirtualFileSystem/*->VFS_MakeDirectory",
    "Core/VirtualFileSystem/*->VFS_RemoveDirectory",
    "Core/VirtualFileSystem/*->VFS_SymbolLink",
    "Core/VirtualFileSystem/*->VFS_ReadLink",
    "Core/VirtualFileSystem/*->VFS_Link",
    "Core/VirtualFileSystem/*->VFS_ReName",
    "Core/VirtualFileSystem/*->VFS_ChangeMode",
    "Core/VirtualFileSystem/*->VFS_ChangeOwner",
    "Core/VirtualFileSystem/*->VFS_Truncate",
    "Core/VirtualFileSystem/*->VFS_NodeReferenceIncrement",
    "Core/VirtualFileSystem/*->VFS_NodeReferenceDecrement",
    "Core/VirtualFileSystem/*->VFS_NodeGetAttribute",
    "Core/VirtualFileSystem/*->VFS_NodeSetAttribute",
    "Core/VirtualFileSystem/*->VFS_DentryInvalidate",
    "Core/VirtualFileSystem/*->VFS_DentryRevalidate",
    "Core/VirtualFileSystem/*->VFS_DentryAttach",
    "Core/VirtualFileSystem/*->VFS_DentryDetach",
    "Core/VirtualFileSystem/*->VFS_DentryName",
    "Core/VirtualFileSystem/*->VFS_SetWorkingDirectory",
    "Core/VirtualFileSystem/*->VFS_GetWorkingDirectory",
    "Core/VirtualFileSystem/*->VFS_SetRoot",
    "Core/VirtualFileSystem/*->VFS_GetRoot",
    "Core/VirtualFileSystem/*->VFS_SetUnMask",
    "Core/VirtualFileSystem/*->VFS_GetUnMask",
    "Core/VirtualFileSystem/*->VFS_NotifySubscribe",
    "Core/VirtualFileSystem/*->VFS_NotifyUnSubscribe",
    "Core/VirtualFileSystem/*->VFS_NotifyPoll",
    "Core/VirtualFileSystem/*->VFS_Access",
    "Core/VirtualFileSystem/*->VFS_Exists",
    "Core/VirtualFileSystem/*->VFS_IsDirectory",
    "Core/VirtualFileSystem/*->VFS_IsFile",
    "Core/VirtualFileSystem/*->VFS_IsSymbolLink",
    "Core/VirtualFileSystem/*->VFS_Copy",
    "Core/VirtualFileSystem/*->VFS_Move",
    "Core/VirtualFileSystem/*->VFS_ReadAll",
    "Core/VirtualFileSystem/*->VFS_WriteAll",
    "Core/VirtualFileSystem/*->VFS_MountTableEnumerate",
    "Core/VirtualFileSystem/*->VFS_MountTableFind",
    "Core/VirtualFileSystem/*->VFS_NodePath",
    "Core/VirtualFileSystem/*->VFS_NodeName",
    "Core/VirtualFileSystem/*->VFS_AllocateName",
    "Core/VirtualFileSystem/*->VFS_FreeName",
    "Core/VirtualFileSystem/*->VFS_JoinPath",
    "Core/VirtualFileSystem/*->VFS_SetFlag",
    "Core/VirtualFileSystem/*->VFS_ClearFlag",
    "Core/VirtualFileSystem/*->VFS_GetFlags",
    "Core/VirtualFileSystem/*->VFS_SyncAll",
    "Core/VirtualFileSystem/*->VFS_PruneCaches",
    "Core/VirtualFileSystem/*->VFS_RegisterDeviceNode",
    "Core/VirtualFileSystem/*->VFS_UnRegisterDeviceNode",
    "Core/VirtualFileSystem/*->VFS_RegisterPseudoFileSystem",
    "Core/VirtualFileSystem/*->VFS_UnRegisterPseudoFileSystem",
    "Core/VirtualFileSystem/*->VFS_SetDefaultFileSystem",
    "Core/VirtualFileSystem/*->VFS_GetDefaultFileSystem",
    "Core/VirtualFileSystem/*->VFS_SetMaxName",
    "Core/VirtualFileSystem/*->VFS_GetMaxName",
    "Core/VirtualFileSystem/*->VFS_SetMaxPath",
    "Core/VirtualFileSystem/*->VFS_GetMaxPath",
    "Core/VirtualFileSystem/*->VFS_SetDirectoryCacheLimit",
    "Core/VirtualFileSystem/*->VFS_GetDirectoryCacheLimit",
    "Core/VirtualFileSystem/*->VFS_SetFileCacheLimit",
    "Core/VirtualFileSystem/*->VFS_GetFileCacheLimit",
    "Core/VirtualFileSystem/*->VFS_SetIOBlockSize",
    "Core/VirtualFileSystem/*->VFS_GetIOBlockSize",
    "Core/VirtualFileSystem/*->AllocateDirectoryEntry",
    "Core/VirtualFileSystem/*->Walk",

    "Core/System/*->System_KickStart",
    "Core/System/*->System_PowerOff",
    "Core/System/*->System_CreateNode",
    "Core/System/*->System_DeleteNode",
    "Core/System/*->System_FindNode",
    "Core/System/*->System_AttachNode",
    "Core/System/*->System_DetachNode",
    "Core/System/*->System_GetRoot",
    "Core/System/*->System_CreateFile",
    "Core/System/*->System_CreateDirectory",
    "Core/System/*->System_Open",
    "Core/System/*->System_Close",
    "Core/System/*->System_Read",
    "Core/System/*->System_Write",
    "Core/System/*->System_ReadDirectory",
    "Core/System/*->System_LookUp",
    "Core/System/*->System_Stat",
    "Core/System/*->System_GetAttribute",
    "Core/System/*->System_SetAttribute",
    "Core/System/*->System_StatFileSystem",
    "Core/System/*->System_Sync",
    "Core/System/*->System_Release",
    "Core/System/*->System_SetContext",
    "Core/System/*->System_GetContext",
    "Core/System/*->System_GetContextSize",
    "Core/System/*->System_Walk",
    "Core/System/*->System_Mount",
    "Core/System/*->System_UnMount",
    "Core/System/*->System_RegisterFileSystem",
    "Core/System/*->System_UnRegisterFileSystem",
    "Core/System/*->System_Ioctl",
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
CORE_TraceError(SYSTEM_ERROR* Error)
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

/*Per module*/
typedef struct ERROR_KEYS
{
    const char* ModuleName;
    int TracebackBase;
    int TracebackCount;

    /*Operations*/
    const char* (*TraceMapper)(int);

    struct ERROR_KEYS* Next;
} ERROR_KEYS;

typedef struct
{
    ERROR_KEYS* Head;
    ERROR_KEYS* Tail;

    int NextModuleBase;  /* Next available base ID */
} ERROR_KEYS_REGISTRY;

extern ERROR_KEYS_REGISTRY ErrorKeysRegistry;

int RegisterErrorKeys(const char*, const char* (*TraceMapper)(int), int, SYSTEM_ERROR*);
const char* TraceError(SYSTEM_ERROR*);