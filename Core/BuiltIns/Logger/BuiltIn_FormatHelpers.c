#include <__KCONF.h>
#include <BuiltIns/Logger/Formatter.h>
#include <BuiltIns/Logger/Emitter.h>

void
ProcessInteger(__builtin_va_list* Arguments, FORMATTER_FLAGS* Flags, int Base, int Signed)
{
    char Buffer[64];
    int64_t Value = 0;
    uint64_t UnSignedValue = 0;
    int IsNegative = 0;

    /* Extract value based on length modifier and signedness */
    if (Signed)
    {
        switch (Flags->Length)
        {
            case -2:
                Value = (signed char)__builtin_va_arg(*Arguments, int);
                break;
            case -1:
                Value = (short)__builtin_va_arg(*Arguments, int);
                break;
            case 0:
                Value = __builtin_va_arg(*Arguments, int);
                break;
            case 1:
                Value = __builtin_va_arg(*Arguments, long);
                break;
            case 2:
                Value = __builtin_va_arg(*Arguments, long long);
                break;
            default:
                Value = __builtin_va_arg(*Arguments, int);
                break;
        }
        if (Value < 0)
        {
            IsNegative = 1;
            UnSignedValue     = -Value;
        }
        else
        {
            UnSignedValue = Value;
        }
    }
    else
    {
        switch (Flags->Length)
        {
            case -2:
                UnSignedValue = (unsigned char)__builtin_va_arg(*Arguments, unsigned int);
                break;
            case -1:
                UnSignedValue = (unsigned short)__builtin_va_arg(*Arguments, unsigned int);
                break;
            case 0:
                UnSignedValue = __builtin_va_arg(*Arguments, unsigned int);
                break;
            case 1:
                UnSignedValue = __builtin_va_arg(*Arguments, unsigned long);
                break;
            case 2:
                UnSignedValue = __builtin_va_arg(*Arguments, unsigned long long);
                break;
            default:
                UnSignedValue = __builtin_va_arg(*Arguments, unsigned int);
                break;
        }
    }

    /* Convert unsigned value to string in specified base */
    UnsignedToStringEx(UnSignedValue, Buffer, Base, (Flags->Length & 0x80) ? 1 : 0);

    /* Apply formatting (padding, prefixes, alignment) */
    FormatOutput(Buffer, Flags, IsNegative, Base);
}

void
ProcessString(__builtin_va_list* Arguments, FORMATTER_FLAGS* Flags)
{
    const char* String = __builtin_va_arg(*Arguments, const char*);
    if (!String)
    {
        String = "(null)";
    }

    int Length = StringLength(String);
    if (Flags->HasPrecision && Flags->Precision < Length)
    {
        Length = Flags->Precision;
    }

    /* Right-align padding */
    if (!Flags->LeftAlign && Flags->Width > Length)
    {
        for (int Index = 0; Index < Flags->Width - Length; Index++)
        {
            PutChar(' ');
        }
    }

    /* Output string characters */
    for (int Index = 0; Index < Length; Index++)
    {
        PutChar(String[Index]);
    }

    /* Left-align padding */
    if (Flags->LeftAlign && Flags->Width > Length)
    {
        for (int Index = 0; Index < Flags->Width - Length; Index++)
        {
            PutChar(' ');
        }
    }
}

void
ProcessChar(__builtin_va_list* Arguments, FORMATTER_FLAGS* Flags)
{
    char Character = (char)__builtin_va_arg(*Arguments, int);

    /* Right-align padding */
    if (!Flags->LeftAlign && Flags->Width > 1)
    {
        for (int Index = 0; Index < Flags->Width - 1; Index++)
        {
            PutChar(' ');
        }
    }

    PutChar(Character);

    /* Left-align padding */
    if (Flags->LeftAlign && Flags->Width > 1)
    {
        for (int Index = 0; Index < Flags->Width - 1; Index++)
        {
            PutChar(' ');
        }
    }
}

void
ProcessPointer(__builtin_va_list* Arguments, FORMATTER_FLAGS* Flags __unused)
{
    void* Pointer = __builtin_va_arg(*Arguments, void*);
    char Buffer[32];

    PutString("0x");
    UnsignedToStringEx((uintptr_t)Pointer, Buffer, 16, 0);
    PutString(Buffer);
}

