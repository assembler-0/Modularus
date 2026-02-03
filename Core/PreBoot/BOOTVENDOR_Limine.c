#include <__KCONF.h>
#include <Boot/PreBoot.h>

#define Requested __attribute__((used, section(".requests")))

#ifdef BOOTVENDOR_Limine
    /*Services*/
    Requested
    volatile struct limine_hhdm_request HHDM=
    {
        .id = LIMINE_HHDM_REQUEST, .revision = LimineRevision
    };  
    Requested
    volatile struct limine_memmap_request MEMORYMAP=
    {
        .id = LIMINE_MEMMAP_REQUEST, .revision = LimineRevision
    };  
    Requested
    volatile struct limine_framebuffer_request FRAMEBUFFER=
    {
        .id = LIMINE_FRAMEBUFFER_REQUEST, .revision = LimineRevision
    };  
    Requested
    volatile struct limine_smp_request SMP=
    {
        .id = LIMINE_SMP_REQUEST, .revision = LimineRevision
    };  
    Requested
    volatile struct limine_module_request MODULE=
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
            return (void*)&HHDM;
        }
        case RequestMEMORYMAP:
        {
            return (void*)&MEMORYMAP;
        }
        case RequestFRAMEBUFFER:
        {
            return (void*)&FRAMEBUFFER;
        }
        case RequestSMP:
        {
            return (void*)&SMP;
        }
        case RequestMODULE:
        {
            return (void*)&MODULE;
        }
        default:
        {
            ErrorOut(Error, -BadRequest, FUNC_Limine_REQUEST);
            return Error2Pointer(-BadRequest);
        }
    }
}