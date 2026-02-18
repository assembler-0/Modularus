#pragma once

#include <CTools.h>
#include <Types.h>
#include <KernelCLibrary.h>

/*========================================*/

/*
    PreBoot compile time options
*/
/*|_>*/#define BOOTVENDOR_Limine

/*========================================*/

/*
    Types source
*/
/*|_>*/#define StandardLIBC

/*========================================*/

/*
    Limits
*/
/*|_>*/#define MaxModules 512

/*========================================*/

/*
    Toggles
*/
/*|_>*/ #define BUILTINS
/*|_>*/ #define TESTING
/*|*/
/*|_>*/ /*BuiltIns Toggles*/
/*|_>*/ #define BUILTIN_UART
/*|_>*/ #define BUILTIN_Formatter
/*|_>*/ #define BUILTIN_Loader
/*|_>*/ #define BUILTIN_Linker

/*========================================*/
