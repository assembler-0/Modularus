#include <System.h>
#include <VirtualFileSystem.h>
#include <DirtyHeap.h>
#include <KernelCLibrary.h>

int
SystemSetContext(SYSTEM_NODE* Node, void* Context, uint64_t Size, SYSTEM_ERROR* Error)
{
    #define ErrorOut_SystemSetContext(Code) \
        ErrorOut(Error, Code, FUNC_SystemSetContext)

    if (Probe4Error(Node) || !Node)
    {
        ErrorOut_SystemSetContext(-EINVAL);
        return Error->ErrorCode;
    }
    
    Node->Context = Context;
    Node->ContextSize = Size;
    
    return GeneralOK;
}

void*
SystemGetContext(SYSTEM_NODE* Node, SYSTEM_ERROR* Error)
{
    #define ErrorOut_SystemGetContext(Code) \
        ErrorOut(Error, Code, FUNC_SystemGetContext)

    if (Probe4Error(Node) || !Node)
    {
        ErrorOut_SystemGetContext(-EINVAL);
        return Error2Pointer(Error->ErrorCode);
    }
    
    return Node->Context;
}

uint64_t
SystemGetContextSize(SYSTEM_NODE* Node, SYSTEM_ERROR* Error)
{
    #define ErrorOut_SystemGetContextSize(Code) \
        ErrorOut(Error, Code, FUNC_SystemGetContextSize)

    if (Probe4Error(Node) || !Node)
    {
        ErrorOut_SystemGetContextSize(-EINVAL);
        return 0; /*this is indeed a error*/
    }
    
    return Node->ContextSize;
}