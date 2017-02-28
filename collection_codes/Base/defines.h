//
//  defines.h
//  collection_codes
//
//  Created by hejunqiu on 2017/2/24.
//  Copyright © 2017年 CHE. All rights reserved.
//

#ifndef DEFINES_H
#define DEFINES_H

#ifndef NESPEC
#define NESPEC cc
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

#if defined(_MSC_VER)
# ifndef _NOEXCEPT_
# define _NOEXCEPT_ _NOEXCEPT
# endif // !_NOEXCEPT_

# ifndef __attribute__
# define __attribute__(...)
#endif // !__attribute__

# ifndef __printflike
# define __printflike(...)
# endif // !__printflike

# ifndef __unused
# define __unused
# endif // !__unused

#endif // _MSC_VER

#ifndef __PL64__
# if defined(_MSC_VER)
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

#if defined(__clang__)
#define __printflike__ __printflike
#else
#define __printflike__(...)
#endif

CC_END

#endif /* DEFINES_H */
