#pragma once

/*totally the entire VFS is not stolen from https://github.com/VOXIDEVOSTRO/AxeialOS*/

#include <Types.h>
#include <Errors.h>
#include <KExports.h>

typedef struct VFS_NODE             VFS_NODE;
typedef struct DIRECTORY_ENTRY      DIRECTORY_ENTRY;
typedef struct SUPER_BLOCK          SUPER_BLOCK;
typedef struct FILE                 FILE;
typedef struct FILESYSTEM_TYPE      FILESYSTEM_TYPE;
typedef struct VFS_NODE_OPERATIONS  VFS_NODE_OPERATIONS;
typedef struct SUPER_OPERATIONS     SUPER_OPERATIONS;
typedef struct VFS_DIRECTORY_ENTRY  VFS_DIRECTORY_ENTRY;
typedef struct VFS_STAT             VFS_STAT;
typedef struct VFS_STAT_FILESYSTEM  VFS_STAT_FILESYSTEM;
typedef struct VFS_PERMISSIONS      VFS_PERMISSIONS;
typedef struct VFS_TIME             VFS_TIME;
typedef struct VFS_MOUNT_FLAGS      VFS_MOUNT_FLAGS;
typedef struct VFS_NAME_BUFFER      VFS_NAME_BUFFER;

/*Node Type*/
typedef enum VFS_NODE_TYPE
{
    VFSNode_NONE,
    VFSNode_FILE,
    VFSNode_DIRECTORY,
    VFSNode_DEVICE,
    VFSNode_SYMBOL,
    VFSNode_FIFO,
    VFSNode_SOCKET
} VFS_NODE_TYPE;

/*Open Flags*/
typedef enum VFS_OPEN_FLAGS
{
    VfsOpenFlag_NONE      = 0,
    VfsOpenFlag_READONLY = 1 << 0,
    VfsOpenFlag_WRITEONLY= 1 << 1,
    VfsOpenFlag_READWRITE= 1 << 2,
    VfsOpenFlag_CREATE   = 1 << 3,
    VfsOpenFlag_TRUNCATE = 1 << 4,
    VfsOpenFlag_APPEND   = 1 << 5,
    VfsOpenFlag_EXCLUDE  = 1 << 6,
    VfsOpenFlag_SYNC     = 1 << 7,
    VfsOpenFlag_DIRECT   = 1 << 8
} VFS_OPEN_FLAGS;

/*Seek Wheel*/
typedef enum VFS_SEEK_WHENCE
{
    VFSSeek_SET,
    VFSSeek_CURRENT,
    VFSSeek_END
} VFS_SEEK_WHENCE;

/*Permissions*/
typedef enum VFS_PERMISSION_MODE
{
    VFSMode_NONE = 0,

    /* User */
    VFSMode_RUSR = 1 << 8,
    VFSMode_WUSR = 1 << 7,
    VFSMode_XUSR = 1 << 6,

    /* Group */
    VFSMode_RGRP = 1 << 5,
    VFSMode_WGRP = 1 << 4,
    VFSMode_XGRP = 1 << 3,

    /* Other */
    VFSMode_ROTH = 1 << 2,
    VFSMode_WOTH = 1 << 1,
    VFSMode_XOTH = 1 << 0
} VFS_PERMISSION_MODE;

/*IO*/
typedef enum VFS_IO_FLAGS
{
    VFSIO_NONE    = 0,
    VFSIO_FUA     = 1 << 0,
    VFSIO_SYNC    = 1 << 1,
    VFSIO_NOCACHE = 1 << 2
} VFS_IO_FLAGS;

/*Mount*/
typedef enum VFS_MOUNT_OPTION
{
    VFSMount_NONE      = 0,
    VFSMount_READONLY  = 1 << 0,
    VFSMount_NOEXECUTE = 1 << 1,
    VFSMount_NODEVICE  = 1 << 2,
    VFSMount_NOSUID    = 1 << 3,
    VFSMount_SYNC      = 1 << 4,
    VFSMount_NOATIME   = 1 << 5
} VFS_MOUNT_OPTION;

