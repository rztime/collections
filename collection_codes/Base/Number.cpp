//
//  Number.cpp
//  TagBufCPP
//
//  Created by hejunqiu on 16/9/16.
//  Copyright © 2016年 CHE. All rights reserved.
//

#include "Number.hpp"
#include "TaggedPointer.h"
#include "String.hpp"
#include "Object.inl"
#include "String.inl"

CC_BEGIN

struct NumberPrivate : public ObjectPrivate
{
    union {
        long long longLongValue;
        double doubleValue;
        long longValue;
        float floatValue;
        int intValue;
        short shortValue;
        char charValue;
        bool boolValue;
    } internal = {0};

    NumberPrivate(float v)  _NOEXCEPT_ { internal.floatValue = v; }

    NumberPrivate(double v)  _NOEXCEPT_ { internal.doubleValue = v; }

    template<typename T>
    NumberPrivate(T v)  _NOEXCEPT_ { internal.longLongValue = (long long)v; }

    NumberPrivate()  _NOEXCEPT_ {}

    NumberPrivate* duplicate() const _NOEXCEPT_ override
    {
        auto copy = new NumberPrivate;
        copy->internal = internal;
        return copy;
    }
};

Number::Number()  _NOEXCEPT_
:Object()
{
}

Number::Number(bool v) _NOEXCEPT_
:Object(new NumberPrivate(v))
{}

Number::Number(char v) _NOEXCEPT_
:Object(new NumberPrivate(v))
{}

Number::Number(unsigned char v) _NOEXCEPT_
:Object(new NumberPrivate(v))
{}

Number::Number(short v) _NOEXCEPT_
:Object(new NumberPrivate(v))
{}

Number::Number(unsigned short v) _NOEXCEPT_
:Object(new NumberPrivate(v))
{}

Number::Number(int v) _NOEXCEPT_
:Object(new NumberPrivate(v))
{}

Number::Number(unsigned int v) _NOEXCEPT_
:Object(new NumberPrivate(v))
{}

Number::Number(float v) _NOEXCEPT_
:Object(new NumberPrivate(v))
{}

Number::Number(double v) _NOEXCEPT_
:Object(new NumberPrivate(v))
{}

Number::Number(long v) _NOEXCEPT_
:Object(new NumberPrivate(v))
{}

Number::Number(unsigned long v) _NOEXCEPT_
:Object(new NumberPrivate(v))
{}

Number::Number(long long v) _NOEXCEPT_
:Object(new NumberPrivate(v))
{}

Number::Number(unsigned long long v) _NOEXCEPT_
:Object(new NumberPrivate(v))
{}


bool Number::boolValue() const _NOEXCEPT_
{
    return (bool)*this;
}

char Number::charValue() const _NOEXCEPT_
{
    return (char)*this;
}

unsigned char Number::unsignedCharValue() const _NOEXCEPT_
{
    return (unsigned char)*this;
}

short Number::shortValue() const _NOEXCEPT_
{
    return (short)*this;
}

unsigned short Number::unsignedShortValue() const _NOEXCEPT_
{
    return (unsigned short)*this;
}

int Number::intValue() const _NOEXCEPT_
{
    return (int)*this;
}

unsigned int Number::unsignedIntValue() const _NOEXCEPT_
{
    return (unsigned int)*this;
}

float Number::floatValue() const _NOEXCEPT_
{
    return (float)*this;
}

double Number::doubleValue() const _NOEXCEPT_
{
    return (double)*this;
}

long Number::longValue() const _NOEXCEPT_
{
    return (long)*this;
}

unsigned long Number::unsignedLongValue() const _NOEXCEPT_
{
    return (unsigned long)*this;
}

long long Number::longLongValue() const _NOEXCEPT_
{
    return (long long)*this;
}

unsigned long long Number::unsginedLongLongValue() const _NOEXCEPT_
{
    return (unsigned long long)*this;
}

integer Number::integerValue() const _NOEXCEPT_
{
    return (integer)*this;
}

uinteger Number::unsignedIntegerValue() const _NOEXCEPT_
{
    return (uinteger)*this;
}


Number::operator bool() const _NOEXCEPT_
{
    if (isTaggedPointer()) {
        return (bool)(((uintptr_t)this ^ TAGGED_POINTER_NUMBER_FLAG) >> 1);
    }
    D_D(Number);
    return d.internal.boolValue;
}

Number::operator unsigned char() const _NOEXCEPT_
{
    if (isTaggedPointer()) {
        return (unsigned char)(((uintptr_t)this ^ TAGGED_POINTER_NUMBER_FLAG) >> 1);
    }
    D_D(Number);
    return d.internal.charValue;
}

Number::operator char() const _NOEXCEPT_
{
    return this->operator unsigned char();
}

Number::operator unsigned short() const _NOEXCEPT_
{
    if (isTaggedPointer()) {
        return (unsigned short)(((uintptr_t)this ^ TAGGED_POINTER_NUMBER_FLAG) >> 1);
    }
    D_D(Number);
    return d.internal.shortValue;
}

