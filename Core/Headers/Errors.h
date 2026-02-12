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
    FUNC_VfsInit,
    FUNC_VfsShutdown,
    FUNC_VfsRegisterFs,
    FUNC_VfsUnregisterFs,
    FUNC_VfsFindFs,
    FUNC_VfsListFs,
    FUNC_VfsMount,
    FUNC_VfsUnmount,
    FUNC_VfsSwitchRoot,
    FUNC_VfsBindMount,
    FUNC_VfsMoveMount,
    FUNC_VfsRemount,
    FUNC_VfsResolve,
    FUNC_VfsResolveAt,
    FUNC_VfsLookup,
    FUNC_VfsMkpath,
    FUNC_VfsRealpath,
    FUNC_VfsOpen,
    FUNC_VfsOpenAt,
    FUNC_VfsClose,
    FUNC_VfsRead,
    FUNC_VfsWrite,
    FUNC_VfsLseek,
    FUNC_VfsIoctl,
    FUNC_VfsFsync,
    FUNC_VfsFstats,
    FUNC_VfsStats,
    FUNC_VfsReaddir,
    FUNC_VfsReaddirF,
    FUNC_VfsCreate,
    FUNC_VfsUnlink,
    FUNC_VfsMkdir,
    FUNC_VfsRmdir,
    FUNC_VfsSymlink,
    FUNC_VfsReadlink,
    FUNC_VfsLink,
    FUNC_VfsRename,
    FUNC_VfsChmod,
    FUNC_VfsChown,
    FUNC_VfsTruncate,
    FUNC_VnodeRefInc,
    FUNC_VnodeRefDec,
    FUNC_VnodeGetAttr,
    FUNC_VnodeSetAttr,
    FUNC_DentryInvalidate,
    FUNC_DentryRevalidate,
    FUNC_DentryAttach,
    FUNC_DentryDetach,
    FUNC_DentryName,
    FUNC_VfsSetCwd,
    FUNC_VfsGetCwd,
    FUNC_VfsSetRoot,
    FUNC_VfsGetRoot,
    FUNC_VfsSetUmask,
    FUNC_VfsGetUmask,
    FUNC_VfsNotifySubscribe,
    FUNC_VfsNotifyUnsubscribe,
    FUNC_VfsNotifyPoll,
    FUNC_VfsAccess,
    FUNC_VfsExists,
    FUNC_VfsIsDir,
    FUNC_VfsIsFile,
    FUNC_VfsIsSymlink,
    FUNC_VfsCopy,
    FUNC_VfsMove,
    FUNC_VfsReadAll,
    FUNC_VfsWriteAll,
    FUNC_VfsMountTableEnumerate,
    FUNC_VfsMountTableFind,
    FUNC_VfsNodePath,
    FUNC_VfsNodeName,
    FUNC_VfsAllocName,
    FUNC_VfsFreeName,
    FUNC_VfsJoinPath,
    FUNC_VfsSetFlag,
    FUNC_VfsClearFlag,
    FUNC_VfsGetFlags,
    FUNC_VfsSyncAll,
    FUNC_VfsPruneCaches,
    FUNC_VfsRegisterDevNode,
    FUNC_VfsUnregisterDevNode,
    FUNC_VfsRegisterPseudoFs,
    FUNC_VfsUnregisterPseudoFs,
    FUNC_VfsSetDefaultFs,
    FUNC_VfsGetDefaultFs,
    FUNC_VfsSetMaxName,
    FUNC_VfsGetMaxName,
    FUNC_VfsSetMaxPath,
    FUNC_VfsGetMaxPath,
    FUNC_VfsSetDirCacheLimit,
    FUNC_VfsGetDirCacheLimit,
    FUNC_VfsSetFileCacheLimit,
    FUNC_VfsGetFileCacheLimit,
    FUNC_VfsSetIoBlockSize,
    FUNC_VfsGetIoBlockSize,

    /*System*/
    FUNC_SystemInit,
    FUNC_SystemShutdown,
    FUNC_SystemCreateNode,
    FUNC_SystemDeleteNode,
    FUNC_SystemFindNode,
    FUNC_SystemAttachNode,
    FUNC_SystemDetachNode,
    FUNC_SystemGetRoot,
    FUNC_SystemCreateFile,
    FUNC_SystemCreateDirectory,
    FUNC_SystemVfsOpen,
    FUNC_SystemVfsClose,
    FUNC_SystemVfsRead,
    FUNC_SystemVfsWrite,
    FUNC_SystemVfsReaddir,
    FUNC_SystemVfsLookup,
    FUNC_SystemVfsStat,
    FUNC_SystemGetattr,
    FUNC_SystemSetattr,
    FUNC_SystemStatFs,
    FUNC_SystemSync,
    FUNC_SystemRelease,
    FUNC_SystemSetContext,
    FUNC_SystemGetContext,
    FUNC_SystemGetContextSize,
    FUNC_SystemWalkPath,

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

    "Core/VirtualFileSystem/*->VfsInit",
    "Core/VirtualFileSystem/*->VfsShutdown",
    "Core/VirtualFileSystem/*->VfsRegisterFs",
    "Core/VirtualFileSystem/*->VfsUnregisterFs",
    "Core/VirtualFileSystem/*->VfsFindFs",
    "Core/VirtualFileSystem/*->VfsListFs",
    "Core/VirtualFileSystem/*->VfsMount",
    "Core/VirtualFileSystem/*->VfsUnmount",
    "Core/VirtualFileSystem/*->VfsSwitchRoot",
    "Core/VirtualFileSystem/*->VfsBindMount",
    "Core/VirtualFileSystem/*->VfsMoveMount",
    "Core/VirtualFileSystem/*->VfsRemount",
    "Core/VirtualFileSystem/*->VfsResolve",
    "Core/VirtualFileSystem/*->VfsResolveAt",
    "Core/VirtualFileSystem/*->VfsLookup",
    "Core/VirtualFileSystem/*->VfsMkpath",
    "Core/VirtualFileSystem/*->VfsRealpath",
    "Core/VirtualFileSystem/*->VfsOpen",
    "Core/VirtualFileSystem/*->VfsOpenAt",
    "Core/VirtualFileSystem/*->VfsClose",
    "Core/VirtualFileSystem/*->VfsRead",
    "Core/VirtualFileSystem/*->VfsWrite",
    "Core/VirtualFileSystem/*->VfsLseek",
    "Core/VirtualFileSystem/*->VfsIoctl",
    "Core/VirtualFileSystem/*->VfsFsync",
    "Core/VirtualFileSystem/*->VfsFstats",
    "Core/VirtualFileSystem/*->VfsStats",
    "Core/VirtualFileSystem/*->VfsReaddir",
    "Core/VirtualFileSystem/*->VfsReaddirF",
    "Core/VirtualFileSystem/*->VfsCreate",
    "Core/VirtualFileSystem/*->VfsUnlink",
    "Core/VirtualFileSystem/*->VfsMkdir",
    "Core/VirtualFileSystem/*->VfsRmdir",
    "Core/VirtualFileSystem/*->VfsSymlink",
    "Core/VirtualFileSystem/*->VfsReadlink",
    "Core/VirtualFileSystem/*->VfsLink",
    "Core/VirtualFileSystem/*->VfsRename",
    "Core/VirtualFileSystem/*->VfsChmod",
    "Core/VirtualFileSystem/*->VfsChown",
    "Core/VirtualFileSystem/*->VfsTruncate",
    "Core/VirtualFileSystem/*->VnodeRefInc",
    "Core/VirtualFileSystem/*->VnodeRefDec",
    "Core/VirtualFileSystem/*->VnodeGetAttr",
    "Core/VirtualFileSystem/*->VnodeSetAttr",
    "Core/VirtualFileSystem/*->DentryInvalidate",
    "Core/VirtualFileSystem/*->DentryRevalidate",
    "Core/VirtualFileSystem/*->DentryAttach",
    "Core/VirtualFileSystem/*->DentryDetach",
    "Core/VirtualFileSystem/*->DentryName",
    "Core/VirtualFileSystem/*->VfsSetCwd",
    "Core/VirtualFileSystem/*->VfsGetCwd",
    "Core/VirtualFileSystem/*->VfsSetRoot",
    "Core/VirtualFileSystem/*->VfsGetRoot",
    "Core/VirtualFileSystem/*->VfsSetUmask",
    "Core/VirtualFileSystem/*->VfsGetUmask",
    "Core/VirtualFileSystem/*->VfsNotifySubscribe",
    "Core/VirtualFileSystem/*->VfsNotifyUnsubscribe",
    "Core/VirtualFileSystem/*->VfsNotifyPoll",
    "Core/VirtualFileSystem/*->VfsAccess",
    "Core/VirtualFileSystem/*->VfsExists",
    "Core/VirtualFileSystem/*->VfsIsDir",
    "Core/VirtualFileSystem/*->VfsIsFile",
    "Core/VirtualFileSystem/*->VfsIsSymlink",
    "Core/VirtualFileSystem/*->VfsCopy",
    "Core/VirtualFileSystem/*->VfsMove",
    "Core/VirtualFileSystem/*->VfsReadAll",
    "Core/VirtualFileSystem/*->VfsWriteAll",
    "Core/VirtualFileSystem/*->VfsMountTableEnumerate",
    "Core/VirtualFileSystem/*->VfsMountTableFind",
    "Core/VirtualFileSystem/*->VfsNodePath",
    "Core/VirtualFileSystem/*->VfsNodeName",
    "Core/VirtualFileSystem/*->VfsAllocName",
    "Core/VirtualFileSystem/*->VfsFreeName",
    "Core/VirtualFileSystem/*->VfsJoinPath",
    "Core/VirtualFileSystem/*->VfsSetFlag",
    "Core/VirtualFileSystem/*->VfsClearFlag",
    "Core/VirtualFileSystem/*->VfsGetFlags",
    "Core/VirtualFileSystem/*->VfsSyncAll",
    "Core/VirtualFileSystem/*->VfsPruneCaches",
    "Core/VirtualFileSystem/*->VfsRegisterDevNode",
    "Core/VirtualFileSystem/*->VfsUnregisterDevNode",
    "Core/VirtualFileSystem/*->VfsRegisterPseudoFs",
    "Core/VirtualFileSystem/*->VfsUnregisterPseudoFs",
    "Core/VirtualFileSystem/*->VfsSetDefaultFs",
    "Core/VirtualFileSystem/*->VfsGetDefaultFs",
    "Core/VirtualFileSystem/*->VfsSetMaxName",
    "Core/VirtualFileSystem/*->VfsGetMaxName",
    "Core/VirtualFileSystem/*->VfsSetMaxPath",
    "Core/VirtualFileSystem/*->VfsGetMaxPath",
    "Core/VirtualFileSystem/*->VfsSetDirCacheLimit",
    "Core/VirtualFileSystem/*->VfsGetDirCacheLimit",
    "Core/VirtualFileSystem/*->VfsSetFileCacheLimit",
    "Core/VirtualFileSystem/*->VfsGetFileCacheLimit",
    "Core/VirtualFileSystem/*->VfsSetIoBlockSize",
    "Core/VirtualFileSystem/*->VfsGetIoBlockSize",

    "Core/System/*->SystemInit",
    "Core/System/*->SystemShutdown",
    "Core/System/*->SystemCreateNode",
    "Core/System/*->SystemDeleteNode",
    "Core/System/*->SystemFindNode",
    "Core/System/*->SystemAttachNode",
    "Core/System/*->SystemDetachNode",
    "Core/System/*->SystemGetRoot",
    "Core/System/*->SystemCreateFile",
    "Core/System/*->SystemCreateDirectory",
    "Core/System/*->SystemVfsOpen",
    "Core/System/*->SystemVfsClose",
    "Core/System/*->SystemVfsRead",
    "Core/System/*->SystemVfsWrite",
    "Core/System/*->SystemVfsReaddir",
    "Core/System/*->SystemVfsLookup",
    "Core/System/*->SystemVfsStat",
    "Core/System/*->SystemGetattr",
    "Core/System/*->SystemSetattr",
    "Core/System/*->SystemStatFs",
    "Core/System/*->SystemSync",
    "Core/System/*->SystemRelease",
    "Core/System/*->SystemSetContext",
    "Core/System/*->SystemGetContext",
    "Core/System/*->SystemGetContextSize",
    "Core/System/*->SystemWalkPath",

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