/*Name*/
typedef enum VFS_RENAME_FLAGS
{
    VFSRename_NONE      = 0,
    VFSRename_NOREPLACE = 1 << 0,
    VFSRename_EXCHANGE  = 1 << 1,
    VFSRename_WHITEOUT  = 1 << 2
} VFS_RENAME_FLAGS;

/*Notify*/
typedef enum VFS_NOTIFY_MASK
{
    VFSNotify_NONE      = 0,
    VFSNotify_CREATE    = 1 << 0,
    VFSNotify_DELETE    = 1 << 1,
    VFSNotify_WRITE     = 1 << 2,
    VFSNotify_MOVE      = 1 << 3,
    VFSNotify_ATTRIBUTE = 1 << 4
} VFS_NOTIFY_MASK;

typedef struct VFS_TIME
{
    long Second;
    long NanoSecond;
} VFS_TIME;

typedef struct VFS_PERMISSIONS
{
    /*Mains*/
    long Mode;

    /*Ids*/
    long UserID;
    long GroupID;
} VFS_PERMISSIONS;

typedef struct VFS_STAT
{
    /*Mains*/
    long Inode;
    long Size;
    long Blocks;
    long BlockSize;
    long NumberOfLinks;
    long RawDevice;
    long Device;
    long Flags;

    /*Attr*/
    VFS_NODE_TYPE   Type;
    VFS_PERMISSIONS Permission;

    /*Time*/
    VFS_TIME Atime;
    VFS_TIME Mtime;
    VFS_TIME Ctime;
} VFS_STAT;

typedef struct VFS_STAT_FILESYSTEM
{
    long TypeID;
    long BlockSize;
    long Blocks;
    long BlocksFree;
    long BlocksAvailable;
    long Files;
    long FilesFree;
    long NameLength;
    long Flags;
} VFS_STAT_FILESYSTEM;

typedef struct VFS_DIRECTORY_ENTRY
{
    char Name[256];
    long Type;
    long Inode;
} VFS_DIRECTORY_ENTRY; /*Dirent*/

typedef struct VFS_NAME_BUFFER
{
    char* Buffer;
    long  Length;
} VFS_NAME_BUFFER;

typedef struct VFS_NODE_OPERATIONS
{
    int (*Open)(VFS_NODE*, FILE*, SYSTEM_ERROR*);
    int (*Close)(FILE*, SYSTEM_ERROR*);
    long (*Read)(FILE*, void*, long, SYSTEM_ERROR*);
    long (*Write)(FILE*, const void*, long, SYSTEM_ERROR*);
    long (*Lseek)(FILE*, long, int, SYSTEM_ERROR*);
    int (*Ioctl)(FILE*, unsigned long, void*, SYSTEM_ERROR*);
    int (*Stat)(VFS_NODE*, VFS_STAT*, SYSTEM_ERROR*);
    long (*Readdir)(VFS_NODE*, void*, long, SYSTEM_ERROR*);
    VFS_NODE* (*Lookup)(VFS_NODE*, const char*, SYSTEM_ERROR*);
    int (*Create)(VFS_NODE*, const char*, long, VFS_PERMISSIONS, SYSTEM_ERROR*);
    int (*Unlink)(VFS_NODE*, const char*, SYSTEM_ERROR*);
    int (*Mkdir)(VFS_NODE*, const char*, VFS_PERMISSIONS, SYSTEM_ERROR*);
    int (*Rmdir)(VFS_NODE*, const char*, SYSTEM_ERROR*);
    int (*Symlink)(VFS_NODE*, const char*, const char*, VFS_PERMISSIONS, SYSTEM_ERROR*);
    int (*Readlink)(VFS_NODE*, VFS_NAME_BUFFER*, SYSTEM_ERROR*);
    int (*Link)(VFS_NODE*, VFS_NODE*, const char*, SYSTEM_ERROR*);
    int (*Rename)(VFS_NODE*, const char*, VFS_NODE*, const char*, long, SYSTEM_ERROR*);
    int (*Chmod)(VFS_NODE*, long, SYSTEM_ERROR*);
    int (*Chown)(VFS_NODE*, long, long, SYSTEM_ERROR*);
    int (*Truncate)(VFS_NODE*, long, SYSTEM_ERROR*);
    int (*Sync)(VFS_NODE*, SYSTEM_ERROR*);
    int (*Map)(VFS_NODE*, void**, long, long, SYSTEM_ERROR*);
    int (*Unmap)(VFS_NODE*, void*, long, SYSTEM_ERROR*);
} VFS_NODE_OPERATIONS;

