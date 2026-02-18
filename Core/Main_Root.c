#include <__KCONF.h>
#include <Errors.h>

#include <VirtualFileSystem.h>
#include <System.h>/*sysfs++*/

#ifdef BUILTINS
    #ifdef BUILTIN_UART
        #include <BuiltIns/Device/UART.h>
    #endif

    #ifdef BUILTIN_Formatter
        #include <BuiltIns/Logger/Emitter.h>
        #include <BuiltIns/Logger/Formatter.h>
    #endif

    #ifdef BUILTIN_Linker
        #include <BuiltIns/Linker/LinkerELF.h>
    #endif

    #ifdef BUILTIN_Loader
        #include <BuiltIns/Loader/Loader.h>
    #endif
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
        #ifdef BUILTIN_UART
            UART_KickStart(Error);
        #endif

        #ifdef BUILTIN_Formatter
            Emitter_KickStart(Error);
        #endif

        #ifdef BUILTIN_Loader
            Loader_GetModules(Error); /*So the /loader can register itself*/
        #endif
    #endif

    #ifdef TESTING
        #ifdef BUILTIN_Formatter
            KrnPrintf("\nHello World!\n");
        #endif
        
        #ifdef BUILTIN_Loader
            FILE* LoaderFile = VFS_Open("/loader", VFS_OpenFlag_WRITEONLY, Error);
            LOADED_MODULE TestModule;
            VFS_IOControl(LoaderFile, LoaderCommand_GET, &TestModule, Error);
        #endif

        /*Should run Test.ko*/
        #ifdef BUILTIN_Linker
            #ifdef BUILTIN_Loader
                Module_Link(TestModule.Address, Error);
            #endif
            Module_Run(Error);
        #endif
    #endif

    for(;;)
    {
        __asm__("hlt");
    }
}