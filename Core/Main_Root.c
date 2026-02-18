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

    /*STANDARD*/
    System_KickStart(Error/*As Root "/"*/);

    /*STANDARD*/
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

        #ifdef BUILTIN_Linker
            Linker_Init(Error); /*/linker*/
        #endif
    #endif

    #ifdef TESTING

        #ifdef BUILTIN_Formatter
            KrnPrintf("\nHello World!\n");
        #endif
        
        #ifdef BUILTIN_Loader
            FILE* LoaderFile = VFS_Open("/loader", VFS_OpenFlag_WRITEONLY, Error);
            LOADED_MODULE TestModule;
            LOADER_COMMAND_GET_ARGUMENTS TestModuleRequest =
            {
                .Name = "Test.ko",
                .Out = &TestModule
            };
            VFS_IOControl(LoaderFile, LoaderCommand_GET, &TestModuleRequest, Error);
        #endif

        #ifdef BUILTIN_Linker
            #ifdef BUILTIN_Loader
                FILE* LinkerFile = VFS_Open("/linker", VFS_OpenFlag_WRITEONLY, Error);
                VFS_IOControl(LinkerFile, LinkerCommand_LINK, TestModule.Address, Error);
            #endif
            VFS_IOControl(LinkerFile, LinkerCommand_RUN, NULL, Error);
        #endif

    #else

        /*STANDARD*/
        #ifdef BUILTIN_Loader
            FILE* LoaderFile = VFS_Open("/loader", VFS_OpenFlag_WRITEONLY, Error);
            LOADED_MODULE STANDARD_InitModule;
            LOADER_COMMAND_GET_ARGUMENTS STANDARD_InitModuleRequest =
            {
                .Name = "STANDARD_Init.ko",
                .Out = &STANDARD_InitModule
            };
            VFS_IOControl(LoaderFile, LoaderCommand_GET, &STANDARD_InitModuleRequest, Error);
        #endif

        #ifdef BUILTIN_Linker
            #ifdef BUILTIN_Loader
                FILE* LinkerFile = VFS_Open("/linker", VFS_OpenFlag_WRITEONLY, Error);
                VFS_IOControl(LinkerFile, LinkerCommand_LINK, STANDARD_InitModule.Address, Error);
            #endif
            VFS_IOControl(LinkerFile, LinkerCommand_RUN, NULL, Error);
        #endif

    #endif

    for(;;)
    {
        __asm__("hlt");
    }
}