typedef struct SUPER_OPERATIONS
{
    int (*Sync)(SUPER_BLOCK*, SYSTEM_ERROR*);
    int (*StatFs)(SUPER_BLOCK*, VFS_STAT_FILESYSTEM*, SYSTEM_ERROR*);
    void (*Release)(SUPER_BLOCK*, SYSTEM_ERROR*);
    int (*Umount)(SUPER_BLOCK*, SYSTEM_ERROR*);
} SUPER_OPERATIONS;

typedef struct FILESYSTEM_TYPE
{
    const char* Name; /*Fat32*/
    SUPER_BLOCK* (*Mount)(const char*, const char*, SYSTEM_ERROR*);
    void* Private;
} FILESYSTEM_TYPE;

/*SUPER_BLOCK*/
struct SUPER_BLOCK
{
    const FILESYSTEM_TYPE*  Type;
    void*                   Device;
    long                    Flags;
    VFS_NODE*               Root;
    const SUPER_OPERATIONS* Operations;
    void*                   Private;
};

/*Nodes*/
struct VFS_NODE
{
    VFS_NODE_TYPE               Type;
    const VFS_NODE_OPERATIONS* Operations;
    SUPER_BLOCK*               SuperBlock;
    void*                      Private;
    long                       ReferenceCount;
};

struct DIRECTORY_ENTRY
{
    const char*      Name;
    DIRECTORY_ENTRY* Parent;
    VFS_NODE*        Node;
    long             Flags;
}; /*DIRECTORY_ENTRY*/

struct FILE
{
    VFS_NODE* Node;
    long      Offset;
    long      Flags;
    long      ReferenceCount;
    void*     Private;
}; /*Filentry*/


extern const long MaxFileSystemTypes;
extern const long MaxMounts;
extern const FILESYSTEM_TYPE* FileSystemRegistry[32];
extern long FileSystemCount;

typedef struct MOUNT_ENTRY
{
    SUPER_BLOCK* SuperBlock;
    char         Path[1024];
} MOUNT_ENTRY;

extern MOUNT_ENTRY Mounts[64];
extern long MountsCount;
extern VFS_NODE* RootNode;
extern DIRECTORY_ENTRY* RootDirectoryEntry;
extern long  UnMask;
extern long  MaxName;
extern long  MaxPath;
extern long  DirectoryCacheLimit;
extern long  FileCacheLimit;
extern long  IOBlockSize;
extern char  DefaultFileSystem[64];

int VfsInit(SYSTEM_ERROR*);
int VfsShutdown(SYSTEM_ERROR*);

int           VfsRegisterFs(const FILESYSTEM_TYPE*, SYSTEM_ERROR*);
int           VfsUnregisterFs(const char*, SYSTEM_ERROR*);
const FILESYSTEM_TYPE* VfsFindFs(const char*, SYSTEM_ERROR*);
long          VfsListFs(const char**, long, SYSTEM_ERROR*);

