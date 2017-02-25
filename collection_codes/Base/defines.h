//
//  defines.h
//  collection_codes
//
//  Created by hejunqiu on 2017/2/24.
//  Copyright © 2017年 CHE. All rights reserved.
//

#ifndef defines_h
#define defines_h

#include <type_traits>

#ifndef NESPEC
#define NESPEC collection_codes
#endif // !NESPEC

#ifndef CC_BEGIN
#define CC_BEGIN namespace NESPEC {
#endif // !CC_BEGIN

#ifndef CC_END
#define CC_END }
#endif // !CC_END

#ifndef _CC
#define _CC ::NESPEC
#define _cc _CC
#endif // !_CC

CC_BEGIN

#ifndef CLASS_TAGGEDPOINTER_AVAILABLE
#define CLASS_TAGGEDPOINTER_AVAILABLE
#endif // !CLASS_TAGGEDPOINTER_AVAILABLE

#if defined(__cplusplus)
# define CC_EXTERN extern "C"
#else
# define CC_EXTERN extern
#endif

#ifdef assert
#undef assert
#endif // assert

#ifdef DEBUG
# define ____fn____ __PRETTY_FUNCTION__
# define assert(cond, desc, ...) \
    do {\
        if (!(cond)) {\
            fprintf(stderr, "*** Assertion failure in [%s], %s:%d => ('%s')", ____fn____, __FILE__, __LINE__, #cond);\
            if (*desc) {\
                putchar(','), putchar(' ');\
                fprintf(stderr, desc, ##__VA_ARGS__);\
            }\
            putchar('\n');\
            abort();\
        }\
    } while (0)

# define parameterAssert(cond) assert((cond), "Invalid parameter not satisfying: %s", #cond)
#else
# define assert(cond, desc, ...)
# define parameterAssert(c)
#endif // DEBUG

#ifndef __PL64__
# ifdef WIN32
#  ifdef _WIN64
#   define __PL64__ 1
#  else
#   define __PL64__ 0
#  endif // _WIN64
# endif // WIN32

# ifdef __clang__
#  define __PL64__ __LP64__
# endif // __clang__
#endif // !__PL64__

using uint32_t = unsigned int;
using uint16_t = unsigned short;
using uint64_t = unsigned long long;
using byte = unsigned char;

#if __PL64__
using IMP = unsigned long long;
using uintptr_t = unsigned long;

using integer = long long;
using uinteger = unsigned long long;
#else
using IMP = unsigned int;
using uintptr_t = unsigned int;

using integer = int;
using uinteger = unsigned int;

#endif // __PL64__

const constexpr integer NotFound = (integer)-1;

#define ARRAY_CONTAINS_TYPE(objType, containsType) objType
#define ARRAY_CONTAINS(type) Array *

#if defined(__clang__)
#define __printflike__ __printflike
#else
#define __printflike__(...)
#endif

CC_END

#endif /* defines_h */
