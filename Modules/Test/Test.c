#include <__KCONF.h>
#include <VirtualFileSystem.h>
#include <Errors.h>

SYSTEM_ERROR Err;
SYSTEM_ERROR* Error = &Err;

size_t /*totally not stolen*/
strlen(const char* __Str__)
{
    size_t __Size__ = 0;
    while (__Str__[__Size__] != '\0')
    {
        __Size__++;
    }
    return __Size__;
}

int _start(void)
{
    FILE* Uart = VFS_Open("/uart", VFS_OpenFlag_WRITEONLY, Error);
    char String[] = "Hello World but from module!\n";
    VFS_Write(Uart, &String, strlen(String), Error);

    return 0;
}

int _exit(void)
{
    return 0;
}