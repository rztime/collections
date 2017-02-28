//
//  Data.cpp
//  TagBufCPP
//
//  Created by hejunqiu on 16/9/2.
//  Copyright © 2016年 CHE. All rights reserved.
//

#include "Data.hpp"
#include "Object.inl"
#include "Exception.hpp"
#include <stdlib.h>
#include <memory.h>
#include <ostream>

using namespace std;

CC_BEGIN

struct DataPrivate : public ObjectPrivate
{
    DataPrivate(integer capacity)  _NOEXCEPT_
    :data(capacity)
    {}

    DataPrivate(const vector<byte> &d) _NOEXCEPT_
    :data(d)
    {}

    DataPrivate(vector<byte> &&d) _NOEXCEPT_
    :data(std::move(d)) {}

    DataPrivate* duplicate() const _NOEXCEPT_ override
    {
        return new DataPrivate(data);
    }

    vector<byte> data;
};

Data::Data(uinteger capacity) _NOEXCEPT_
:Object(new DataPrivate(capacity))
{}

Data::Data(const void *bytes, uinteger length) _NOEXCEPT_
:Data(length)
{
    append((byte *)bytes, length);
}

Data::Data(const vector<byte> &bytes) _NOEXCEPT_
:Object(new DataPrivate(bytes))
{
}

Data::Data(vector<byte> &&bytes) _NOEXCEPT_
:Object(new DataPrivate(std::move(bytes)))
{
}

Data::Data(const string &aString) _NOEXCEPT_
:Data(aString.length())
{
    append(aString);
}

Data::Data(const Data &data) _NOEXCEPT_
:Object(new DataPrivate(D_O(Data, data).data))
{
}

Data::Data(Data &&data) _NOEXCEPT_
:Object(new DataPrivate(std::move(D_O(Data, data).data)))
{
}

Data::~Data() _NOEXCEPT_
{
}

uinteger Data::length() const _NOEXCEPT_
{
    D_D(Data);
    return d.data.size();
}

uinteger Data::capacity() const _NOEXCEPT_
{
    D_D(Data);
    return d.data.capacity();
}

void Data::clear() _NOEXCEPT_
{
    D_D(Data);
    d.data.clear();
}

void Data::deleteBytesInRange(Range range) _NOEXCEPT_(false)
{
    if (range.maxRange() > length()) {
		throwException(OutOfRangeException, "Out of Range%p@", range.description().get());
    }
    D_D(Data);
    auto s = d.data.begin() + range.location;
    d.data.erase(s, s + range.length);
}

Data& Data::operator=(const Data &rhs) _NOEXCEPT_
{
    clear();
    append(rhs);
    return *this;
}

Data& Data::operator=(const std::vector<byte> &rhs) _NOEXCEPT_
{
    clear();
    append(rhs);
    return *this;
}

Data& Data::operator=(const char *utf8String)  _NOEXCEPT_(false)
{
	if (!utf8String) {
		throwException(InvalidArgumentException, "Parameter utf8String must not be null!");
	}
    auto s = strlen((char *)utf8String);
    D_D(Data);
    d.data.resize(s);
    memcpy(d.data.data(), utf8String, s);
    return *this;
}

Data& Data::operator=(const std::string &rhs) _NOEXCEPT_
{
    D_D(Data);
    d.data.resize(rhs.size());
    auto iter = rhs.begin();
    memcpy(d.data.data(), &*iter, rhs.size());
    return *this;
}

Data & Data::operator+=(const Data & rhs) _NOEXCEPT_
{
	append(rhs);
	return *this;
}

Data & Data::operator+=(const byte rhs) _NOEXCEPT_
{
	append(&rhs, 1);
	return *this;
}

Data& Data::append(const void *bytes, uinteger length) _NOEXCEPT_(false)
{
    insert(bytes, length, this->length());
    return *this;
}

Data& Data::append(const Data &data) _NOEXCEPT_
{
    insert(data, length());
    return *this;
}

Data& Data::append(const vector<byte> &data) _NOEXCEPT_
{
    insert(data, length());
    return *this;
}

Data& Data::append(const string &aString) _NOEXCEPT_
{
    insert(aString, length());
    return *this;
}

void Data::insert(byte b, uinteger pos) _NOEXCEPT_(false)
{
	insert(&b, 1, pos);
}

void Data::insert(const void *b, uinteger length, uinteger pos) _NOEXCEPT_(false)
{
	if (pos > this->length()) {
		throwException(RangeException, "Parameter:pos(%zu) should not greater than string's length(%zu)", pos, this->length());
	}
    D_D(Data);
    d.data.insert(d.data.begin() + pos, *(byte *)b, *((const byte *)b + length));
}

void Data::insert(const Data &data, uinteger pos) _NOEXCEPT_(false)
{
    D_D(Data);
    auto iter_b = D_O(Data, data).data.begin();
    auto iter_e = D_O(Data, data).data.end();
    d.data.insert(d.data.begin() + pos, iter_b, iter_e);
}

void Data::insert(const vector<byte> &data, uinteger pos) _NOEXCEPT_(false)
{
    D_D(Data);
    d.data.insert(d.data.begin() + pos, data.begin(), data.end());
}

void Data::insert(const string &aString, uinteger pos) _NOEXCEPT_(false)
{
    D_D(Data);
    d.data.insert(d.data.begin() + pos, aString.begin(), aString.end());
}

void Data::replace(Range range, const Data &data) _NOEXCEPT_(false)
{
    replace(range, data.data(), data.length());
}

void Data::replace(Range range, const void *data, uinteger length) _NOEXCEPT_(false)
{
    if (range.maxRange() > this->length()) {
        return;
    }
    deleteBytesInRange(range);
    insert(data, length, range.location);
}

void Data::resetInRange(Range range) _NOEXCEPT_(false)
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

byte Data::operator[](const uinteger idx) const _NOEXCEPT_(false)
{
    D_D(Data);
    return d.data[idx];
}
byte& Data::operator[](const uinteger idx) _NOEXCEPT_(false)
{
    D_D(Data);
    return d.data[idx];
}

byte * Data::data() _NOEXCEPT_
{
	D_D(Data);
	return d.data.data();
}

const byte * Data::data() const _NOEXCEPT_
{
	D_D(Data);
	return d.data.data();
}

std::ostream& operator<<(std::ostream& os, const Data &data) _NOEXCEPT_
{
    return os << &data;
}

std::ostream& operator<<(std::ostream& os, const shared_ptr<Data> &data) _NOEXCEPT_
{
    return os << data.get();
}

std::ostream& operator<<(std::ostream& os, const Data *data) _NOEXCEPT_
{
    if (!data) {
        os << "(null)";
    } else {
        auto flags = os.flags();
        os.setf(ios::showbase);
        os.setf(ios_base::hex | ios_base::left, ios_base::basefield);
        os << "<";
        size_t length = data->length();
        const unsigned int *p = (const unsigned int *)data->data();
        for (;;) {
            os << *p++;
            if (length -= 4 < 4) {
                break;
            }
            os << " ";
        }
        if (length > 0) {
            unsigned int rest = 0;
            memcpy(&rest, p, length);
            os << rest;
        }
        os << ">";
        os.setf(flags);
    }
    return os;
}

CC_END
