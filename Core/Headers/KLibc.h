#pragma once
#include <__KCONF.h>

/*string.h*/ /*totally not stolen from: https://github.com/VOXIDEVOSTRO/AxeialOS/blob/main/Kernel/KrnlLibs/Includes/String.h*/
void   strcpy(char* __Dest__, const char* __Src__, uint32_t __MaxLen__);
void*  memcpy(void* __Dest__, const void* __Src__, size_t __Size__);    
void*  memset(void* __Dest__, int __Value__, size_t __Index__);         
int    strcmp(const char* __Str1__, const char* __Str2__);              
int    strncmp(const char* __S1__, const char* __S2__, size_t __Size__);
size_t strlen(const char* __Str__);                                     
char*  strrchr(const char* s, int c);                                   
char*  strchr(const char* __Str__, int __Idx__);                        
long   atol(const char* __Str__);                                       
char*  strncpy(char* __Dst__, const char* __Src__, long __Idx__);       