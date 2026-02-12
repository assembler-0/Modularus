#include <System.h>
#include <VirtualFileSystem.h>
#include <DirtyHeap.h>
#include <KernelCLibrary.h>

SYSTEM_NODE*
SystemCreateNode(const char* Name, SYSTEM_NODE_TYPE_ENUMERATION Type, const SYSTEM_OPERATIONS* Operations, void* Context, uint64_t ContextSize, SYSTEM_ERROR* Error)
{
    #define ErrorOut_SystemCreateNode(Code) \
        ErrorOut(Error, Code, FUNC_SystemCreateNode)

    if (NodeAllocatedCount >= MaxSystemNodes)
    {
        ErrorOut_SystemCreateNode(-ENOSPC);
        return Error2Pointer(Error->ErrorCode);
    }
    
    SYSTEM_NODE* Node = &NodePool[NodeAllocatedCount++];
    memset(Node, 0, sizeof(SYSTEM_NODE));
    
    Node->Name = Name;
    Node->Type = Type;
    Node->Operations = Operations;
    Node->Context = Context;
    Node->ContextSize = ContextSize;
    Node->Mode = 0644;
    Node->UserID = 0;
    Node->GroupID = 0;
    Node->ReferenceCount = 1;
    
    if (SystemInstance)
    {
        SystemInstance->NodeCount++;
    }
    
    return Node;
}

int
SystemDeleteNode(SYSTEM_NODE* Node, SYSTEM_ERROR* Error)
{
    #define ErrorOut_SystemDeleteNode(Code) \
        ErrorOut(Error, Code, FUNC_SystemDeleteNode)

    if (Probe4Error(Node) || !Node)
    {
        ErrorOut_SystemDeleteNode(-EINVAL);
        return Error->ErrorCode;
    }
    
    if (Node->Parent)
    {
        SYSTEM_NODE* Child = Node->Parent->Child;
        SYSTEM_NODE* Previous = 0;
        
        while (Child)
        {
            if (Child == Node)
            {
                if (Previous)
                {
                    Previous->Next = Node->Next;
                }
                else
                {
                    Node->Parent->Child = Node->Next;
                }
                break;
            }
            Previous = Child;
            Child = Child->Next;
        }
    }
    
    memset(Node, 0, sizeof(SYSTEM_NODE));
    NodeAllocatedCount--;
    
    return GeneralOK;
}

SYSTEM_NODE*
SystemFindNode(SYSTEM_NODE* Parent, const char* Name, SYSTEM_ERROR* Error)
{
    #define ErrorOut_SystemFindNode(Code) \
        ErrorOut(Error, Code, FUNC_SystemFindNode)

    if (Probe4Error(Parent) || !Parent || Probe4Error(Name) || !Name)
    {
        ErrorOut_SystemFindNode(-EINVAL);
        return Error2Pointer(Error->ErrorCode);
    }
    
    SYSTEM_NODE* Child = Parent->Child;
    
    while (Child)
    {
        if (strcmp(Child->Name, Name) == 0)
        {
            return Child;
        }
        Child = Child->Next;
    }
    
    ErrorOut_SystemFindNode(-EINVAL);
    return Error2Pointer(Error->ErrorCode);
}

int
SystemAttachNode(SYSTEM_NODE* Parent, SYSTEM_NODE* Child, SYSTEM_ERROR* Error)
{
    #define ErrorOut_SystemAttachNode(Code) \
        ErrorOut(Error, Code, FUNC_SystemAttachNode)

    if (Probe4Error(Parent) || !Parent || Probe4Error(Child) || !Child)
    {
        ErrorOut_SystemAttachNode(-EINVAL);
        return Error->ErrorCode;
    }
    
    Child->Parent = Parent;
    Child->Next = Parent->Child;
    Parent->Child = Child;
    
    return GeneralOK;
}

int
SystemDetachNode(SYSTEM_NODE* Node, SYSTEM_ERROR* Error)
{
    return SystemDeleteNode(Node, Error);
}

SYSTEM_NODE*
SystemGetRoot(SYSTEM_ERROR* Error)
{
    #define ErrorOut_SystemGetRoot(Code) \
        ErrorOut(Error, Code, FUNC_SystemGetRoot)

    if (Probe4Error(SystemRoot) || !SystemRoot)
    {
        ErrorOut_SystemGetRoot(-ENOENT);
        return Error2Pointer(Error->ErrorCode);
    }
    
    return SystemRoot;
}