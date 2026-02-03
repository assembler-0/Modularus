#pragma once

#include <__KCONF.h>
#include <Errors.h>
#include <Modules/KExports.h>

#ifdef BOOTVENDOR_Limine
    #include <Boot/Vendors/BOOTVENDOR_Limine.h>
#endif

/*PreBoot layer*/
typedef
enum
{
    RequestHHDM = 1,
    RequestMEMORYMAP,
    RequestFRAMEBUFFER,
    RequestSMP,
    RequestMODULE
} REQUEST_TYPE;

typedef
struct
{
    uint64_t ID;
    uint32_t Revision;
    void* Pointer;
} BOOT_REQUEST;

/*Vendor services*/
void* Limine_REQUEST(REQUEST_TYPE, SYSTEM_ERROR*);

/*Main*/
BOOT_REQUEST RequestBootService(REQUEST_TYPE, SYSTEM_ERROR*);

/*Api*/
KEXPORT(RequestBootService)