SUPER_BLOCK* VfsMount(const char*, const char*, const char*, long, const char*, SYSTEM_ERROR*);
int         VfsUnmount(const char*, SYSTEM_ERROR*);
int         VfsSwitchRoot(const char*, SYSTEM_ERROR*);

int VfsBindMount(const char*, const char*, SYSTEM_ERROR*);
int VfsMoveMount(const char*, const char*, SYSTEM_ERROR*);
int VfsRemount(const char*, long, const char*, SYSTEM_ERROR*);

DIRECTORY_ENTRY* VfsResolve(const char*, SYSTEM_ERROR*);
DIRECTORY_ENTRY* VfsResolveAt(DIRECTORY_ENTRY*, const char*, SYSTEM_ERROR*);
VFS_NODE*  VfsLookup(DIRECTORY_ENTRY*, const char*, SYSTEM_ERROR*);
int     VfsMkpath(const char*, long, SYSTEM_ERROR*);
int     VfsRealpath(const char*, char*, long, SYSTEM_ERROR*);

FILE* VfsOpen(const char*, long, SYSTEM_ERROR*);
FILE* VfsOpenAt(DIRECTORY_ENTRY*, const char*, long, SYSTEM_ERROR*);
int   VfsClose(FILE*, SYSTEM_ERROR*);
long  VfsRead(FILE*, void*, long, SYSTEM_ERROR*);
long  VfsWrite(FILE*, const void*, long, SYSTEM_ERROR*);
long  VfsLseek(FILE*, long, int, SYSTEM_ERROR*);
int   VfsIoctl(FILE*, uint64_t, void*, SYSTEM_ERROR*);
int   VfsFsync(FILE*, SYSTEM_ERROR*);
int   VfsFstats(FILE*, VFS_STAT*, SYSTEM_ERROR*);
int   VfsStats(const char*, VFS_STAT*, SYSTEM_ERROR*);

long VfsReaddir(const char*, void*, long, SYSTEM_ERROR*);
long VfsReaddirF(FILE*, void*, long, SYSTEM_ERROR*);

int VfsCreate(const char*, long, VFS_PERMISSIONS, SYSTEM_ERROR*);
int VfsUnlink(const char*, SYSTEM_ERROR*);
int VfsMkdir(const char*, VFS_PERMISSIONS, SYSTEM_ERROR*);
int VfsRmdir(const char*, SYSTEM_ERROR*);
int VfsSymlink(const char*, const char*, VFS_PERMISSIONS, SYSTEM_ERROR*);
int VfsReadlink(const char*, char*, long, SYSTEM_ERROR*);
int VfsLink(const char*, const char*, SYSTEM_ERROR*);
int VfsRename(const char*, const char*, long, SYSTEM_ERROR*);
int VfsChmod(const char*, long, SYSTEM_ERROR*);
int VfsChown(const char*, long, long, SYSTEM_ERROR*);
int VfsTruncate(const char*, long, SYSTEM_ERROR*);

int VnodeRefInc(VFS_NODE*, SYSTEM_ERROR*);
int VnodeRefDec(VFS_NODE*, SYSTEM_ERROR*);
int VnodeGetAttr(VFS_NODE*, VFS_STAT*, SYSTEM_ERROR*);
int VnodeSetAttr(VFS_NODE*, const VFS_STAT*, SYSTEM_ERROR*);
int DentryInvalidate(DIRECTORY_ENTRY*, SYSTEM_ERROR*);
int DentryRevalidate(DIRECTORY_ENTRY*, SYSTEM_ERROR*);
int DentryAttach(DIRECTORY_ENTRY*, VFS_NODE*, SYSTEM_ERROR*);
int DentryDetach(DIRECTORY_ENTRY*, SYSTEM_ERROR*);
int DentryName(DIRECTORY_ENTRY*, char*, long, SYSTEM_ERROR*);

