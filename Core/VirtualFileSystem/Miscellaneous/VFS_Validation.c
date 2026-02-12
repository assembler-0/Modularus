#include <KernelCLibrary.h>
#include <VirtualFileSystem.h>
#include <__KCONF.h>
#include <DirtyHeap.h>

int
VfsAccess(const char* Path, long Mode __UNUSED, SYSTEM_ERROR* Error)
{
    #define ErrorOut_VfsAccess(Code) \
        ErrorOut(Error, Code, FUNC_VfsAccess)

    DIRECTORY_ENTRY* DirectoryEntry = VfsResolve(Path, Error);
    if (Probe4Error(DirectoryEntry) || !DirectoryEntry)
    {
        ErrorOut_VfsAccess(-ENOENT);
        return Error->ErrorCode;
    }

    return GeneralOK;
}

int
VfsExists(const char* Path, SYSTEM_ERROR* Error)
{
    #define ErrorOut_VfsExists(Code) \
        ErrorOut(Error, Code, FUNC_VfsExists)

    DIRECTORY_ENTRY* DirectoryEntry = VfsResolve(Path, Error);
    if (Probe4Error(DirectoryEntry) || !DirectoryEntry)
    {
        ErrorOut_VfsExists(-ENOENT);
        return Error->ErrorCode;
    }

    return GeneralOK;
}

int
VfsIsDir(const char* Path, SYSTEM_ERROR* Error)
{
    #define ErrorOut_VfsIsDir(Code) \
        ErrorOut(Error, Code, FUNC_VfsIsDir)

    DIRECTORY_ENTRY* DirectoryEntry = VfsResolve(Path, Error);
    if (Probe4Error(DirectoryEntry) || !DirectoryEntry || Probe4Error(DirectoryEntry->Node) || !DirectoryEntry->Node)
    {
        ErrorOut_VfsIsDir(-ENOENT);
        return Error->ErrorCode;
    }

    if(DirectoryEntry->Node->Type == VFSNode_DIRECTORY)
    {
        return GeneralOK;
    }
    else
    {
        ErrorOut_VfsIsDir(-ENOENT);
        return Error->ErrorCode;
    }   
}

int
VfsIsFile(const char* Path, SYSTEM_ERROR* Error)
{
    #define ErrorOut_VfsIsFile(Code) \
        ErrorOut(Error, Code, FUNC_VfsIsFile)

    DIRECTORY_ENTRY* DirectoryEntry = VfsResolve(Path, Error);
    if (Probe4Error(DirectoryEntry) || !DirectoryEntry || Probe4Error(DirectoryEntry->Node) || !DirectoryEntry->Node)
    {
        ErrorOut_VfsIsFile(-ENOENT);
        return Error->ErrorCode;
    }

    if(DirectoryEntry->Node->Type == VFSNode_FILE)
    {
        return GeneralOK;
    }
    else
    {
        ErrorOut_VfsIsFile(-ENOENT);
        return Error->ErrorCode;
    }
}

int
VfsIsSymlink(const char* Path, SYSTEM_ERROR* Error)
{
    #define ErrorOut_VfsIsSymlink(Code) \
        ErrorOut(Error, Code, FUNC_VfsIsSymlink)

    DIRECTORY_ENTRY* DirectoryEntry = VfsResolve(Path, Error);
    if (Probe4Error(DirectoryEntry) || !DirectoryEntry || Probe4Error(DirectoryEntry->Node) || !DirectoryEntry->Node)
    {
        ErrorOut_VfsIsSymlink(-ENOENT);
        return Error->ErrorCode;
    }

    if (DirectoryEntry->Node->Type == VFSNode_SYMBOL)
    {
        return GeneralOK;
    }
    else
    {
        ErrorOut_VfsIsSymlink(-ENOENT);
        return Error->ErrorCode;
    }
}
