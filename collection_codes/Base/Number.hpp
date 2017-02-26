//
//  Number.hpp
//  TagBufCPP
//
//  Created by hejunqiu on 16/9/16.
//  Copyright © 2016年 CHE. All rights reserved.
//

#ifndef NUMBER_HPP
#define NUMBER_HPP

#include "Object.hpp"
#include <memory>

using std::shared_ptr;

CC_BEGIN

class String;

CLASS_TAGGEDPOINTER_AVAILABLE class Number : public Object
{
public:
    Number();
    Number(bool v);
    Number(char v);
    Number(unsigned char v);
    Number(short v);
    Number(unsigned short v);
    Number(int v);
    Number(unsigned int v);
    Number(float v);
    Number(double v);
    Number(long v);
    Number(unsigned long v);
    Number(long long v);
    Number(unsigned long long v);

    operator bool() const;
    operator char() const;
    operator unsigned char() const;
    operator short() const;
    operator unsigned short() const;
    operator int() const;
    operator unsigned int() const;
    operator float() const;
    operator long() const;
    operator unsigned long() const;
    operator long long() const;
    operator unsigned long long() const;
    operator double() const;

    bool boolValue() const;
    char charValue() const;
    unsigned char unsignedCharValue() const;
    short shortValue() const;
    unsigned short unsignedShortValue() const;
    int intValue() const;
    unsigned int unsignedIntValue() const;
    float floatValue() const;
    double doubleValue() const;
    long longValue() const;
    unsigned long unsignedLongValue() const;
    long long longLongValue() const;
    unsigned long long unsginedLongLongValue() const;
    integer integerValue() const;
    uinteger unsignedIntegerValue() const;

    static shared_ptr<Number> numberWithValue(bool v);
    static shared_ptr<Number> numberWithValue(char v);
    static shared_ptr<Number> numberWithValue(unsigned char v);
    static shared_ptr<Number> numberWithValue(short v);
    static shared_ptr<Number> numberWithValue(unsigned short v);
    static shared_ptr<Number> numberWithValue(int v);
    static shared_ptr<Number> numberWithValue(unsigned int v);
    static shared_ptr<Number> numberWithValue(float v);
    static shared_ptr<Number> numberWithValue(double v);
    static shared_ptr<Number> numberWithValue(long v);
    static shared_ptr<Number> numberWithValue(unsigned long v);
    static shared_ptr<Number> numberWithValue(long long v);
    static shared_ptr<Number> numberWithValue(unsigned long long v);
};

#define number(v) numberWithValue(v)

CC_END

#endif /* NUMBER_HPP */
