#pragma once

#include <CTools.h>
#include <Types.h>

/*PreBoot compile time options.*/
#define BOOTVENDOR_Limine

/*Types source*/
#define StandardLIBC

/*Limits*/
#define MaxDependencies 64
#define MaxModules 512
#define PMM_MaxRegions 128
#define MaxGDT 256
#define MaxIDT 256
#define MaxIST 7
#define MaxIRQHandlers 16

#define CONFIG_OPTIMIZED_STRING
#define CONFIG_LIBC_STRING_FULL
#define CONFIG_KSTRTO_ERRORS
#define CONFIG_STRING_FLOAT
#define CONFIG_STRING_PATTERN
#define CONFIG_STRING_ADVANCED
#define CONFIG_STRING_CRYPTO
