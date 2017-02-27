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

    NumberPrivate(float v) { internal.floatValue = v; }

    NumberPrivate(double v) { internal.doubleValue = v; }

    template<typename T>
    NumberPrivate(T v) { internal.longLongValue = (long long)v; }

    NumberPrivate()
    {}

    NumberPrivate* duplicate() const override
    {
        auto copy = new NumberPrivate;
        copy->internal = internal;
        return copy;
    }
};

Number::Number()
:Object()
{
}

Number::Number(bool v)
:Object(new NumberPrivate(v))
{}

Number::Number(char v)
:Object(new NumberPrivate(v))
{}

Number::Number(unsigned char v)
:Object(new NumberPrivate(v))
{}

Number::Number(short v)
:Object(new NumberPrivate(v))
{}

Number::Number(unsigned short v)
:Object(new NumberPrivate(v))
{}

Number::Number(int v)
:Object(new NumberPrivate(v))
{}

Number::Number(unsigned int v)
:Object(new NumberPrivate(v))
{}

Number::Number(float v)
:Object(new NumberPrivate(v))
{}

Number::Number(double v)
:Object(new NumberPrivate(v))
{}

Number::Number(long v)
:Object(new NumberPrivate(v))
{}

Number::Number(unsigned long v)
:Object(new NumberPrivate(v))
{}

Number::Number(long long v)
:Object(new NumberPrivate(v))
{}

Number::Number(unsigned long long v)
:Object(new NumberPrivate(v))
{}


bool Number::boolValue() const
{
    return (bool)*this;
}

char Number::charValue() const
{
    return (char)*this;
}

unsigned char Number::unsignedCharValue() const
{
    return (unsigned char)*this;
}

short Number::shortValue() const
{
    return (short)*this;
}

unsigned short Number::unsignedShortValue() const
{
    return (unsigned short)*this;
}

int Number::intValue() const
{
    return (int)*this;
}

unsigned int Number::unsignedIntValue() const
{
    return (unsigned int)*this;
}

float Number::floatValue() const
{
    return (float)*this;
}

double Number::doubleValue() const
{
    return (double)*this;
}

long Number::longValue() const
{
    return (long)*this;
}

unsigned long Number::unsignedLongValue() const
{
    return (unsigned long)*this;
}

long long Number::longLongValue() const
{
    return (long long)*this;
}

unsigned long long Number::unsginedLongLongValue() const
{
    return (unsigned long long)*this;
}

integer Number::integerValue() const
{
    return (integer)*this;
}

uinteger Number::unsignedIntegerValue() const
{
    return (uinteger)*this;
}


Number::operator bool() const
{
    if (isTaggedPointer()) {
        return (bool)(((uintptr_t)this ^ TAGGED_POINTER_NUMBER_FLAG) >> 1);
    }
    D_D(Number);
    return d.internal.boolValue;
}

Number::operator unsigned char() const
{
    if (isTaggedPointer()) {
        return (unsigned char)(((uintptr_t)this ^ TAGGED_POINTER_NUMBER_FLAG) >> 1);
    }
    D_D(Number);
    return d.internal.charValue;
}

Number::operator char() const
{
    return this->operator unsigned char();
}

Number::operator unsigned short() const
{
    if (isTaggedPointer()) {
        return (unsigned short)(((uintptr_t)this ^ TAGGED_POINTER_NUMBER_FLAG) >> 1);
    }
    D_D(Number);
    return d.internal.shortValue;
}

Number::operator short() const
{
    return this->operator unsigned short();
}

Number::operator unsigned int() const
{
    if (isTaggedPointer()) {
        return (unsigned int)(((uintptr_t)this ^ TAGGED_POINTER_NUMBER_FLAG) >> 1);
    }
    D_D(Number);
    return d.internal.intValue;
}

Number::operator int() const
{
    return this->operator unsigned int();
}

