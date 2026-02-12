#include <__KCONF.h>
#include <Boot/PreBoot.h>

#define Requested __attribute__((used, section(".requests")))

#ifdef BOOTVENDOR_Limine
    /*Services*/
    Requested
    volatile struct limine_hhdm_request ReqHHDM=
    {
        .id = LIMINE_HHDM_REQUEST_ID, .revision = LimineRevision
    };  
    Requested
    volatile struct limine_memmap_request ReqMEMORYMAP=
    {
        .id = LIMINE_MEMMAP_REQUEST_ID, .revision = LimineRevision
    };  
    Requested
    volatile struct limine_framebuffer_request ReqFRAMEBUFFER=
    {
        .id = LIMINE_FRAMEBUFFER_REQUEST_ID, .revision = LimineRevision
    };  
    Requested
    volatile struct limine_mp_request ReqMP=
    {
        .id = LIMINE_MP_REQUEST_ID, .revision = LimineRevision
    };  
    Requested
    volatile struct limine_module_request ReqMODULE=
    {
        .id = LIMINE_MODULE_REQUEST_ID, .revision = LimineRevision
    };

    /*Request*/
    void* Limine_REQUEST(REQUEST_TYPE Type, SYSTEM_ERROR* Error)
    {
        switch (Type)
        {
            case RequestHHDM:
            {
                return (void*)&ReqHHDM;
            }
            case RequestMEMORYMAP:
            {
                return (void*)&ReqMEMORYMAP;
            }
            case RequestFRAMEBUFFER:
            {
                return (void*)&ReqFRAMEBUFFER;
            }
            case RequestSMP:
            {
                return (void*)&ReqMP;
            }
            case RequestMODULE:
            {
                return (void*)&ReqMODULE;
            }
            default:
            {
                ErrorOut(Error, -BadRequest, FUNC_Limine_REQUEST);
                return Error2Pointer(-BadRequest);
            }
        }
    }
#endif