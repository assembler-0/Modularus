#pragma once

#include <__KCONF.h>
#include <Boot/PreBoot.h>

#ifdef BOOTVENDOR_Limine
    #include <limine.h>
#endif

/*Requests*/
#ifdef BOOTVENDOR_Limine
    
    #define LimineRevision 0
    extern volatile struct limine_hhdm_request HHDM;
    extern volatile struct limine_memmap_request MEMORYMAP;
    extern volatile struct limine_framebuffer_request FRAMEBUFFER;
    extern volatile struct limine_smp_request SMP;
    extern volatile struct limine_module_request MODULE;

#endif