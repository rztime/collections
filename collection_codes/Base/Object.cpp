//
//  id.cpp
//  TagBufCPP
//
//  Created by hejunqiu on 16/9/16.
//  Copyright © 2016年 CHE. All rights reserved.
//

#include "Object.hpp"
#include "TaggedPointer.h"
#include "Object.inl"

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

bool Object::operator==(const Object *anObject) const
{
    return anObject && this->equalTo(*anObject);
}

bool Object::operator==(const Object &anObject) const
{
    return this->equalTo(anObject);
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

Object *Object::duplicate() const
{
    if (isTaggedPointer()) {
        return (Object *)this;
    }
    ObjectPrivate *private_d = _d ? _d->duplicate() : nullptr;
    Object *copy = new Object(private_d);
    return copy;
}

CC_END
