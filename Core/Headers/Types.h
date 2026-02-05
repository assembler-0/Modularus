#pragma once
#include <__KCONF.h>

#ifdef StandardLIBC

//  #include <fenv.h>
    #include <float.h>
    #include <iso646.h>
    #include <limits.h>
    #include <stdalign.h>
    #include <stdarg.h>
    #include <stdatomic.h>
    #include <stdbool.h>
    #include <stddef.h>
    #include <stdint.h>
    #include <stdnoreturn.h>

#else

    #include "Types/__aln__.h"
    #include "Types/__args__.h"
    #include "Types/__bool__.h"
    #include "Types/__def__.h"
    #include "Types/__fenv__.h"
    #include "Types/__flt__.h"
    #include "Types/__i646__.h"
    #include "Types/__int__.h"
    #include "Types/__lims__.h"
    #include "Types/__noret__.h"

#endif