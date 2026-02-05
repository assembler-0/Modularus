#include "Errors.h"
#include <__KCONF.h>

#include <Defaults/Defaults.h>

static SYSTEM_ERROR error;

void kmain(void)
{   
    LoadDefaults(&error);
    
    for(;;)
    {
        __asm__("hlt");
    }
}