int VfsSetCwd(const char*, SYSTEM_ERROR*);
int VfsGetCwd(char*, long, SYSTEM_ERROR*);
int VfsSetRoot(const char*, SYSTEM_ERROR*);
int VfsGetRoot(char*, long, SYSTEM_ERROR*);

int  VfsSetUmask(long, SYSTEM_ERROR*);
long VfsGetUmask(SYSTEM_ERROR*);

int VfsNotifySubscribe(const char*, long, SYSTEM_ERROR*);
int VfsNotifyUnsubscribe(const char*, SYSTEM_ERROR*);
int VfsNotifyPoll(const char*, long*, SYSTEM_ERROR*);

int VfsAccess(const char*, long, SYSTEM_ERROR*);
int VfsExists(const char*, SYSTEM_ERROR*);
int VfsIsDir(const char*, SYSTEM_ERROR*);
int VfsIsFile(const char*, SYSTEM_ERROR*);
int VfsIsSymlink(const char*, SYSTEM_ERROR*);

int VfsCopy(const char*, const char*, long, SYSTEM_ERROR*);
int VfsMove(const char*, const char*, long, SYSTEM_ERROR*);

int VfsReadAll(const char*, void*, long, long*, SYSTEM_ERROR*);
int VfsWriteAll(const char*, const void*, long, SYSTEM_ERROR*);

int VfsMountTableEnumerate(char*, long, SYSTEM_ERROR*);
int VfsMountTableFind(const char*, char*, long, SYSTEM_ERROR*);

int VfsNodePath(VFS_NODE*, char*, long, SYSTEM_ERROR*);
int VfsNodeName(VFS_NODE*, char*, long, SYSTEM_ERROR*);

int VfsAllocName(char**, long, SYSTEM_ERROR*);
int VfsFreeName(char*, SYSTEM_ERROR*);
int VfsJoinPath(const char*, const char*, char*, long, SYSTEM_ERROR*);

int  VfsSetFlag(const char*, long, SYSTEM_ERROR*);
int  VfsClearFlag(const char*, long, SYSTEM_ERROR*);
long VfsGetFlags(const char*, SYSTEM_ERROR*);

int VfsSyncAll(SYSTEM_ERROR*);
int VfsPruneCaches(SYSTEM_ERROR*);

int VfsRegisterDevNode(const char*, void*, long, SYSTEM_ERROR*);
int VfsUnregisterDevNode(const char*, SYSTEM_ERROR*);
int VfsRegisterPseudoFs(const char*, SUPER_BLOCK*, SYSTEM_ERROR*);
int VfsUnregisterPseudoFs(const char*, SYSTEM_ERROR*);

int         VfsSetDefaultFs(const char*, SYSTEM_ERROR*);
const char* VfsGetDefaultFs(SYSTEM_ERROR*);

int  VfsSetMaxName(long, SYSTEM_ERROR*);
long VfsGetMaxName(SYSTEM_ERROR*);

int  VfsSetMaxPath(long, SYSTEM_ERROR*);
long VfsGetMaxPath(SYSTEM_ERROR*);

int  VfsSetDirCacheLimit(long, SYSTEM_ERROR*);
long VfsGetDirCacheLimit(SYSTEM_ERROR*);

int  VfsSetFileCacheLimit(long, SYSTEM_ERROR*);
long VfsGetFileCacheLimit(SYSTEM_ERROR*);

int  VfsSetIoBlockSize(long, SYSTEM_ERROR*);
long VfsGetIoBlockSize(SYSTEM_ERROR*);

/*StringHelpers.c*/
int IsSeperator(char, SYSTEM_ERROR*);
const char* SkipSeperator(const char*, SYSTEM_ERROR*);
long NextCompare(const char* Path, char* Output, long Capacity, SYSTEM_ERROR* Error);

