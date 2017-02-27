//
//  types.h
//  TagBufCPP
//
//  Created by hejunqiu on 16/8/31.
//  Copyright © 2016年 CHE. All rights reserved.
//

#ifndef TYPES_H
#define TYPES_H

#include "defines.h"

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

#endif /* TYPES_H */
