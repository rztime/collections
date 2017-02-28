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
#include <type_traits>

using std::shared_ptr;

CC_BEGIN

class String;

CLASS_TAGGEDPOINTER_AVAILABLE class Number : public Object
{
public:
    Number() _NOEXCEPT_;
    Number(bool v) _NOEXCEPT_;
    Number(char v) _NOEXCEPT_;
    Number(unsigned char v) _NOEXCEPT_;
    Number(short v) _NOEXCEPT_;
    Number(unsigned short v) _NOEXCEPT_;
    Number(int v) _NOEXCEPT_;
    Number(unsigned int v) _NOEXCEPT_;
    Number(float v) _NOEXCEPT_;
    Number(double v) _NOEXCEPT_;
    Number(long v) _NOEXCEPT_;
    Number(unsigned long v) _NOEXCEPT_;
    Number(long long v) _NOEXCEPT_;
    Number(unsigned long long v) _NOEXCEPT_;

    operator bool() const _NOEXCEPT_;
    operator char() const _NOEXCEPT_;
    operator unsigned char() const _NOEXCEPT_;
    operator short() const _NOEXCEPT_;
    operator unsigned short() const _NOEXCEPT_;
    operator int() const _NOEXCEPT_;
    operator unsigned int() const _NOEXCEPT_;
    operator float() const _NOEXCEPT_;
    operator long() const _NOEXCEPT_;
    operator unsigned long() const _NOEXCEPT_;
    operator long long() const  _NOEXCEPT_;
    operator unsigned long long() const _NOEXCEPT_;
    operator double() const _NOEXCEPT_;

    bool boolValue() const _NOEXCEPT_;
    char charValue() const _NOEXCEPT_;
    unsigned char unsignedCharValue() const _NOEXCEPT_;
    short shortValue() const _NOEXCEPT_;
    unsigned short unsignedShortValue() const _NOEXCEPT_;
    int intValue() const _NOEXCEPT_;
    unsigned int unsignedIntValue() const _NOEXCEPT_;
    float floatValue() const _NOEXCEPT_;
    double doubleValue() const _NOEXCEPT_;
    long longValue() const _NOEXCEPT_;
    unsigned long unsignedLongValue() const _NOEXCEPT_;
    long long longLongValue() const _NOEXCEPT_;
    unsigned long long unsginedLongLongValue() const _NOEXCEPT_;
    integer integerValue() const _NOEXCEPT_;
    uinteger unsignedIntegerValue() const _NOEXCEPT_;

    static shared_ptr<Number> numberWithValue(bool v) _NOEXCEPT_;
    static shared_ptr<Number> numberWithValue(char v) _NOEXCEPT_;
    static shared_ptr<Number> numberWithValue(unsigned char v) _NOEXCEPT_;
    static shared_ptr<Number> numberWithValue(short v) _NOEXCEPT_;
    static shared_ptr<Number> numberWithValue(unsigned short v) _NOEXCEPT_;
    static shared_ptr<Number> numberWithValue(int v) _NOEXCEPT_;
    static shared_ptr<Number> numberWithValue(unsigned int v) _NOEXCEPT_;
    static shared_ptr<Number> numberWithValue(float v) _NOEXCEPT_;
    static shared_ptr<Number> numberWithValue(double v) _NOEXCEPT_;
    static shared_ptr<Number> numberWithValue(long v) _NOEXCEPT_;
    static shared_ptr<Number> numberWithValue(unsigned long v) _NOEXCEPT_;
    static shared_ptr<Number> numberWithValue(long long v) _NOEXCEPT_;
    static shared_ptr<Number> numberWithValue(unsigned long long v) _NOEXCEPT_;
};

#define number(v) numberWithValue(v)

__unused static shared_ptr<Number> operator""_sn(unsigned long long num)  _NOEXCEPT_
{
	return Number::numberWithValue(num);
}

__unused static shared_ptr<Number> operator""_sn(long double num)  _NOEXCEPT_
{
	return Number::numberWithValue((double)num);
}

CC_END

#endif /* NUMBER_HPP */