Number::operator short() const _NOEXCEPT_
{
    return this->operator unsigned short();
}

Number::operator unsigned int() const _NOEXCEPT_
{
    if (isTaggedPointer()) {
        return (unsigned int)(((uintptr_t)this ^ TAGGED_POINTER_NUMBER_FLAG) >> 1);
    }
    D_D(Number);
    return d.internal.intValue;
}

Number::operator int() const _NOEXCEPT_
{
    return this->operator unsigned int();
}

Number::operator unsigned long() const _NOEXCEPT_
{
    if (isTaggedPointer()) {
        return (unsigned long)this->operator unsigned long long();
    }
    D_D(Number);
    return d.internal.longValue;
}

Number::operator long() const _NOEXCEPT_
{
    return this->operator unsigned long();
}

Number::operator unsigned long long() const _NOEXCEPT_
{
    if (isTaggedPointer()) {
        return (unsigned long long)(((uintptr_t)this ^ TAGGED_POINTER_NUMBER_FLAG) >> 1);
    }
    D_D(Number);;;
    return d.internal.longLongValue;
}

Number::operator long long() const _NOEXCEPT_
{
    return this->operator unsigned long long();
}

Number::operator float() const _NOEXCEPT_
{
    if (isTaggedPointer()) {
        uintptr_t ret = (uintptr_t)this;
        if (ret & 0xFFFFFFFF00000000ULL) { // may be a double value
            _double d(static_cast<uint64_t>(ret ^ TAGGED_POINTER_NUMBER_FLAG));
            return (float)d.d;
        }
        ret = ((uint64_t)this ^ TAGGED_POINTER_NUMBER_FLAG) >> 1;
        _float f(static_cast<uint32_t>(ret));
        return f.f;
    }
    D_D(Number);
    return d.internal.floatValue;
}

Number::operator double() const _NOEXCEPT_
{
    if (isTaggedPointer()) {
        uintptr_t ret = ((uintptr_t)this ^ TAGGED_POINTER_NUMBER_FLAG);
        _double d(static_cast<uint64_t>(ret));
        return d.d;
    }
    D_D(Number);
    return d.internal.doubleValue;
}



shared_ptr<Number> Number::numberWithValue(bool v) _NOEXCEPT_
{
    return numberWithValue((unsigned int)v);
}

shared_ptr<Number> Number::numberWithValue(char v) _NOEXCEPT_
{
    return numberWithValue((unsigned int)v);
}

shared_ptr<Number> Number::numberWithValue(unsigned char v) _NOEXCEPT_
{
    return numberWithValue((unsigned int)v);
}

shared_ptr<Number> Number::numberWithValue(short v) _NOEXCEPT_
{
    return numberWithValue((unsigned int)v);
}

shared_ptr<Number> Number::numberWithValue(unsigned short v) _NOEXCEPT_
{
    return numberWithValue((unsigned int)v);
}

shared_ptr<Number> Number::numberWithValue(int v) _NOEXCEPT_
{
    return numberWithValue((unsigned int)v);
}

shared_ptr<Number> Number::numberWithValue(unsigned int v) _NOEXCEPT_
{
    Number *o = reinterpret_cast<Number *>(((uintptr_t)v) << 1 | TAGGED_POINTER_NUMBER_FLAG);
    return shared_ptr<Number>(o, __deleter__());
}

shared_ptr<Number> Number::numberWithValue(float v) _NOEXCEPT_
{
    _float f(v);
    uintptr_t ret = f.ff;
    Number * o = reinterpret_cast<Number *>((ret) << 1 | TAGGED_POINTER_NUMBER_FLAG);
    return shared_ptr<Number>(o, __deleter__());
}

shared_ptr<Number> Number::numberWithValue(double v) _NOEXCEPT_
{
    _double d(v);
    return numberWithValue(d.dd);
}

shared_ptr<Number> Number::numberWithValue(long v) _NOEXCEPT_
{
    return numberWithValue((unsigned long)v);
}

shared_ptr<Number> Number::numberWithValue(unsigned long v) _NOEXCEPT_
{
    if (v & MAX_INDICATE_NUMBER) {
        return std::make_shared<Number>(v);
    }
    Number *o = reinterpret_cast<Number *>(((uintptr_t)v) << 1 | TAGGED_POINTER_NUMBER_FLAG);
    return std::shared_ptr<Number>(o, __deleter__());
}

shared_ptr<Number> Number::numberWithValue(long long v) _NOEXCEPT_
{
    return numberWithValue((unsigned long long)v);
}

shared_ptr<Number> Number::numberWithValue(unsigned long long v) _NOEXCEPT_
{
    if (v & MAX_INDICATE_NUMBER) {
        return std::make_shared<Number>(v);
    }
    Number *o = reinterpret_cast<Number *>(((uintptr_t)v) << 1 | TAGGED_POINTER_NUMBER_FLAG);
    return std::shared_ptr<Number>(o, __deleter__());
}

CC_END
