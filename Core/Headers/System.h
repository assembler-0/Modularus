#pragma once

#include <__KCONF.h>
#include <Errors.h>
#include <VirtualFileSystem.h>

typedef struct SYSTEM_NODE SYSTEM_NODE;
typedef struct SYSTEM_FILE SYSTEM_FILE;

typedef struct SYSTEM_OPERATIONS
{
    int (*Open)(SYSTEM_NODE*, SYSTEM_FILE*, SYSTEM_ERROR*);
    int (*Close)(SYSTEM_FILE*, SYSTEM_ERROR*);
    long (*Read)(SYSTEM_FILE*, void*, uint64_t, SYSTEM_ERROR*);
    long (*Write)(SYSTEM_FILE*, const void*, uint64_t, SYSTEM_ERROR*);
    int (*Getattr)(SYSTEM_NODE*, VFS_STAT*, SYSTEM_ERROR*);
    int (*Setattr)(SYSTEM_NODE*, const VFS_STAT*, SYSTEM_ERROR*);
} SYSTEM_OPERATIONS;

typedef enum SYSTEM_NODE_TYPE_ENUMERATION
{
    SYSTEM_NODE_TYPE_ENUMERATION_NONE,
    SYSTEM_NODE_TYPE_ENUMERATION_FILE,
    SYSTEM_NODE_TYPE_ENUMERATION_DIRECTORY,
    SYSTEM_NODE_TYPE_ENUMERATION_SYMBOLIC_LINK
} SYSTEM_NODE_TYPE_ENUMERATION;

struct SYSTEM_FILE
{
    SYSTEM_NODE* Node;
    uint64_t Offset;
    uint64_t Flags;
    uint64_t ReferenceCount;
    void* Private;
};

struct SYSTEM_NODE
{
    const char* Name;
    SYSTEM_NODE_TYPE_ENUMERATION Type;
    const SYSTEM_OPERATIONS* Operations;
    SYSTEM_NODE* Parent;
    SYSTEM_NODE* Child;
    SYSTEM_NODE* Next;
    void* Context;
    uint64_t ContextSize;
    uint64_t Mode;
    uint64_t UserID;
    uint64_t GroupID;
    uint64_t ReferenceCount;
};

typedef struct SYSTEM_FILESYSTEM_INSTANCE
{
    SUPER_BLOCK* SuperBlock;
    SYSTEM_NODE* Root;
    uint64_t NodeCount;
    uint64_t FileCount;
} SYSTEM_FILESYSTEM_INSTANCE;

extern SYSTEM_NODE* SystemRoot;
extern SYSTEM_FILESYSTEM_INSTANCE* SystemInstance;
extern SYSTEM_NODE* NodePool;
extern uint64_t NodeAllocatedCount;
extern SYSTEM_FILE* FilePool;
extern uint64_t FileAllocatedCount;
extern const uint64_t MaxSystemNodes;
extern const uint64_t MaxSystemFiles;

int SystemInit(SYSTEM_ERROR* Error);
int SystemShutdown(SYSTEM_ERROR* Error);

SYSTEM_NODE* SystemCreateNode(const char*, SYSTEM_NODE_TYPE_ENUMERATION, const SYSTEM_OPERATIONS*, void*, uint64_t, SYSTEM_ERROR*);
int SystemDeleteNode(SYSTEM_NODE*, SYSTEM_ERROR*);
SYSTEM_NODE* SystemFindNode(SYSTEM_NODE*, const char*, SYSTEM_ERROR*);
SYSTEM_NODE* SystemGetRoot(SYSTEM_ERROR*);
int SystemAttachNode(SYSTEM_NODE*, SYSTEM_NODE*, SYSTEM_ERROR*);
int SystemDetachNode(SYSTEM_NODE*, SYSTEM_ERROR*);

SYSTEM_NODE* SystemCreateFile(const char*, const SYSTEM_OPERATIONS*, void*, uint64_t, SYSTEM_ERROR*);
SYSTEM_NODE* SystemCreateDirectory(const char*, SYSTEM_ERROR*);

int SystemVfsOpen(VFS_NODE*, FILE*, SYSTEM_ERROR*);
int SystemVfsClose(FILE*, SYSTEM_ERROR*);
long SystemVfsRead(FILE*, void*, uint64_t, SYSTEM_ERROR*);
long SystemVfsWrite(FILE*, const void*, uint64_t, SYSTEM_ERROR*);

long SystemVfsReaddir(VFS_NODE*, void*, uint64_t, SYSTEM_ERROR*);
VFS_NODE* SystemVfsLookup(VFS_NODE*, const char*, SYSTEM_ERROR*);

int SystemVfsStat(VFS_NODE*, VFS_STAT*, SYSTEM_ERROR*);
int SystemGetattr(SYSTEM_NODE*, VFS_STAT*, SYSTEM_ERROR*);
int SystemSetattr(SYSTEM_NODE*, const VFS_STAT*, SYSTEM_ERROR*);

int SystemStatFs(SUPER_BLOCK*, VFS_STAT_FILESYSTEM*, SYSTEM_ERROR*);
int SystemSync(SUPER_BLOCK*, SYSTEM_ERROR*);
void SystemRelease(SUPER_BLOCK*, SYSTEM_ERROR*);

int SystemSetContext(SYSTEM_NODE*, void*, uint64_t, SYSTEM_ERROR*);
void* SystemGetContext(SYSTEM_NODE*, SYSTEM_ERROR*);
uint64_t SystemGetContextSize(SYSTEM_NODE*, SYSTEM_ERROR*);

SYSTEM_NODE* SystemWalkPath(const char*, SYSTEM_ERROR*);
