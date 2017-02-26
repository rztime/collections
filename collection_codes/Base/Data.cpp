//
//  Data.cpp
//  TagBufCPP
//
//  Created by hejunqiu on 16/9/2.
//  Copyright © 2016年 CHE. All rights reserved.
//

#include "Data.hpp"
#include <stdlib.h>
#include <memory.h>
#include <memory>
#include "TaggedPointer.h"

using namespace std;

CC_BEGIN

struct DataPrivate : public ObjectPrivate
{
    DataPrivate(integer capacity)
    :data(capacity){}

    DataPrivate(const vector<byte> &d)
    :data(d) {}

    DataPrivate(vector<byte> &&d)
    :data(std::move(d)){}

    vector<byte> data;
};

Data::Data(uinteger capacity)
:Object(new DataPrivate(capacity))
{}

Data::Data(const byte *bytes, uinteger length)
:Data(length)
{
    append(bytes, length);
}

Data::Data(const vector<byte> &bytes)
:Object(new DataPrivate(bytes))
{
}

Data::Data(vector<byte> &&bytes)
:Object(new DataPrivate(std::move(bytes)))
{
}

Data::Data(const string &aString)
:Data(aString.length())
{
    append(aString);
}

Data *Data::duplicate() const
{
    if (isTaggedPointer()) {
        return (Data *)this;
    }
    D_D(Data);
    Data *obj = new Data(d.data.size());
    *obj = d.data;
    return obj;
}

Data::~Data()
{
}

uinteger Data::length() const
{
    D_D(Data);
    return d.data.size();
}

uinteger Data::capacity() const
{
    D_D(Data);
    return d.data.capacity();
}

void Data::clear()
{
    D_D(Data);
    d.data.clear();
}

void Data::deleteBytesInRange(Range range)
{
    if (range.maxRange() > length()) {
        return;
    }
    D_D(Data);
    auto s = d.data.begin() + range.location;
    d.data.erase(s, s + range.length);
}

Data& Data::operator=(const Data &rhs)
{
    clear();
    append(rhs);
    return *this;
}

Data& Data::operator=(const std::vector<byte> &rhs)
{
    clear();
    append(rhs);
    return *this;
}

Data& Data::operator=(const char *utf8String)
{
    auto s = strlen((char *)utf8String);
    D_D(Data);
    d.data.resize(s);
    memcpy(d.data.data(), utf8String, s);
    return *this;
}

Data& Data::operator=(const std::string &rhs)
{
    D_D(Data);
    d.data.resize(rhs.size());
    auto iter = rhs.begin();
    memcpy(d.data.data(), &*iter, rhs.size());
    return *this;
}

Data& Data::append(const void *bytes, uinteger length)
{
    insert(bytes, length, this->length());
    return *this;
}

Data& Data::append(const Data &data)
{
    insert(data, length());
    return *this;
}

Data& Data::append(const vector<byte> &data)
{
    insert(data, length());
    return *this;
}

Data& Data::append(const string &aString)
{
    insert(aString, length());
    return *this;
}

void Data::insert(byte b, uinteger pos)
{
    D_D(Data);
    d.data.insert(d.data.begin() + pos, b);
}

void Data::insert(const void *b, uinteger length, uinteger pos)
{
    D_D(Data);
    d.data.insert(d.data.begin() + pos, *(byte *)b, *((const byte *)b + length));
}

void Data::insert(const Data &data, uinteger pos)
{
    D_D(Data);
    auto iter_b = D_O(Data, data).data.begin();
    auto iter_e = D_O(Data, data).data.end();
    d.data.insert(d.data.begin() + pos, iter_b, iter_e);
}

void Data::insert(const vector<byte> &data, uinteger pos)
{
    D_D(Data);
    d.data.insert(d.data.begin() + pos, data.begin(), data.end());
}

void Data::insert(const string &aString, uinteger pos)
{
    D_D(Data);
    d.data.insert(d.data.begin() + pos, aString.begin(), aString.end());
}

void Data::replace(Range range, const Data &data)
{
    replace(range, data.data(), data.length());
}

void Data::replace(Range range, const void *data, uinteger length)
{
    if (range.maxRange() > this->length()) {
        return;
    }
    deleteBytesInRange(range);
    insert(data, length, range.location);
}

void Data::resetInRange(Range range)
{
    if (range.maxRange() > length()) {
        return;
    }
    D_D(Data);
    auto s = d.data.begin() + range.location;
    auto e = d.data.begin() + range.maxRange();
    while (s < e) {
        *++s = 0;
    }
}

byte Data::operator[](const uinteger idx) const
{
    D_D(Data);
    return d.data[idx];
}
byte& Data::operator[](const uinteger idx)
{
    D_D(Data);
    return d.data[idx];
}

byte * Data::data()
{
	D_D(Data);
	return d.data.data();
}

const byte * Data::data() const
{
	D_D(Data);
	return d.data.data();
}

CC_END
