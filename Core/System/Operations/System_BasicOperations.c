
#include <System.h>
#include <VirtualFileSystem.h>
#include <DirtyHeap.h>
#include <KernelCLibrary.h>

int
SystemVfsOpen(VFS_NODE* VFSNode, FILE* File, SYSTEM_ERROR* Error)
{
    #define ErrorOut_SystemVfsOpen(Code) \
        ErrorOut(Error, Code, FUNC_SystemVfsOpen)

    if (Probe4Error(VFSNode) || !VFSNode || Probe4Error(File) || !File)
    {
        ErrorOut_SystemVfsOpen(-EINVAL);
        return Error->ErrorCode;
    }
    
    SYSTEM_NODE* Node = (SYSTEM_NODE*)VFSNode->Private;
    
    if (Probe4Error(Node) || !Node)
    {
        ErrorOut_SystemVfsOpen(-ENOENT);
        return Error->ErrorCode;
    }
    
    if (FileAllocatedCount >= MaxSystemFiles)
    {
        ErrorOut_SystemVfsOpen(-ENFILE);
        return Error->ErrorCode;
    }
    
    SYSTEM_FILE* SystemFile = &FilePool[FileAllocatedCount++];
    memset(SystemFile, 0, sizeof(SYSTEM_FILE));
    
    SystemFile->Node = Node;
    SystemFile->Offset = 0;
    SystemFile->Flags = 0;
    SystemFile->ReferenceCount = 1;
    
    File->Private = SystemFile;
    
    if (Node->Operations && Node->Operations->Open)
    {
        return Node->Operations->Open(Node, SystemFile, Error);
    }
    
    return GeneralOK;
}

int
SystemVfsClose(FILE* File, SYSTEM_ERROR* Error)
{
    #define ErrorOut_SystemVfsClose(Code) \
        ErrorOut(Error, Code, FUNC_SystemVfsClose)

    if (Probe4Error(File) || !File)
    {
        ErrorOut_SystemVfsClose(-EINVAL);
        return Error->ErrorCode;
    }
    
    SYSTEM_FILE* SystemFile = (SYSTEM_FILE*)File->Private;
    
    if (Probe4Error(SystemFile) || !SystemFile)
    {
        ErrorOut_SystemVfsClose(-EBADF);
        return Error->ErrorCode;
    }
    
    memset(SystemFile, 0, sizeof(SYSTEM_FILE));
    FileAllocatedCount--;
    
    return GeneralOK;
}

long
SystemVfsRead(FILE* File, void* Buffer, uint64_t Size, SYSTEM_ERROR* Error)
{
    #define ErrorOut_SystemVfsRead(Code) \
        ErrorOut(Error, Code, FUNC_SystemVfsRead)

    if (Probe4Error(File) || !File || Probe4Error(Buffer) || !Buffer)
    {
        ErrorOut_SystemVfsRead(-EINVAL);
        return Error->ErrorCode;
    }
    
    SYSTEM_FILE* SystemFile = (SYSTEM_FILE*)File->Private;
    
    if (Probe4Error(SystemFile) || !SystemFile || Probe4Error(SystemFile->Node) || !SystemFile->Node)
    {
        ErrorOut_SystemVfsRead(-EBADF);
        return Error->ErrorCode;
    }
    
    SYSTEM_NODE* Node = SystemFile->Node;
    
    if (Node->Operations && Node->Operations->Read)
    {
        return Node->Operations->Read(SystemFile, Buffer, Size, Error);
    }
    
    if (Probe4Error(Node->Context) || !Node->Context)
    {
        ErrorOut_SystemVfsRead(-ENOENT);
        return Error->ErrorCode;
    }
    
    uint64_t Available = Node->ContextSize - SystemFile->Offset;
    
    if (Size > Available)
    {
        Size = Available;
    }
    
    if (Size > 0)
    {
        memcpy(Buffer, (char*)Node->Context + SystemFile->Offset, Size);
        SystemFile->Offset += Size;
    }
    
    return Size;
}

long
SystemVfsWrite(FILE* File, const void* Buffer, uint64_t Size, SYSTEM_ERROR* Error)
{
    #define ErrorOut_SystemVfsWrite(Code) \
        ErrorOut(Error, Code, FUNC_SystemVfsWrite)

    if (Probe4Error(File) || !File || Probe4Error(Buffer) || !Buffer)
    {
        ErrorOut_SystemVfsWrite(-EINVAL);
        return Error->ErrorCode;
    }
    
    SYSTEM_FILE* SystemFile = (SYSTEM_FILE*)File->Private;
    
    if (Probe4Error(SystemFile) || !SystemFile || Probe4Error(SystemFile->Node) || !SystemFile->Node)
    {
        ErrorOut_SystemVfsWrite(-EBADF);
        return Error->ErrorCode;
    }
    
    SYSTEM_NODE* Node = SystemFile->Node;
    
    if (Node->Operations && Node->Operations->Write)
    {
        return Node->Operations->Write(SystemFile, Buffer, Size, Error);
    }
    
    ErrorOut_SystemVfsWrite(-EROFS);
    return Error->ErrorCode;
}