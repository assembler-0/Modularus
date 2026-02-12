#include <__KCONF.h>
#include <Errors.h>
#include <KExports.h>

ERROR_KEYS_REGISTRY ErrorKeysRegistry=
{
    .Head = NULL,
    .Tail = NULL,
    .NextModuleBase = MaxTraceback
};

int RegisterErrorKeys(const char* ModuleName, const char* (*TraceMapper)(int), int TracebackCount, SYSTEM_ERROR* Error)
{
    #define ErrorOut_RegisterErrorContext(Code) \
        ErrorOut(Error, Code, FUNC_RegisterErrorKeys);

    /*Well it's statically allocated for now, also thinking so that this gets
      Rellocated to a real heap or default inbuilt heap, TODO: make a inbuilt
      OR module heap*/
    static ERROR_KEYS Contexts[MaxModules];
    static int ContextCount = 0;
    
    if(ContextCount >= MaxModules)
    {
        ErrorOut_RegisterErrorContext(-Limits);
        return Error->ErrorCode;
    }
    
    ERROR_KEYS* Context = &Contexts[ContextCount++];
    Context->ModuleName = ModuleName;
    Context->TracebackBase = ErrorKeysRegistry.NextModuleBase;
    Context->TracebackCount = TracebackCount;
    Context->TraceMapper = TraceMapper;
    Context->Next = NULL;

    if(!ErrorKeysRegistry.Head)
    {
        ErrorKeysRegistry.Head = Context;
        ErrorKeysRegistry.Tail = Context;
    }
    else
    {
        ErrorKeysRegistry.Tail->Next = Context;
        ErrorKeysRegistry.Tail = Context;
    }
    
    int BaseID = ErrorKeysRegistry.NextModuleBase;
    ErrorKeysRegistry.NextModuleBase += TracebackCount;
    
    return BaseID;
}

/*Both module and core*/
const char* TraceError(SYSTEM_ERROR* Error)
{
    int TraceID = Error->TraceBack;
    
    /* Core traceback */
    if(TraceID < MaxTraceback)
    {
        return CORE_TraceError(Error);
    }
    
    /* Module traceback */
    ERROR_KEYS* Context = ErrorKeysRegistry.Head;
    while(Context)
    {
        if(TraceID >= Context->TracebackBase && TraceID < (Context->TracebackBase + Context->TracebackCount))
        {
            int LocalID = TraceID - Context->TracebackBase;
            return Context->TraceMapper(LocalID);
        }
        Context = Context->Next;
    }
    
    return "Unknown Module Traceback";
}

/*Exporting here to break a circular dependency, Normally would be inside a declaring header*/
KEXPORT(TraceError)
KEXPORT(RegisterErrorKeys)