Number::operator unsigned long() const
{
    if (isTaggedPointer()) {
        return (unsigned long)this->operator unsigned long long();
    }
    D_D(Number);
    return d.internal.longValue;
}

Number::operator long() const
{
    return this->operator unsigned long();
}

Number::operator unsigned long long() const
{
    if (isTaggedPointer()) {
        return (unsigned long long)(((uintptr_t)this ^ TAGGED_POINTER_NUMBER_FLAG) >> 1);
    }
    D_D(Number);;;
    return d.internal.longLongValue;
}

Number::operator long long() const
{
    return this->operator unsigned long long();
}

Number::operator float() const
{
    if (isTaggedPointer()) {
        uintptr_t ret = (uintptr_t)this;
        if (ret & 0xFFFFFFFF00000000ULL) { // may be a double value
            _double d{.dd = static_cast<uint64_t>(ret ^ TAGGED_POINTER_NUMBER_FLAG)};
            return (float)d.d;
        }
        ret = ((uintptr_t)this ^ TAGGED_POINTER_NUMBER_FLAG) >> 1;
        _float f{.ff = static_cast<uint32_t>(ret)};
        return f.f;
    }
    D_D(Number);
    return d.internal.floatValue;
}

Number::operator double() const
{
    if (isTaggedPointer()) {
        uintptr_t ret = ((uintptr_t)this ^ TAGGED_POINTER_NUMBER_FLAG);
        _double d{.dd = static_cast<uint64_t>(ret)};
        return d.d;
    }
    D_D(Number);
    return d.internal.doubleValue;
}



shared_ptr<Number> Number::numberWithValue(bool v)
{
    return numberWithValue((unsigned int)v);
}

shared_ptr<Number> Number::numberWithValue(char v)
{
    return numberWithValue((unsigned int)v);
}

shared_ptr<Number> Number::numberWithValue(unsigned char v)
{
    return numberWithValue((unsigned int)v);
}

shared_ptr<Number> Number::numberWithValue(short v)
{
    return numberWithValue((unsigned int)v);
}

shared_ptr<Number> Number::numberWithValue(unsigned short v)
{
    return numberWithValue((unsigned int)v);
}

shared_ptr<Number> Number::numberWithValue(int v)
{
    return numberWithValue((unsigned int)v);
}

shared_ptr<Number> Number::numberWithValue(unsigned int v)
{
    Number *o = reinterpret_cast<Number *>(((uintptr_t)v) << 1 | TAGGED_POINTER_NUMBER_FLAG);
    return shared_ptr<Number>(o);
}

shared_ptr<Number> Number::numberWithValue(float v)
{
    _float f{v};
    uintptr_t ret = f.ff;
    Number * o = reinterpret_cast<Number *>((ret) << 1 | TAGGED_POINTER_NUMBER_FLAG);
    return shared_ptr<Number>(o);
}

shared_ptr<Number> Number::numberWithValue(double v)
{
    _double d{v};
    return numberWithValue(d.dd);
}

shared_ptr<Number> Number::numberWithValue(long v)
{
    return numberWithValue((unsigned long)v);
}

shared_ptr<Number> Number::numberWithValue(unsigned long v)
{
    if (v & MAX_INDICATE_NUMBER) {
        return std::make_shared<Number>(v);
    }
    Number *o = reinterpret_cast<Number *>(((uintptr_t)v) << 1 | TAGGED_POINTER_NUMBER_FLAG);
    return std::shared_ptr<Number>(o);
}

shared_ptr<Number> Number::numberWithValue(long long v)
{
    return numberWithValue((unsigned long long)v);
}

shared_ptr<Number> Number::numberWithValue(unsigned long long v)
{
    if (v & MAX_INDICATE_NUMBER) {
        return std::make_shared<Number>(v);
    }
    Number *o = reinterpret_cast<Number *>(((uintptr_t)v) << 1 | TAGGED_POINTER_NUMBER_FLAG);
    return std::shared_ptr<Number>(o);
}

CC_END
