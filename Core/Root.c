#include <__KCONF.h>

#include <Defaults/Defaults.h>

SYSTEM_ERROR Err;
SYSTEM_ERROR* Error = &Err;
    
void KernelBoot(void)
{
    LoadDefaults(Error);
    
    for(;;)
    {
        __asm__("hlt");
    }
}