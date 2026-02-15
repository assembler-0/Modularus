#include <__KCONF.h>
#include <KExports.h>

/*CTools defines them*/

#define STACK_CANARY_VALUE 0xdeadbeefcafebabe

uint64_t __stack_chk_guard = STACK_CANARY_VALUE;

__exit
__noreturn 
void __stack_chk_fail(void)
{
    for (;;)
    {
        __asm__("cli");
        __asm__("hlt");
    }
}

KEXPORT(__stack_chk_fail)