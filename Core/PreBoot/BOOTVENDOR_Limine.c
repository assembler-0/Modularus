#include <__KCONF.h>
#include <Boot/PreBoot.h>

#define Requested __attribute__((used, section(".requests")))

#ifdef BOOTVENDOR_Limine
    /*Services*/
    Requested
    volatile struct limine_hhdm_request ReqHHDM=
    {
        .id = LIMINE_HHDM_REQUEST, .revision = LimineRevision
    };  
    Requested
    volatile struct limine_memmap_request ReqMEMORYMAP=
    {
        .id = LIMINE_MEMMAP_REQUEST, .revision = LimineRevision
    };  
    Requested
    volatile struct limine_framebuffer_request ReqFRAMEBUFFER=
    {
        .id = LIMINE_FRAMEBUFFER_REQUEST, .revision = LimineRevision
    };  
    Requested
    volatile struct limine_smp_request ReqSMP=
    {
        .id = LIMINE_SMP_REQUEST, .revision = LimineRevision
    };  
    Requested
    volatile struct limine_module_request ReqMODULE=
    {
        .id = LIMINE_MODULE_REQUEST, .revision = LimineRevision
    };
#endif

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