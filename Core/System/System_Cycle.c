#include <System.h>
#include <VirtualFileSystem.h>
#include <DirtyHeap.h>
#include <KernelCLibrary.h>

SYSTEM_NODE* SystemRoot = 0;
SYSTEM_FILESYSTEM_INSTANCE* SystemInstance = 0;
SYSTEM_NODE* NodePool = 0;
uint64_t NodeAllocatedCount = 0;
SYSTEM_FILE* FilePool = 0;
uint64_t FileAllocatedCount = 0;
const uint64_t MaxSystemNodes = 1024;
const uint64_t MaxSystemFiles = 256;

int
SystemInit(SYSTEM_ERROR* Error)
{
    #define ErrorOut_SystemInit(Code) \
        ErrorOut(Error, Code, FUNC_SystemInit)

    NodeAllocatedCount = 0;
    FileAllocatedCount = 0;
    
    NodePool = KMalloc(MaxSystemNodes * sizeof(SYSTEM_NODE), Error);
    if (Probe4Error(NodePool) || !NodePool)
    {
        ErrorOut_SystemInit(-ENOMEM);
        return Error->ErrorCode;
    }
    
    FilePool = KMalloc(MaxSystemFiles * sizeof(SYSTEM_FILE), Error);
    if (Probe4Error(FilePool) || !FilePool)
    {
        KFree(NodePool, Error);

        ErrorOut_SystemInit(-ENOMEM);
        return Error->ErrorCode;
    }
    
    memset(NodePool, 0, MaxSystemNodes * sizeof(SYSTEM_NODE));
    memset(FilePool, 0, MaxSystemFiles * sizeof(SYSTEM_FILE));
    
    SystemInstance = KMalloc(sizeof(SYSTEM_FILESYSTEM_INSTANCE), Error);
    if (Probe4Error(SystemInstance) || !SystemInstance)
    {
        KFree(NodePool, Error);
        KFree(FilePool, Error);

        ErrorOut_SystemInit(-ENOMEM);
        return Error->ErrorCode;
    }
    
    SystemInstance->SuperBlock = 0;
    SystemInstance->Root = 0;
    SystemInstance->NodeCount = 0;
    SystemInstance->FileCount = 0;
    
    return GeneralOK;
}

int
SystemShutdown(SYSTEM_ERROR* Error)
{
    if (SystemInstance)
    {
        KFree(SystemInstance, Error);
        SystemInstance = 0;
    }
    
    if (FilePool)
    {
        KFree(FilePool, Error);
        FilePool = 0;
    }
    
    if (NodePool)
    {
        KFree(NodePool, Error);
        NodePool = 0;
    }
    
    SystemRoot = 0;
    NodeAllocatedCount = 0;
    FileAllocatedCount = 0;
    
    return GeneralOK;
}