#pragma once

#include <__KCONF.h>

typedef struct
{
    const char* Format;
    void**      Arguments;
    int         ArgumentIndex;
} FORMATTER_STATE;

typedef struct
{
    int LeftAlign;
    int ShowSign;
    int SpacePrefix;
    int AlternateForm;
    int ZeroPad;
    int Width;
    int Precision;
    int HasPrecision;
    int Length;
} FORMATTER_FLAGS;

#define ClrNormal    0xFFFFFF
#define ClrInvisible 0x000000
#define ClrError     0xFF0000
#define ClrSuccess   0x00FF00
#define ClrBlue      0x0000FF
#define ClrWarn      0xFFFF00
#define ClrInfo      0x00FFFF
#define ClrMagnet    0xFF00FF
#define ClrTang      0xFF8000
#define ClrDebug     0x808080

#ifdef DEBUG
#    define PDebug(Format, ...) _PDebug(Format, ##__VA_ARGS__)
#else
#    define PDebug(Format, ...)                                                                       \
        do                                                                                         \
        {                                                                                          \
        } while (0)
#endif

#ifndef AllErrors
/*Independent error reporter with cause and returned or returning error code, very useful*/
#    define PushError(Func, CallLogger, ErrorMsg, RetErrCode)                                      \
        do                                                                                         \
        {                                                                                          \
            CallLogger(                                                                            \
                "[traceback: %s] %s, errno: %s\n", (Func), (ErrorMsg), ErrName(RetErrCode));       \
        } while (0)
#else
/*Independent error reporter with cause and returned or returning error code, very useful*/
#    define PushError(Func, CallLogger, ErrorMsg, RetErrCode)                                      \
        do                                                                                         \
        {                                                                                          \
            PError("[traceback: %s] %s, errno: %s\n", (Func), (ErrorMsg), ErrName(RetErrCode));    \
        } while (0)
#endif

/*Main*/
void KrnPrintf(const char*, ...);
// void KrnPrintfColor(uint32_t, uint32_t, const char*, ...);
void ProcessFormatSpecifier(const char**, __builtin_va_list*);

/*Helpers*/
void PrintInteger(int, int, int);
void PrintUnsigned(uint32_t, int, int);
void PrintString(const char*);
void PrintChar(char);
void PrintPointer(void*);
int  StringLength(const char*);
void ReverseString(char*, int);
void IntegerToString(int, char*, int);
void UnsignedToString(uint32_t, char*, int);
void ProcessInteger(__builtin_va_list*, FORMATTER_FLAGS*, int, int);
void ProcessString(__builtin_va_list*, FORMATTER_FLAGS*);
void ProcessChar(__builtin_va_list*, FORMATTER_FLAGS*);
void ProcessPointer(__builtin_va_list* , FORMATTER_FLAGS*);
void FormatOutput(const char*, FORMATTER_FLAGS*, int, int);
void UnsignedToStringEx(uint64_t, char*, int, int);

/*Logging*/
void PError(const char*, ...);
void PWarn(const char*, ...);
void PInfo(const char*, ...);
void _PDebug(const char*, ...);
void PSuccess(const char*, ...);