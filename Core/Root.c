#include <Types.h>
#include <__KCONF.h>
#include <Errors.h>
#include <Boot/PreBoot.h>

void _start(void)
{
    for(;;)
    {
        __asm__("hlt");
    }
}