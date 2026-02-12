#include <KernelCLibrary.h>
#include <VirtualFileSystem.h>
#include <__KCONF.h>
#include <DirtyHeap.h>

FILE*
VfsOpen(const char* Path, long Flags, SYSTEM_ERROR* Error)
{
    #define ErrorOut_VfsOpen(Code) \
        ErrorOut(Error, Code, FUNC_VfsOpen)

    DIRECTORY_ENTRY* DirectoryEntry = VfsResolve(Path, Error);
    if (Probe4Error(DirectoryEntry) || !DirectoryEntry || Probe4Error(DirectoryEntry->Node) || !DirectoryEntry->Node)
    {
        ErrorOut_VfsOpen(-ENOENT);
        return Error2Pointer(-ENOENT);
    }

    if (Probe4Error(DirectoryEntry->Node->Operations) || !DirectoryEntry->Node->Operations || Probe4Error(DirectoryEntry->Node->Operations->Open) || !DirectoryEntry->Node->Operations->Open)
    {
        ErrorOut_VfsOpen(-ENOSYS);
        return Error2Pointer(-ENOSYS);
    }

    FILE* FileHandle = (FILE*)KMalloc(sizeof(FILE), Error);
    if (Probe4Error(FileHandle) || !FileHandle)
    {
        ErrorOut_VfsOpen(-ENOMEM);
        return Error2Pointer(-ENOMEM);
    }

    FileHandle->Node = DirectoryEntry->Node;
    FileHandle->Offset = 0;
    FileHandle->Flags = Flags;
    FileHandle->ReferenceCount = 1;
    FileHandle->Private = 0;

    if (DirectoryEntry->Node->Operations->Open(DirectoryEntry->Node, FileHandle, Error) != GeneralOK)
    {
        KFree(FileHandle, Error);

        ErrorOut_VfsOpen(Error->ErrorCode);
        return Error2Pointer(Error->ErrorCode);
    }

    return FileHandle;
}

int
VfsClose(FILE* FileHandle, SYSTEM_ERROR* Error)
{
    #define ErrorOut_VfsClose(Code) \
        ErrorOut(Error, Code, FUNC_VfsClose)

    if (Probe4Error(FileHandle) || !FileHandle)
    {
        ErrorOut_VfsClose(-EINVAL);
        return Error->ErrorCode;
    }

    if (FileHandle->Node && FileHandle->Node->Operations && FileHandle->Node->Operations->Close)
    {
        FileHandle->Node->Operations->Close(FileHandle, Error);
    }

    KFree(FileHandle, Error);
    
    return GeneralOK;
}

long
VfsRead(FILE* FileHandle, void* OutputBuffer, long BufferLength, SYSTEM_ERROR* Error)
{
    #define ErrorOut_VfsRead(Code) \
        ErrorOut(Error, Code, FUNC_VfsRead)
    
    if (Probe4Error(FileHandle) || !FileHandle || Probe4Error(OutputBuffer) || !OutputBuffer || BufferLength <= 0)
    {
        ErrorOut_VfsRead(-EINVAL);
        return Error->ErrorCode;
    }

    if (Probe4Error(FileHandle->Node) || !FileHandle->Node || Probe4Error(FileHandle->Node->Operations) || !FileHandle->Node->Operations || Probe4Error(FileHandle->Node->Operations->Read) || !FileHandle->Node->Operations->Read)
    {
        ErrorOut_VfsRead(-ENOSYS);
        return Error->ErrorCode;
    }

    long BytesRead = FileHandle->Node->Operations->Read(FileHandle, OutputBuffer, BufferLength, Error);
    if (BytesRead > 0)
    {
        FileHandle->Offset += BytesRead;
    }
    
    return BytesRead;
}

long
VfsWrite(FILE* FileHandle, const void* InputBuffer, long BufferLength, SYSTEM_ERROR* Error)
{
    #define ErrorOut_VfsWrite(Code) \
        ErrorOut(Error, Code, FUNC_VfsWrite)
    
    if (Probe4Error(FileHandle) || !FileHandle || Probe4Error(InputBuffer) || !InputBuffer || BufferLength <= 0)
    {
        ErrorOut_VfsWrite(-EINVAL);
        return Error->ErrorCode;
    }

    if (Probe4Error(FileHandle->Node) || !FileHandle->Node || Probe4Error(FileHandle->Node->Operations) || !FileHandle->Node->Operations || Probe4Error(FileHandle->Node->Operations->Write) || !FileHandle->Node->Operations->Write)
    {
        ErrorOut_VfsWrite(-ENOSYS);
        return Error->ErrorCode;
    }

    long BytesWritten = FileHandle->Node->Operations->Write(FileHandle, InputBuffer, BufferLength, Error);
    if (BytesWritten > 0)
    {
        FileHandle->Offset += BytesWritten;
    }
    
    return BytesWritten;
}

