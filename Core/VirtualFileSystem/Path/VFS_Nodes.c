#include <KernelCLibrary.h>
#include <VirtualFileSystem.h>
#include <__KCONF.h>
#include <DirtyHeap.h>

int
VnodeRefInc(VFS_NODE* Node, SYSTEM_ERROR* Error)
{
    #define ErrorOut_VnodeRefInc(Code) \
        ErrorOut(Error, Code, FUNC_VnodeRefInc)

    if (Probe4Error(Node) || !Node)
    {
        ErrorOut_VnodeRefInc(-EINVAL);
        return Error->ErrorCode;
    }

    Node->ReferenceCount++;
    
    return (int)Node->ReferenceCount;
}

int
VnodeRefDec(VFS_NODE* Node, SYSTEM_ERROR* Error)
{
    #define ErrorOut_VnodeRefDec(Code) \
        ErrorOut(Error, Code, FUNC_VnodeRefDec)
    
    if (Probe4Error(Node) || !Node)
    {
        ErrorOut_VnodeRefDec(-EINVAL);
        return Error->ErrorCode;
    }

    if (Node->ReferenceCount > 0)
    {
        Node->ReferenceCount--;
    }
    
    return (int)Node->ReferenceCount;
}

int
VnodeGetAttr(VFS_NODE* Node, VFS_STAT* Buffer, SYSTEM_ERROR* Error)
{
    #define ErrorOut_VnodeGetAttr(Code) \
        ErrorOut(Error, Code, FUNC_VnodeGetAttr)

    if (Probe4Error(Node) || !Node || Probe4Error(Buffer) || !Buffer)
    {
        ErrorOut_VnodeGetAttr(-EINVAL);
        return Error->ErrorCode;
    }

    if (Probe4Error(Node->Operations) || !Node->Operations || Probe4Error(Node->Operations->Stat) || !Node->Operations->Stat)
    {
        ErrorOut_VnodeGetAttr(-ENOSYS);
        return Error->ErrorCode;
    }
    
    return Node->Operations->Stat(Node, Buffer, Error);
}

int
VnodeSetAttr(VFS_NODE* Node __UNUSED, const VFS_STAT* Buffer __UNUSED, SYSTEM_ERROR* Error)
{
    #define ErrorOut_VnodeSetAttr(Code) \
        ErrorOut(Error, Code, FUNC_VnodeSetAttr)

    ErrorOut_VnodeSetAttr(-ENOSYS);
    return Error->ErrorCode;
}