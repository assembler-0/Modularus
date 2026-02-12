#include <KernelCLibrary.h>
#include <VirtualFileSystem.h>
#include <__KCONF.h>
#include <DirtyHeap.h>

int
VfsChmod(const char* Path, long Mode, SYSTEM_ERROR* Error)
{
    #define ErrorOut_VfsChmod(Code) \
        ErrorOut(Error, Code, FUNC_VfsChmod)

    DIRECTORY_ENTRY* DirectoryEntry = VfsResolve(Path, Error);
    if (Probe4Error(DirectoryEntry) || !DirectoryEntry || Probe4Error(DirectoryEntry->Node) || !DirectoryEntry->Node)
    {
        ErrorOut_VfsChmod(-ENOENT);
        return Error->ErrorCode;
    }

    if (Probe4Error(DirectoryEntry->Node->Operations) || !DirectoryEntry->Node->Operations || Probe4Error(DirectoryEntry->Node->Operations->Chmod) || !DirectoryEntry->Node->Operations->Chmod)
    {
        ErrorOut_VfsChmod(-ENOSYS);
        return Error->ErrorCode;
    }
    
    return DirectoryEntry->Node->Operations->Chmod(DirectoryEntry->Node, Mode, Error);
}

int
VfsChown(const char* Path, long UserID, long GroupID, SYSTEM_ERROR* Error)
{
    #define ErrorOut_VfsChown(Code) \
        ErrorOut(Error, Code, FUNC_VfsChown)
    
    DIRECTORY_ENTRY* DirectoryEntry = VfsResolve(Path, Error);
    if (Probe4Error(DirectoryEntry) || !DirectoryEntry || Probe4Error(DirectoryEntry->Node) || !DirectoryEntry->Node)
    {
        ErrorOut_VfsChown(-ENOENT);
        return Error->ErrorCode;
    }

    if (Probe4Error(DirectoryEntry->Node->Operations) || !DirectoryEntry->Node->Operations || Probe4Error(DirectoryEntry->Node->Operations->Chown) || !DirectoryEntry->Node->Operations->Chown)
    {
        ErrorOut_VfsChown(-ENOSYS);
        return Error->ErrorCode;
    }
    
    return DirectoryEntry->Node->Operations->Chown(DirectoryEntry->Node, UserID, GroupID, Error);
}
