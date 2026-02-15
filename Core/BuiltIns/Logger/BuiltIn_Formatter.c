#include <__KCONF.h>
#include <BuiltIns/Logger/Formatter.h>
#include <BuiltIns/Logger/Emitter.h>

void
KrnPrintf(const char* Format, ...)
{
    __builtin_va_list Arguments;
    __builtin_va_start(Arguments, Format);

    while (*Format)
    {
        if (*Format == '%')
        {
            Format++;
            ProcessFormatSpecifier(&Format, &Arguments);
        }
        else
        {
            PutChar(*Format);
            Format++;
        }
    }

    __builtin_va_end(Arguments);
}

/*

void
KrnPrintfColor(uint32_t __FG__, uint32_t __BG__, const char* Format, ...)
{
    
    
    uint32_t OldFG = Console.TXColor;
    uint32_t OldBG = Console.BGColor;

    SetBGColor(__FG__, __BG__);

    __builtin_va_list Arguments;
    __builtin_va_start(Arguments, Format);

    while (*Format)
    {
        if (*Format == '%')
        {
            Format++;
            ProcessFormatSpecifier(&Format, &Arguments);
        }
        else
        {
            PutChar(*Format);
            Format++;
        }
    }

    __builtin_va_end(Arguments);
    SetBGColor(OldFG, OldBG);
    R
}

*/

void
ProcessFormatSpecifier(const char** Format, __builtin_va_list* Arguments)
{
    FORMATTER_FLAGS Flags = {0};

    /* Parse flags: -, +, space, #, 0 */
    while (1)
    {
        switch (**Format)
        {
            case '-':
                Flags.LeftAlign = 1;
                (*Format)++;
                continue;
            case '+':
                Flags.ShowSign = 1;
                (*Format)++;
                continue;
            case ' ':
                Flags.SpacePrefix = 1;
                (*Format)++;
                continue;
            case '#':
                Flags.AlternateForm = 1;
                (*Format)++;
                continue;
            case '0':
                Flags.ZeroPad = 1;
                (*Format)++;
                continue;
            default:
                break;
        }
        break;
    }

    /* Parse width: either number or * for argument */
    if (**Format == '*')
    {
        Flags.Width = __builtin_va_arg(*Arguments, int);
        (*Format)++;
    }
    else
    {
        while (**Format >= '0' && **Format <= '9')
        {
            Flags.Width = Flags.Width * 10 + (**Format - '0');
            (*Format)++;
        }
    }

    /* Parse precision: . followed by number or * */
    if (**Format == '.')
    {
        Flags.HasPrecision = 1;
        (*Format)++;
        if (**Format == '*')
        {
            Flags.Precision = __builtin_va_arg(*Arguments, int);
            (*Format)++;
        }
        else
        {
            while (**Format >= '0' && **Format <= '9')
            {
                Flags.Precision = Flags.Precision * 10 + (**Format - '0');
                (*Format)++;
            }
        }
    }

    /* Parse length modifiers: hh, h, l, ll, z, t, J */
    switch (**Format)
    {
        case 'l':
            (*Format)++;
            if (**Format == 'l')
            {
                Flags.Length = 2; /* ll */
                (*Format)++;
            }
            else
            {
                Flags.Length = 1; /* l */
            }
            break;
        case 'h':
            (*Format)++;
            if (**Format == 'h')
            {
                Flags.Length = -2; /* hh */
                (*Format)++;
            }
            else
            {
                Flags.Length = -1; /* h */
            }
            break;
        case 'z':
            Flags.Length = 3;
            (*Format)++;
            break;
        case 't':
            Flags.Length = 4;
            (*Format)++;
            break;
        case 'J':
            Flags.Length = 5;
            (*Format)++;
            break;
    }

    /* Process the conversion specifier */
    switch (**Format)
    {
        case 'd':
        case 'I':
            ProcessInteger(Arguments, &Flags, 10, 1);
            break;
        case 'u':
            ProcessInteger(Arguments, &Flags, 10, 0);
            break;
        case 'x':
            ProcessInteger(Arguments, &Flags, 16, 0);
            break;
        case 'X':
            Flags.Length |= 0x80; /* Uppercase flag */
            ProcessInteger(Arguments, &Flags, 16, 0);
            break;
        case 'o':
            ProcessInteger(Arguments, &Flags, 8, 0);
            break;
        case 'b':
            ProcessInteger(Arguments, &Flags, 2, 0);
            break;
        case 's':
            ProcessString(Arguments, &Flags);
            break;
        case 'c':
            ProcessChar(Arguments, &Flags);
            break;
        case 'p':
            ProcessPointer(Arguments, &Flags);
            break;
        case 'N':
            /* Not implemented for security reasons */
            break;
        case 'f':
        case 'F':
        case 'e':
        case 'E':
        case 'g':
        case 'G':
            /* Floating point not implemented in kernel */
            PutString("(float)");
            break;
        case '%':
            PutChar('%');
            break;
        default:
            /* Unknown specifier - print as-is */
            PutChar('%');
            PutChar(**Format);
            break;
    }
    (*Format)++;
}