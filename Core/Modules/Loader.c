#include <__KCONF.h>
#include <Modules/Loader.h>

LOADER* ActiveLoader = NULL;

int RegisterLoader(LOADER* Loader, SYSTEM_ERROR* Error)
{
    #define ErrorOut_RegisterLoader(Code) \
        ErrorOut(Error, Code, FUNC_RegisterLoader);

    if(Probe4Error(Loader)             || 
       !Loader                         || 
       Probe4Error(Loader->Operations) ||
       !Loader->Operations)
    {
        ErrorOut_RegisterLoader(-BadParameters);
        return -BadParameters;
    }
    
    ActiveLoader = Loader;
    
    return GeneralOK;
}

LOADER* GetLoader(SYSTEM_ERROR* Error)
{
    #define ErrorOut_GetLoader(Code) \
        ErrorOut(Error, Code, FUNC_GetLoader);

    if(Probe4Error(ActiveLoader) || !ActiveLoader)
    {
        ErrorOut_GetLoader(-NotFound);
        return Error2Pointer(-NotFound);
    }
    
    return ActiveLoader;
}

/*Wrap-Ups for operations*/

LOADED_FILE* LoadFile(const char* Path, SYSTEM_ERROR* Error)
{
    #define ErrorOut_LoadFile(Code) \
        ErrorOut(Error, Code, FUNC_LoadFile);

    if(Probe4Error(ActiveLoader) || !ActiveLoader || !ActiveLoader->Operations->Load)
    {
        ErrorOut_LoadFile(-NotFound);
        return Error2Pointer(-NotFound);
    }

    return ActiveLoader->Operations->Load(Path, Error);
}

int UnLoadFile(LOADED_FILE* File, SYSTEM_ERROR* Error)
{
    #define ErrorOut_UnloadFile(Code) \
        ErrorOut(Error, Code, FUNC_UnLoadFile);

    if(Probe4Error(ActiveLoader) || !ActiveLoader || !ActiveLoader->Operations->Unload)
    {
        ErrorOut_UnloadFile(-NotFound);
        return -NotFound;
    }

    return ActiveLoader->Operations->Unload(File, Error);
}