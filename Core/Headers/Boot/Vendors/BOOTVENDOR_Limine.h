#pragma once

#include <Boot/PreBoot.h>
#include <__KCONF.h>

#ifdef BOOTVENDOR_Limine
#include <Boot/Vendors/Limine.h>
#endif

/*Requests*/
#ifdef BOOTVENDOR_Limine

#define LimineRevision 0
extern volatile struct limine_hhdm_request ReqHHDM;
extern volatile struct limine_memmap_request ReqMEMORYMAP;
extern volatile struct limine_framebuffer_request ReqFRAMEBUFFER;
extern volatile struct limine_mp_request ReqSMP;
extern volatile struct limine_module_request ReqMODULE;

#endif