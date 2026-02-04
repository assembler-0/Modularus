#include "Boot/Vendors/Limine.h"
#include <__KCONF.h>
#include <Boot/PreBoot.h>

#define __limine_requests __attribute__((used, section(".requests")))
#define __limine_requests_start __attribute__((used, section(".requests_start")))
#define __limine_requests_end __attribute__((used, section(".requests_end")))

#ifdef BOOTVENDOR_Limine
    __limine_requests
    volatile uint64_t limineBaseRevision[] = LIMINE_BASE_REVISION(4);

    __limine_requests_start
    volatile uint64_t limineRequestsStartMarker[] = LIMINE_REQUESTS_START_MARKER;

    __limine_requests_end
    volatile uint64_t LimineRequestsEndMarker[] = LIMINE_REQUESTS_END_MARKER;

    /* Services */
    __limine_requests
    volatile struct limine_hhdm_request ReqHHDM=
    {
        .id = LIMINE_HHDM_REQUEST_ID, .revision = LimineRevision
    };  
    
    __limine_requests
    volatile struct limine_memmap_request ReqMEMORYMAP=
    {
        .id = LIMINE_MEMMAP_REQUEST_ID, .revision = LimineRevision
    };  
    
    __limine_requests
    volatile struct limine_framebuffer_request ReqFRAMEBUFFER=
    {
        .id = LIMINE_FRAMEBUFFER_REQUEST_ID, .revision = LimineRevision
    };

    __limine_requests
    volatile struct limine_mp_request ReqSMP=
    {
        .id = LIMINE_MP_REQUEST_ID, .revision = LimineRevision
    };

    __limine_requests
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
                return (void*)&ReqSMP;
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