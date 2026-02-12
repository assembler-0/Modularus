#include <System.h>
#include <VirtualFileSystem.h>
#include <DirtyHeap.h>
#include <KernelCLibrary.h>

SYSTEM_NODE*
SystemCreateFile(const char* Name, const SYSTEM_OPERATIONS* Operations, void* Context, uint64_t ContextSize, SYSTEM_ERROR* Error)
{
    return SystemCreateNode(Name, SYSTEM_NODE_TYPE_ENUMERATION_FILE, Operations, Context, ContextSize, Error);
}

SYSTEM_NODE*
SystemCreateDirectory(const char* Name, SYSTEM_ERROR* Error)
{
    return SystemCreateNode(Name, SYSTEM_NODE_TYPE_ENUMERATION_DIRECTORY, 0, 0, 0, Error);
}