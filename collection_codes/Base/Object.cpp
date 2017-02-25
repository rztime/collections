//
//  id.cpp
//  TagBufCPP
//
//  Created by hejunqiu on 16/9/16.
//  Copyright © 2016年 CHE. All rights reserved.
//

#include "Object.hpp"
#include "TaggedPointer.h"

CC_BEGIN

Object::Object(ObjectPrivate *data/* = 0*/)
:_d(data)
{
}

Object::~Object()
{
    if (!isTaggedPointer()) {
        delete _d;
        _d = nullptr;
    }
}

bool Object::isTaggedPointer() const
{
    return ISTAGGEDPOINTER();
}

bool Object::equalTo(const Object &anObject) const
{
    return this == &anObject;
}

String *Object::description() const
{
    return nullptr;
}

uint64_t Object::hash_code() const
{
    return (uint64_t)this;
}

CC_END
