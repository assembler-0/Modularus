#include <__KCONF.h>
#include <Errors.h>

#include <VirtualFileSystem.h>
#include <System.h>/*sysfs++*/

SYSTEM_ERROR Err;
SYSTEM_ERROR* Error = &Err;
    
void KernelBoot(void)
{
    /*STANDARD*/
    VfsInit(Error);

    /*sysfs++ as root*/
    SystemInit(Error);
    SystemRoot = SystemCreateDirectory("/", Error);
    VfsMount("", "/", "sysfs++", 0, 0, Error);

    for(;;)
    {
        __asm__("hlt");
    }
}