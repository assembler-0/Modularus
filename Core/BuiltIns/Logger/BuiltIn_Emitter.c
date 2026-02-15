#include <__KCONF.h>
#include <VirtualFileSystem.h>

static FILE* EmitterDevice;

void Emitter_KickStart(SYSTEM_ERROR* Error)
{
    EmitterDevice = VFS_Open("/uart", VFS_OpenFlag_WRITEONLY, Error);
}

void PutChar(char Character)
{
    SYSTEM_ERROR Err;
    SYSTEM_ERROR* Error = &Err;
    
    VFS_Write(EmitterDevice, &Character, 1, Error);
}

void PutString(const char* String)
{
    while (*String)
    {
        PutChar(*String);
        String++;
    }
}