FILE*
VfsOpenAt(DIRECTORY_ENTRY* BaseDirectory, const char* ResolvePath, long Flags, SYSTEM_ERROR* Error)
{
    #define ErrorOut_VfsOpenAt(Code) \
        ErrorOut(Error, Code, FUNC_VfsOpenAt)
    
    DIRECTORY_ENTRY* DirectoryEntry = VfsResolveAt(BaseDirectory, ResolvePath, Error);
    if (Probe4Error(DirectoryEntry) || !DirectoryEntry || Probe4Error(DirectoryEntry->Node) || !DirectoryEntry->Node)
    {
        ErrorOut_VfsOpenAt(-ENOENT);
        return Error2Pointer(-ENOENT);
    }

    if (Probe4Error(DirectoryEntry->Node->Operations) || !DirectoryEntry->Node->Operations || Probe4Error(DirectoryEntry->Node->Operations->Open) ||
        !DirectoryEntry->Node->Operations->Open)
    {
        ErrorOut_VfsOpenAt(-ENOSYS);
        return Error2Pointer(-ENOSYS);
    }

    FILE* FileHandle = (FILE*)KMalloc(sizeof(FILE), Error);
    if (Probe4Error(FileHandle) || !FileHandle)
    {
        ErrorOut_VfsOpenAt(-ENOMEM);
        return Error2Pointer(-ENOMEM);
    }

    FileHandle->Node = DirectoryEntry->Node;
    FileHandle->Offset = 0;
    FileHandle->Flags = Flags;
    FileHandle->ReferenceCount = 1;
    FileHandle->Private = 0;

    if (DirectoryEntry->Node->Operations->Open(DirectoryEntry->Node, FileHandle, Error) != GeneralOK)
    {
        KFree(FileHandle, Error);

        ErrorOut_VfsOpenAt(Error->ErrorCode);
        return Error2Pointer(Error->ErrorCode);
    }

    return FileHandle;
}

long
VfsLseek(FILE* FileHandle, long Offset, int Whence, SYSTEM_ERROR* Error)
{
    #define ErrorOut_VfsLseek(Code) \
        ErrorOut(Error, Code, FUNC_VfsLseek)
    
    if (Probe4Error(FileHandle) || !FileHandle)
    {
        ErrorOut_VfsLseek(-EINVAL);
        return Error->ErrorCode;
    }

    if (Probe4Error(FileHandle->Node) || !FileHandle->Node || Probe4Error(FileHandle->Node->Operations) || !FileHandle->Node->Operations || Probe4Error(FileHandle->Node->Operations->Lseek) || !FileHandle->Node->Operations->Lseek)
    {
        ErrorOut_VfsLseek(-ENOSYS);
        return Error->ErrorCode;
    }

    long NewOffset = FileHandle->Node->Operations->Lseek(FileHandle, Offset, Whence, Error);
    if (NewOffset >= 0)
    {
        FileHandle->Offset = NewOffset;
    }
    
    return NewOffset;
}

int
VfsIoctl(FILE* FileHandle, unsigned long Command, void* Argument, SYSTEM_ERROR* Error)
{
    #define ErrorOut_VfsIoctl(Code) \
        ErrorOut(Error, Code, FUNC_VfsIoctl)
    
    if (Probe4Error(FileHandle) || !FileHandle)
    {
        ErrorOut_VfsIoctl(-EINVAL);
        return Error->ErrorCode;
    }

    if (Probe4Error(FileHandle->Node) || !FileHandle->Node || Probe4Error(FileHandle->Node->Operations) || !FileHandle->Node->Operations || Probe4Error(FileHandle->Node->Operations->Ioctl) || !FileHandle->Node->Operations->Ioctl)
    {
        ErrorOut_VfsIoctl(-ENOSYS);
        return Error->ErrorCode;
    }

    return FileHandle->Node->Operations->Ioctl(FileHandle, Command, Argument, Error);
}