/*CoreHelpers.c*/
DIRECTORY_ENTRY* AllocateDirectoryEntry(const char*, DIRECTORY_ENTRY*, VFS_NODE*, SYSTEM_ERROR*);
DIRECTORY_ENTRY* Walk(VFS_NODE*, DIRECTORY_ENTRY*, const char*, SYSTEM_ERROR*);
MOUNT_ENTRY* FindMount(const char*, SYSTEM_ERROR*);

KEXPORT(VfsInit)
KEXPORT(VfsShutdown)
KEXPORT(VfsRegisterFs)
KEXPORT(VfsUnregisterFs)
KEXPORT(VfsFindFs)
KEXPORT(VfsListFs)
KEXPORT(VfsMount)
KEXPORT(VfsUnmount)
KEXPORT(VfsSwitchRoot)
KEXPORT(VfsBindMount)
KEXPORT(VfsMoveMount)
KEXPORT(VfsRemount)
KEXPORT(VfsResolve)
KEXPORT(VfsResolveAt)
KEXPORT(VfsLookup)
KEXPORT(VfsMkpath)
KEXPORT(VfsRealpath)
KEXPORT(VfsOpen)
KEXPORT(VfsOpenAt)
KEXPORT(VfsClose)
KEXPORT(VfsRead)
KEXPORT(VfsWrite)
KEXPORT(VfsLseek)
KEXPORT(VfsIoctl)
KEXPORT(VfsFsync)
KEXPORT(VfsFstats)
KEXPORT(VfsStats)
KEXPORT(VfsReaddir)
KEXPORT(VfsReaddirF)
KEXPORT(VfsCreate)
KEXPORT(VfsUnlink)
KEXPORT(VfsMkdir)
KEXPORT(VfsRmdir)
KEXPORT(VfsSymlink)
KEXPORT(VfsReadlink)
KEXPORT(VfsLink)
KEXPORT(VfsRename)
KEXPORT(VfsChmod)
KEXPORT(VfsChown)
KEXPORT(VfsTruncate)
KEXPORT(VnodeRefInc)
KEXPORT(VnodeRefDec)
KEXPORT(VnodeGetAttr)
KEXPORT(VnodeSetAttr)
KEXPORT(DentryInvalidate)
KEXPORT(DentryRevalidate)
KEXPORT(DentryAttach)
KEXPORT(DentryDetach)
KEXPORT(DentryName)
KEXPORT(VfsSetCwd)
KEXPORT(VfsGetCwd)
KEXPORT(VfsSetRoot)
KEXPORT(VfsGetRoot)
KEXPORT(VfsSetUmask)
KEXPORT(VfsGetUmask)
KEXPORT(VfsNotifySubscribe)
KEXPORT(VfsNotifyUnsubscribe)
KEXPORT(VfsNotifyPoll)
KEXPORT(VfsAccess)
KEXPORT(VfsExists)
KEXPORT(VfsIsDir)
KEXPORT(VfsIsFile)
KEXPORT(VfsIsSymlink)
KEXPORT(VfsCopy)
KEXPORT(VfsMove)
KEXPORT(VfsReadAll)
KEXPORT(VfsWriteAll)
KEXPORT(VfsJoinPath)
KEXPORT(VfsSetFlag)
KEXPORT(VfsClearFlag)
KEXPORT(VfsGetFlags)
KEXPORT(VfsSyncAll)
KEXPORT(VfsPruneCaches)
KEXPORT(VfsSetDefaultFs)
KEXPORT(VfsGetDefaultFs)
KEXPORT(VfsSetMaxName)
KEXPORT(VfsGetMaxName)
KEXPORT(VfsSetMaxPath)
KEXPORT(VfsGetMaxPath)
KEXPORT(VfsSetDirCacheLimit)
KEXPORT(VfsGetDirCacheLimit)
KEXPORT(VfsSetFileCacheLimit)
KEXPORT(VfsGetFileCacheLimit)
KEXPORT(VfsSetIoBlockSize)
KEXPORT(VfsGetIoBlockSize)