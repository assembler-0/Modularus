#include <__KCONF.h>
#include <Modules/KExports.h>
#include <KLibc.h>

void*
LookUpKExport(const char* Symbol, SYSTEM_ERROR* Error)
{
    #define ErrorOut_LookUpKExport(Code) \
        ErrorOut(Error, Code, FUNC_LookUpKExport)

    if (Probe4Error(Symbol) || !Symbol)
    {
        ErrorOut_LookUpKExport(-BadParameters);
        return Error2Pointer(-BadParameters);
    }

    /* Iterate */
    const KEXPORT* Current = __start_kexports;
    const KEXPORT* End = __stop_kexports;

    while (Current < End)
    {
        if (Current->Name && strcmp(Current->Name, Symbol) == 0)
        {
            return Current->Address;
        }

        Current++;
    }

    /* Symbol not found */
    ErrorOut_LookUpKExport(-NotFound);
    return Error2Pointer(-NotFound);
}