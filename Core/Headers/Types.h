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

/*Tools*/

/*
    Fetched from: https://github.com/assembler-0/AeroSync/blob/main/include/linux/container_of.h
    Thanks alot: https://github.com/assembler-0
*/

#define typeof_member(T, m)    typeof(((T*)0)->m)
/**
 * container_of - cast a member of a structure out to the containing structure
 * @ptr:    the pointer to the member.
 * @type:    the type of the container struct this is embedded in.
 * @member:    the name of the member within the struct.
 *
 * WARNING: any const qualifier of @ptr is lost.
 * Do not use container_of() in new code.
 */
#define container_of(ptr, type, member) ({               \
  const typeof( ((type *)0)->member ) *__mptr = (ptr);    \
  (type *)( (char *)__mptr - offsetof(type,member) );})

/**
 * container_of_const - cast a member of a structure out to the containing
 *            structure and preserve the const-ness of the pointer
 * @ptr:        the pointer to the member
 * @type:        the type of the container struct this is embedded in.
 * @member:        the name of the member within the struct.
 *
 * Always prefer container_of_const() instead of container_of() in new code.
 */
#define container_of_const(ptr, type, member)                \
    _Generic(ptr,                            \
        const typeof(*(ptr)) *: ((const type *)container_of(ptr, type, member)),\
        default: ((type *)container_of(ptr, type, member))    \
    )