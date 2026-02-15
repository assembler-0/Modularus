#include <__KCONF.h>
#include <Errors.h>

int
IsSeperator(char Char, SYSTEM_ERROR* Error __unused)
{
    return Char == '/';
}

const char*
SkipSeperator(const char* Path, SYSTEM_ERROR* Error)
{
    while (Path && IsSeperator(*Path, Error))
    {
        Path++;
    }

    return Path;
}

long
NextCompare(const char* Path, char* Output, long Capacity, SYSTEM_ERROR* Error)
{
    if (Probe4Error(Path) || !Path || !*Path)
    {   
        ErrorOut(Error, -EINVAL, General);
        return Error->ErrorCode;
    }

    const char* PathComponent = Path;
    long Index = 0;
    while (*PathComponent && !IsSeperator(*PathComponent, Error))
    {
        if (Index + 1 < Capacity)
        {
            Output[Index++] = *PathComponent;
        }

        PathComponent++;
    }

    Output[Index] = 0;

    return Index;
}