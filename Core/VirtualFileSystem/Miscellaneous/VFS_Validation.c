#include <KernelCLibrary.h>
#include <VirtualFileSystem.h>
#include <__KCONF.h>
#include <DirtyHeap.h>

int
VFS_Access(const char* Path, long Mode __unused, SYSTEM_ERROR* Error)
{
    #define ErrorOut_VFS_Access(Code) \
        ErrorOut(Error, Code, FUNC_VFS_Access)

    DIRECTORY_ENTRY* DirectoryEntry = VFS_Resolve(Path, Error);
    if (Probe4Error(DirectoryEntry) || !DirectoryEntry)
    {
        ErrorOut_VFS_Access(-ENOENT);
        return Error->ErrorCode;
    }

    return GeneralOK;
}

int
VFS_Exists(const char* Path, SYSTEM_ERROR* Error)
{
    #define ErrorOut_VFS_Exists(Code) \
        ErrorOut(Error, Code, FUNC_VFS_Exists)

    DIRECTORY_ENTRY* DirectoryEntry = VFS_Resolve(Path, Error);
    if (Probe4Error(DirectoryEntry) || !DirectoryEntry)
    {
        ErrorOut_VFS_Exists(-ENOENT);
        return Error->ErrorCode;
    }

    return GeneralOK;
}

int
VFS_IsDirectory(const char* Path, SYSTEM_ERROR* Error)
{
    #define ErrorOut_VFS_IsDirectory(Code) \
        ErrorOut(Error, Code, FUNC_VFS_IsDirectory)

    DIRECTORY_ENTRY* DirectoryEntry = VFS_Resolve(Path, Error);
    if (Probe4Error(DirectoryEntry) || !DirectoryEntry || Probe4Error(DirectoryEntry->Node) || !DirectoryEntry->Node)
    {
        ErrorOut_VFS_IsDirectory(-ENOENT);
        return Error->ErrorCode;
    }

    if(DirectoryEntry->Node->Type == VFSNode_DIRECTORY)
    {
        return GeneralOK;
    }
    else
    {
        ErrorOut_VFS_IsDirectory(-ENOENT);
        return Error->ErrorCode;
    }   
}

int
VFS_IsFile(const char* Path, SYSTEM_ERROR* Error)
{
    #define ErrorOut_VFS_IsFile(Code) \
        ErrorOut(Error, Code, FUNC_VFS_IsFile)

    DIRECTORY_ENTRY* DirectoryEntry = VFS_Resolve(Path, Error);
    if (Probe4Error(DirectoryEntry) || !DirectoryEntry || Probe4Error(DirectoryEntry->Node) || !DirectoryEntry->Node)
    {
        ErrorOut_VFS_IsFile(-ENOENT);
        return Error->ErrorCode;
    }

    if(DirectoryEntry->Node->Type == VFSNode_FILE)
    {
        return GeneralOK;
    }
    else
    {
        ErrorOut_VFS_IsFile(-ENOENT);
        return Error->ErrorCode;
    }
}

int
VFS_IsSymbolLink(const char* Path, SYSTEM_ERROR* Error)
{
    #define ErrorOut_VFS_IsSymbolLink(Code) \
        ErrorOut(Error, Code, FUNC_VFS_IsSymbolLink)

    DIRECTORY_ENTRY* DirectoryEntry = VFS_Resolve(Path, Error);
    if (Probe4Error(DirectoryEntry) || !DirectoryEntry || Probe4Error(DirectoryEntry->Node) || !DirectoryEntry->Node)
    {
        ErrorOut_VFS_IsSymbolLink(-ENOENT);
        return Error->ErrorCode;
    }

    if (DirectoryEntry->Node->Type == VFSNode_SYMBOL)
    {
        return GeneralOK;
    }
    else
    {
        ErrorOut_VFS_IsSymbolLink(-ENOENT);
        return Error->ErrorCode;
    }
}
