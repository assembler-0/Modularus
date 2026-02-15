#include <KernelCLibrary.h>
#include <VirtualFileSystem.h>
#include <__KCONF.h>
#include <DirtyHeap.h>

int
VFS_NodeReferenceIncrement(VFS_NODE* Node, SYSTEM_ERROR* Error)
{
    #define ErrorOut_VFS_NodeReferenceIncrement(Code) \
        ErrorOut(Error, Code, FUNC_VFS_NodeReferenceIncrement)

    if (Probe4Error(Node) || !Node)
    {
        ErrorOut_VFS_NodeReferenceIncrement(-EINVAL);
        return Error->ErrorCode;
    }

    Node->ReferenceCount++;
    
    return (int)Node->ReferenceCount;
}

int
VFS_NodeReferenceDecrement(VFS_NODE* Node, SYSTEM_ERROR* Error)
{
    #define ErrorOut_VFS_NodeReferenceDecrement(Code) \
        ErrorOut(Error, Code, FUNC_VFS_NodeReferenceDecrement)
    
    if (Probe4Error(Node) || !Node)
    {
        ErrorOut_VFS_NodeReferenceDecrement(-EINVAL);
        return Error->ErrorCode;
    }

    if (Node->ReferenceCount > 0)
    {
        Node->ReferenceCount--;
    }
    
    return (int)Node->ReferenceCount;
}

int
VFS_NodeGetAttribute(VFS_NODE* Node, VFS_STAT* Buffer, SYSTEM_ERROR* Error)
{
    #define ErrorOut_VFS_NodeGetAttribute(Code) \
        ErrorOut(Error, Code, FUNC_VFS_NodeGetAttribute)

    if (Probe4Error(Node) || !Node || Probe4Error(Buffer) || !Buffer)
    {
        ErrorOut_VFS_NodeGetAttribute(-EINVAL);
        return Error->ErrorCode;
    }

    if (Probe4Error(Node->Operations) || !Node->Operations || Probe4Error(Node->Operations->Stat) || !Node->Operations->Stat)
    {
        ErrorOut_VFS_NodeGetAttribute(-ENOSYS);
        return Error->ErrorCode;
    }
    
    return Node->Operations->Stat(Node, Buffer, Error);
}

int
VFS_NodeSetAttribute(VFS_NODE* Node __unused, const VFS_STAT* Buffer __unused, SYSTEM_ERROR* Error)
{
    #define ErrorOut_VFS_NodeSetAttribute(Code) \
        ErrorOut(Error, Code, FUNC_VFS_NodeSetAttribute)

    ErrorOut_VFS_NodeSetAttribute(-ENOSYS);
    return Error->ErrorCode;
}