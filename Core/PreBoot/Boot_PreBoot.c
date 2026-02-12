#include <__KCONF.h>
#include <Boot/PreBoot.h>

BOOT_REQUEST RequestBootService(REQUEST_TYPE Type, SYSTEM_ERROR* Error)
{
    #ifdef BOOTVENDOR_Limine
        BOOT_REQUEST Request=
        {
            .Revision = LimineRevision,
            .Pointer  = Limine_REQUEST(Type, Error)
        };

        switch(Type)
        {
            case RequestHHDM:
            {
                Request.ID = RequestHHDM;
                break;
            }        
            case RequestMEMORYMAP:
            {
                Request.ID = RequestMEMORYMAP;
                break;
            }
            case RequestFRAMEBUFFER:
            {
                Request.ID = RequestFRAMEBUFFER;
                break;
            }
            case RequestSMP:
            {
                Request.ID = RequestSMP;
                break;
            }
            case RequestMODULE:
            {
                Request.ID = RequestMODULE;
                break;
            }
            default:
            {
                Request.ID = 0; /*Unknown*/
            }
        }
        return Request;
    #endif

}