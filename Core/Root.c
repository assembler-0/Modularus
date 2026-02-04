#include <__KCONF.h>

#include <Defaults/Defaults.h>

void _start(void)
{
    SYSTEM_ERROR Err;
    SYSTEM_ERROR* Error = &Err;
    
    LoadDefaults(Error);

    for(;;)
    {
        __asm__("hlt");
    }
}