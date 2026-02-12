#include <System.h>
#include <VirtualFileSystem.h>
#include <DirtyHeap.h>
#include <KernelCLibrary.h>

int
SystemGetattr(SYSTEM_NODE* Node, VFS_STAT* Stat, SYSTEM_ERROR* Error)
{
    #define ErrorOut_SystemGetattr(Code) \
        ErrorOut(Error, Code, FUNC_SystemGetattr)

    if (Probe4Error(Node) || !Node || Probe4Error(Stat) || !Stat)
    {
        ErrorOut_SystemGetattr(-EINVAL);
        return Error->ErrorCode;
    }
    
    Stat->Inode = (long)Node;
    Stat->Size = Node->ContextSize;
    Stat->Blocks = 0;
    Stat->BlockSize = 4096;
    Stat->NumberOfLinks = 1;
    Stat->RawDevice = 0;
    Stat->Device = 0;
    Stat->Flags = 0;
    Stat->Type = (Node->Type == SYSTEM_NODE_TYPE_ENUMERATION_DIRECTORY) ? VFSNode_DIRECTORY : VFSNode_FILE;
    Stat->Permission.Mode = Node->Mode;
    Stat->Permission.UserID = Node->UserID;
    Stat->Permission.GroupID = Node->GroupID;
    
    return GeneralOK;
}

int
SystemSetattr(SYSTEM_NODE* Node, const VFS_STAT* Stat, SYSTEM_ERROR* Error)
{
    #define ErrorOut_SystemSetattr(Code) \
        ErrorOut(Error, Code, FUNC_SystemSetattr)

    if (Probe4Error(Node) || !Node || Probe4Error(Stat) || !Stat)
    {
        ErrorOut_SystemSetattr(-EINVAL);
        return Error->ErrorCode;
    }
    
    ErrorOut_SystemSetattr(-EROFS);
    return Error->ErrorCode;
}