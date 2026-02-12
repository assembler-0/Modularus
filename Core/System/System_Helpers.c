#include <System.h>
#include <VirtualFileSystem.h>
#include <DirtyHeap.h>
#include <KernelCLibrary.h>

VFS_NODE*
SystemVfsLookup(VFS_NODE* VFSNode, const char* Name, SYSTEM_ERROR* Error)
{
    #define ErrorOut_SystemVfsLookup(Code) \
        ErrorOut(Error, Code, FUNC_SystemVfsLookup)

    if (Probe4Error(VFSNode) || !VFSNode || Probe4Error(Name) || !Name)
    {
        ErrorOut_SystemVfsLookup(-EINVAL);
        return Error2Pointer(Error->ErrorCode);
    }
    
    SYSTEM_NODE* Node = (SYSTEM_NODE*)VFSNode->Private;
     
    if (Probe4Error(Node) || !Node)
    {
        ErrorOut_SystemVfsLookup(-ENOENT);
        return Error2Pointer(Error->ErrorCode);
    }
    
    SYSTEM_NODE* Found = SystemFindNode(Node, Name, Error);
    
    if (Probe4Error(Found) || !Found)
    {
        ErrorOut_SystemVfsLookup(-ENOENT);
        return Error2Pointer(Error->ErrorCode);
    }
    
    VFS_NODE* NodeAllocationResult = KMalloc(sizeof(VFS_NODE), Error);
    
    if (Probe4Error(NodeAllocationResult) || !NodeAllocationResult)
    {
        ErrorOut_SystemVfsLookup(-ENOMEM);
        return Error2Pointer(Error->ErrorCode);
    }
    
    NodeAllocationResult->Type = (Found->Type == SYSTEM_NODE_TYPE_ENUMERATION_DIRECTORY) ? VFSNode_DIRECTORY : VFSNode_FILE;
    NodeAllocationResult->Private = Found;
    NodeAllocationResult->ReferenceCount = 1;
    
    return NodeAllocationResult;
}

SYSTEM_NODE*
SystemWalkPath(const char* Path, SYSTEM_ERROR* Error)
{
    #define ErrorOut_SystemWalkPath(Code) \
        ErrorOut(Error, Code, FUNC_SystemWalkPath)

    if (Probe4Error(Path) || !Path)
    {
        ErrorOut_SystemWalkPath(-EINVAL);
        return Error2Pointer(Error->ErrorCode);
    }
    
    SYSTEM_NODE* Current = SystemRoot;
    const char* Component = Path;
    
    if (*Component == '/')
    {
        Component++;
    }
    
    while (*Component)
    {
        const char* End = Component;
        
        while (*End && *End != '/')
        {
            End++;
        }
        
        char Name[256];
        uint64_t Length = End - Component;
        
        if (Length >= sizeof(Name))
        {
            ErrorOut_SystemWalkPath(-ENAMETOOLONG);
            return Error2Pointer(Error->ErrorCode);
        }
        
        memcpy(Name, Component, Length);
        Name[Length] = 0;
        
        Current = SystemFindNode(Current, Name, Error);
        
        if (Probe4Error(Current) || !Current)
        {
            ErrorOut_SystemWalkPath(-ENOENT);
            return Error2Pointer(Error->ErrorCode);
        }
        
        Component = End;
        
        if (*Component == '/')
        {
            Component++;
        }
    }
    
    return Current;
}
