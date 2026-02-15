#include <__KCONF.h>
#include <Errors.h>

#include <VirtualFileSystem.h>
#include <System.h>/*sysfs++*/

#ifdef BUILTINS
    #include <BuiltIns/Device/UART.h>
    #include <BuiltIns/Logger/Emitter.h>
    #include <BuiltIns/Logger/Formatter.h>
#endif

SYSTEM_ERROR Err;
SYSTEM_ERROR* Error = &Err;
    
void KernelMain(void)
{
    /*STANDARD*/
    VFS_KickStart(Error);

    /*sysfs++ as root*/
    System_KickStart(Error);
    VFS_Mount("", "/", "sysfs++", 0, 0, Error);

    /*BuiltIns*/
    #ifdef BUILTINS
        UART_KickStart(Error);
        Emitter_KickStart(Error);
    #endif

    #ifdef TESTING
        KrnPrintf("\nHello World! But from the BuiltIn formatter!\n");
    #endif

    for(;;)
    {
        __asm__("hlt");
    }
}