void
FormatOutput(const char* Buffer, FORMATTER_FLAGS* Flags, int IsNegative, int Base)
{
    int Length = StringLength(Buffer);
    int PrefixLength = 0;
    char Prefix[4] = {0};

    /* Build sign/space prefix */
    if (IsNegative)
    {
        Prefix[PrefixLength++] = '-';
    }
    else if (Flags->ShowSign)
    {
        Prefix[PrefixLength++] = '+';
    }
    else if (Flags->SpacePrefix)
    {
        Prefix[PrefixLength++] = ' ';
    }

    /* Build alternate form prefix */
    if (Flags->AlternateForm)
    {
        if (Base == 16)
        {
            Prefix[PrefixLength++] = '0';
            Prefix[PrefixLength++] = 'x';
        }
        else if (Base == 8 && Buffer[0] != '0')
        {
            Prefix[PrefixLength++] = '0';
        }
    }

    int TotalLength = Length + PrefixLength;
    int PadLength   = (Flags->Width > TotalLength) ? Flags->Width - TotalLength : 0;

    /* Left padding (spaces) */
    if (!Flags->LeftAlign && !Flags->ZeroPad)
    {
        for (int Index = 0; Index < PadLength; Index++)
        {
            PutChar(' ');
        }
    }

    /* Output prefix */
    for (int Index = 0; Index < PrefixLength; Index++)
    {
        PutChar(Prefix[Index]);
    }

    /* Zero padding */
    if (!Flags->LeftAlign && Flags->ZeroPad)
    {
        for (int Index = 0; Index < PadLength; Index++)
        {
            PutChar('0');
        }
    }

    /* Output the number */
    PutString(Buffer);

    /* Right padding (spaces) */
    if (Flags->LeftAlign)
    {
        for (int Index = 0; Index < PadLength; Index++)
        {
            PutChar(' ');
        }
    }
}

void
UnsignedToStringEx(uint64_t Value, char* Buffer, int Base, int UpperCase)
{
    int Iteration = 0;
    if (Value == 0)
    {
        Buffer[Iteration++] = '0';
        Buffer[Iteration]   = '\0';
        return;
    }

    while (Value != 0)
    {
        int Remainder = Value % Base;
        if (Remainder > 9)
        {
            Buffer[Iteration++] = (Remainder - 10) + (UpperCase ? 'A' : 'a');
        }
        else
        {
            Buffer[Iteration++] = Remainder + '0';
        }
        Value = Value / Base;
    }

    Buffer[Iteration] = '\0';
    ReverseString(Buffer, Iteration);
}

void
PrintInteger(int Value, int Base, int UpperCase)
{
    char Buffer[32];
    IntegerToString(Value, Buffer, Base);

    if (UpperCase)
    {
        for (int Iteration = 0; Buffer[Iteration]; Iteration++)
        {
            if (Buffer[Iteration] >= 'a' && Buffer[Iteration] <= 'f')
            {
                Buffer[Iteration] = Buffer[Iteration] - 'a' + 'A';
            }
        }
    }

    PrintString(Buffer);
}

void
PrintUnsigned(uint32_t Value, int Base, int UpperCase)
{
    char Buffer[32];
    UnsignedToString(Value, Buffer, Base);

    if (UpperCase)
    {
        for (int Iteration = 0; Buffer[Iteration]; Iteration++)
        {
            if (Buffer[Iteration] >= 'a' && Buffer[Iteration] <= 'f')
            {
                Buffer[Iteration] = Buffer[Iteration] - 'a' + 'A';
            }
        }
    }

    PrintString(Buffer);
}

void
PrintString(const char* String)
{
    if (String == 0)
    {
        PutString("(null)");
        return;
    }
    PutString(String);
}

void
PrintChar(char Character)
{
    PutChar(Character);
}

void
PrintPointer(void* Pointer)
{
    PutString("0x");
    PrintUnsigned((uint32_t)(uintptr_t)Pointer, 16, 0);
}

int
StringLength(const char* String)
{
    int Length = 0;
    while (String[Length])
    {
        Length++;
    }
    return Length;
}

void
ReverseString(char* String, int Length)
{
    int Start = 0;
    int End = Length - 1;

    while (Start < End)
    {
        char Junk = String[Start];
        String[Start] = String[End];
        String[End] = Junk;
        Start++;
        End--;
    }
}

void
IntegerToString(int Value, char* Buffer, int Base)
{
    int Iteration  = 0;
    int IsNegative = 0;

    if (Value == 0)
    {
        Buffer[Iteration++] = '0';
        Buffer[Iteration]   = '\0';
        return;
    }

    if (Value < 0 && Base == 10)
    {
        IsNegative = 1;
        Value = -Value;
    }

    while (Value != 0)
    {
        int Remainder = Value % Base;
        Buffer[Iteration++] = (Remainder > 9) ? (Remainder - 10) + 'a' : Remainder + '0';
        Value = Value / Base;
    }

    if (IsNegative)
    {
        Buffer[Iteration++] = '-';
    }

    Buffer[Iteration] = '\0';
    ReverseString(Buffer, Iteration);
}

void
UnsignedToString(uint32_t Value, char* Buffer, int Base)
{
    int Iteration = 0;

    if (Value == 0)
    {
        Buffer[Iteration++] = '0';
        Buffer[Iteration]   = '\0';
        return;
    }

    while (Value != 0)
    {
        int Remainder = Value % Base;
        Buffer[Iteration++] = (Remainder > 9) ? (Remainder - 10) + 'a' : Remainder + '0';
        Value = Value / Base;
    }

    Buffer[Iteration] = '\0';
    ReverseString(Buffer, Iteration);
}
