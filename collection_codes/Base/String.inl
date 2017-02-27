//
//  String.inl
//  collection_codes
//
//  Created by hejunqiu on 2017/2/27.
//  Copyright © 2017年 CHE. All rights reserved.
//

#ifndef STRING_INL
#define STRING_INL

#include "defines.h"
#include "Object.hpp"
#include "TaggedPointer.h"

CC_BEGIN

struct __deleter__
{
    void operator()(Object *p)
    {
        if (!p->isTaggedPointer()) {
            delete p;
        }
    }
};

CC_END


#endif /* STRING_INL */
