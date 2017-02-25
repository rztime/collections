//
//  String.cpp
//  TagBufCPP
//
//  Created by hejunqiu on 16/8/31.
//  Copyright © 2016年 CHE. All rights reserved.
//

#include "String.hpp"
#include "TaggedPointer.h"
#include <stdlib.h>
#include <string.h>
#include "tprintf.hpp"
#include "Algorithm.hpp"
#include "Data.hpp"
#include "CHException.hpp"

CC_BEGIN

typedef enum : uinteger  {
    StringBufferTypeImmutable,
    StringBufferTypeMutable
} StringBufferType;

struct StringPrivate : ObjectPrivate
{

};

String::String() : Object()
{}

String::~String()
{
}

#define TAGGED_STRING_POINTER_RETURN(obj) reinterpret_cast<const char *>(((uintptr_t)obj ^ TAGGED_POINTER_STRING_FLAG) >> 1)

CC_END
