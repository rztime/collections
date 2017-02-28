//
//  String.cpp
//  TagBufCPP
//
//  Created by hejunqiu on 16/8/31.
//  Copyright © 2016年 CHE. All rights reserved.
//

#include "String.hpp"
#include "String.inl"
#include "Object.inl"
#include "tprintf.hpp"
#include "Algorithm.hpp"
#include "TaggedPointer.h"
#include "Data.hpp"
#include <ostream>
#include <stdlib.h>
#include <string.h>
#include <algorithm>

using namespace std;

CC_BEGIN

typedef enum : uinteger  {
    StringBufferTypeImmutable,
    StringBufferTypeMutable
} StringBufferType;

struct StringPrivate : ObjectPrivate
{
	StringPrivate(const char *s)
	:buf(s)
	{}

	StringPrivate(const string &s)
	:buf(s)
	{}

	StringPrivate(string &&s)
    :buf(s)
	{}

	StringPrivate(const Data &data)
	:buf((char *)data.data(), (char *)data.data() + data.length())
	{}

    StringPrivate(const void *bytes, uinteger length)
    :buf((char *)bytes, length)
    {}

	StringPrivate() {}

	string buf;
};

#define TAGGED_STRING_POINTER_RETURN(obj) reinterpret_cast<const char *>(((uintptr_t)obj ^ TAGGED_POINTER_STRING_FLAG) >> 1)
#define _make_shared_ptr(c) shared_ptr<String>(c, __deleter__())

String::String(const char * s)
:Object(new StringPrivate(s))
{}

String::String(const void *bytes, uinteger length)
:Object(new StringPrivate(bytes, length))
{}

String::String(const string & s)
:Object(new StringPrivate(s))
{}

String::String(string && s)
:Object(new StringPrivate(std::move(s)))
{}

String::String(const Data & data)
:Object(new StringPrivate(data))
{}

String::String(const String &other)
:Object(new StringPrivate)
{
    D_D(String);
    if (other.isTaggedPointer()) {
        d.buf.append(begin().base(), other.length());
    } else {
        d.buf = D_O(String, other).buf;
    }
}

String::String(String &&other)
:Object(new StringPrivate)
{
    D_D(String);
    if (other.isTaggedPointer()) {
        d.buf.append(begin().base(), other.length());
    } else {
        d.buf = std::move(D_O(String, other).buf);
    }
}

String::String()
:Object(new StringPrivate)
{}

String::~String()
{
}

uinteger String::length() const
{
    if (isTaggedPointer()) {
        uintptr_t ptr = (uintptr_t)this;
        uinteger len = 0;
        if (TAGGED_POINTER_STRING_HAS_A_LEGNTH(ptr)) {
            len = (ptr & TAGGED_POINTER_STRING_LENGTH_MASK) >> TAGGED_POINTER_STRING_LENGTH_OFFSET;
            parameterAssert(len <= TAGGED_POINTER_STRING_MAX_LENGTH);
        } else {
            const char *str = TAGGED_POINTER_STRING_GET_STRING(ptr);
            len = strlen(str);
        }
        return len;
    }
	D_D(String);
	return d.buf.size();
}

uinteger String::capacity() const
{
    if (isTaggedPointer()) {
        return length();
    }
	D_D(String);
	return d.buf.capacity();
}

shared_ptr<String> String::substringFromIndex(uinteger index) const
{
	D_D(String);
	return substring(Range(index, d.buf.size() - index));
}

shared_ptr<String> String::substringToIndex(uinteger index) const
{
	return substring(Range(0, index));
}

shared_ptr<String> String::substring(Range range) const
{
    if (isTaggedPointer()) {
        auto ptr = (uintptr_t)this;
        const char *str = TAGGED_POINTER_STRING_GET_STRING(ptr);
        return make_shared<String>(str, length());
    }
	D_D(String);
	string substr = d.buf.substr(range.location, range.length);
	return shared_ptr<String>(new String(std::move(substr)));
}

bool String::compare(const String & aString) const
{
    if (aString.length() != length()) {
        return false;
    }
    auto iter_s_this = this->begin();
    auto iter_s_other = aString.begin();
    auto iter_e = this->end();
    while (*iter_s_this++ == *iter_s_other++ && iter_s_other != iter_e) {
        continue;
    }
    return iter_s_this == iter_e;
}

bool String::caseInsensitiveCompare(const String & aString) const
{
    string s1(begin(), end());
    string s2(aString.begin(), aString.end());
    std::transform(s1.begin(), s1.end(), s1.begin(), ::toupper);
    std::transform(s2.begin(), s2.end(), s2.begin(), ::toupper);
	return s1 == s1;
}

bool String::isEqualToString(const String & aString) const
{
	return compare(aString);
}

bool String::hasPrefix(const String & str) const
{
    if (this->length() < str.length()) {
        return false;
    }
	auto b1 = begin();
	auto b2 = str.begin();
    auto e = str.end();
	while (*b1++ == *b2++ && b2 != e) {
		continue;
	}
	return b2 == e;
}

bool String::hasSuffix(const String & str) const
{
    if (this->length() < str.length()) {
        return false;
    }
    auto b1 = rbegin();
    auto b2 = str.rbegin();
    auto e = str.rend();
	while (*b1++ == *b2++ && b2 != e) {
		continue;
	}
	return b2 == e;
}

bool String::containsString(const String &aString) const
{
    return rangeOfString(aString).location != NotFound;
}

Range String::rangeOfString(const String &aString) const
{
    auto s1 = this->begin().base();
    auto s2 = aString.begin().base();
    return BMContainsString(s1, (uint32_t)length(), s2, (uint32_t)aString.length());
}

shared_ptr<String> String::stringByAppendingString(const String &aString) const
{
    auto copy = this->copy<String>();
    D_O(String, *copy).buf += D_O(String, aString).buf;
    return shared_ptr<String>(copy);
}

shared_ptr<String> String::stringByAppendingFormat(const char *format, ...) const
{
    auto copy = this->copy<String>();
    do {
        if (!format || !*format) {
            break;
        }
        va_list ap;
        va_start(ap, format);
        char *buffer = nullptr;
        uint32_t capacity = 0;
        uint32_t length = (uint32_t)tprintf_c(buffer, &capacity, format, ap, OUTPUT_FLAG_DESCRIPTION);
        va_end(ap);
        D_O(String, *copy).buf.append(buffer, length);
        free(buffer);
        buffer = nullptr;
    } while (0);
    return shared_ptr<String>(copy);
}

shared_ptr<String> String::stringByReplacingOccurrencesOfStringWithString(const String &target, const String &replacement) const
{
    auto copy = this->copy<String>();
    do {
        vector<Range> result;
        searchAllOfOccurrencesOfString(begin().base(),
                                       (uint32_t)length(),
                                       target.begin().base(),
                                       (uint32_t)target.length(),
                                       result);
        if (result.empty()) {
            break;
        }
        auto &buf = D_O(String, *copy).buf;
        auto rep = replacement.begin().base();
        auto repLength = replacement.length();
        for (auto &range : result) {
            buf.replace(range.location, range.length, rep, repLength);
        }
    } while (0);
    return shared_ptr<String>(copy);
}

shared_ptr<String> String::stringByReplacingCharactersInRange(Range range, const String &replacement) const
{
    auto copy = this->copy<String>();
    do {
        if (range.maxRange() > length()) {
            // TODO: 抛出异常
            break;
        }
        auto rep = replacement.begin().base();
        auto repLength = replacement.length();
        auto &buf = D_O(String, *copy).buf;
        buf.replace(range.location, range.length, rep, repLength);
    } while (0);
    return shared_ptr<String>(copy);
}

shared_ptr<vector<shared_ptr<String>>> String::componentsSeparatedByString(const String &separaotr) const
{
    auto result = make_shared<vector<shared_ptr<String>>>(0);
    vector<Range> ranges;
    searchAllOfOccurrencesOfString(begin().base(),
                                   (uint32_t)length(),
                                   separaotr.begin().base(),
                                   (uint32_t)separaotr.length(),
                                   ranges);
    for (auto &range : ranges) {
        result->push_back(this->substring(range));
    }
    return result;
}

double String::doubleValue() const
{
    return std::stod(begin().base());
}

float String::floatValue() const
{
    return std::stof(begin().base());
}

int String::intValue() const
{
    return std::stoi(begin().base());
}

long String::longValue() const
{
    return std::stol(begin().base());
}

long long String::longLongValue() const
{
    return std::stoll(begin().base());
}

bool String::boolValue() const
{
    return std::stod(begin().base());
}

integer String::integerValue() const
{
    return
#if __PL64__
    std::stoll(begin().base());
#else
    std::stoi(begin().base());
#endif
}

uinteger String::unsignedIntegerValue() const
{
    return
#if __PL64__
    std::stoull(begin().base());
#else
    (uint32_t)std::stoi(begin().base());
#endif
}

shared_ptr<String> String::uppercaseString() const
{
    auto copy = this->copy<String>();
    auto &s = D_O(String, *copy).buf;
    std::transform(s.begin(), s.end(), s.begin(), ::toupper);
    return shared_ptr<String>(copy);
}

shared_ptr<String> String::lowercaseString() const
{
    auto copy = this->copy<String>();
    auto &s = D_O(String, *copy).buf;
    std::transform(s.begin(), s.end(), s.begin(), ::tolower);
    return shared_ptr<String>(copy);
}

shared_ptr<Data> String::dataUsingEncoding() const
{
    auto data = make_shared<Data>(begin().base(), length());
    return data;
}

uinteger String::getBytes(void *buffer, uinteger bufferLength) const
{
    parameterAssert(buffer != nullptr);
    uinteger length = std::min(bufferLength, this->length());
    memcpy(buffer, begin().base(), length);
    return length;
}

const char& String::operator[](const size_t pos) const
{
    return begin()[pos];
}

// creation
shared_ptr<String> String::stringWithString(const String &other)
{
    return shared_ptr<String>(other.copy<String>());
}

shared_ptr<String> String::stringWithBytes(const void *bytes, uinteger length)
{
    parameterAssert(bytes != nullptr);
    if (is_steady_pointer(bytes)) { // make a tagged pointer
        auto str_int = (uintptr_t)bytes << 1 | TAGGED_POINTER_STRING_FLAG;
        if (length <= TAGGED_POINTER_STRING_MAX_LENGTH) {
            str_int |= ((length << TAGGED_POINTER_STRING_LENGTH_OFFSET) | TAGGED_POINTER_STRING_LENGTH_FLAG);
        }
        return shared_ptr<String>((String *)str_int, __deleter__());
    }
    return make_shared<String>(bytes, length);
}

shared_ptr<String> String::stringWithUTF8String(const char *nullTerminatedCString)
{
    if (!nullTerminatedCString) {
        return nullptr;
    }
    size_t length = strlen(nullTerminatedCString);
    return stringWithBytes(nullTerminatedCString, length);
}

shared_ptr<String> String::stringWithFormat(const char *format, ...)
{
    va_list ap;
    va_start(ap, format);
    auto str = stringWithFormat(format, ap);
    va_end(ap);
    return str;
}

shared_ptr<String> String::stringWithFormat(const char *format, va_list argList)
{
    char *buffer = nullptr;
    uint32_t capacity = 0;
    uinteger length = tprintf_c(buffer, &capacity, format, argList, OUTPUT_FLAG_DESCRIPTION);
    auto str = make_shared<String>(buffer, length);
    free(buffer);
    return str;
}

std::ostream& operator<<(std::ostream& os, const String &str)
{
    return os << &str;
}

std::ostream& operator<<(std::ostream& os, const shared_ptr<String> &str)
{
    return os << str.get();
}

std::ostream& operator<<(std::ostream& os, const String *str)
{
    if (!str) {
        os << "(null)";
    } else {
        os << str->begin().base();
    }
    return os;
}

Object *String::duplicate() const
{
    if (isTaggedPointer()) {
        string str(begin(), end());
        String *sstr = new String(std::move(str));;
        return sstr;
    } else {
        return Object::duplicate();
    }
}

char *String::__get_pointer() _NOEXCEPT
{
    D_D(String);
    return (char *)d.buf.data();
}

const char *String::__get_pointer() const _NOEXCEPT
{
    D_D(String);
    return d.buf.data();
}


// MutableString

struct MutableStringPrivate : StringPrivate {};

MutableString::MutableString()
:String()
{}

MutableString::MutableString(const char *s)
:String(s)
{}

MutableString::MutableString(const void *bytes, uinteger length)
:String(bytes, length)
{}

MutableString::MutableString(const string &s)
:String(s)
{}

MutableString::MutableString(string &&s)
:String(std::move(s))
{}

MutableString::MutableString(const Data &data)
:String(data)
{}

MutableString::MutableString(const String &other)
:String(other)
{}

MutableString::MutableString(String &&other)
:String(std::move(other))
{}

MutableString::MutableString(const MutableString &other)
:String(other)
{}

MutableString::MutableString(MutableString &&other)
:String(std::move(other))
{}

void MutableString::deleteCharactersInRange(Range range) _NOEXCEPT_(false)
{
    parameterAssert(!isTaggedPointer());
    do {
        if (range.maxRange() > length()) {
            break;
        }
        D_D(MutableString);
        d.buf.erase(range.location, range.length);
    } while (0);
}

MutableString& MutableString::append(const String &other)
{
    parameterAssert(!isTaggedPointer());
    D_D(MutableString);
    d.buf.append(other.begin(), other.end());
    return *this;
}

MutableString& MutableString::append(const char *format, ...)
{
    parameterAssert(!isTaggedPointer());
    va_list ap;
    va_start(ap, format);
    append(format, ap);
    va_end(ap);
    return *this;
}

MutableString& MutableString::append(const char *format, va_list argList)
{
    parameterAssert(!isTaggedPointer());
    insert(length(), format, argList);
    return *this;
}

MutableString& MutableString::append(const char *nullTerminatedCString)
{
    parameterAssert(!isTaggedPointer());
    insert(nullTerminatedCString, length());
    return *this;
}

MutableString& MutableString::append(const Data &data)
{
    parameterAssert(!isTaggedPointer());
    insert(data, length());
    return *this;
}

MutableString& MutableString::append(char c)
{
    parameterAssert(!isTaggedPointer());
    insert(c, length());
    return *this;
}

void MutableString::insert(char c, uinteger pos)
{
    parameterAssert(!isTaggedPointer());
    D_D(MutableString);
    d.buf.insert(pos, 1, c);
}

void MutableString::insert(const char *nullTerminatedCString, uinteger pos)
{
    parameterAssert(!isTaggedPointer());
    D_D(MutableString);
    d.buf.insert(pos, nullTerminatedCString);
}

void MutableString::insert(const byte *b, uinteger length, uinteger pos)
{
    parameterAssert(!isTaggedPointer());
    D_D(MutableString);
    d.buf.insert(pos, (const char *)b, length);
}

void MutableString::insert(const Data &data, uinteger pos)
{
    parameterAssert(!isTaggedPointer());
    D_D(MutableString);
    d.buf.insert(d.buf.begin() + pos, data.data(), data.data() + data.length());
}

void MutableString::insert(const String &aString, uinteger pos)
{
    parameterAssert(!isTaggedPointer());
    D_D(MutableString);
    d.buf.insert(d.buf.begin() + pos, aString.begin(), aString.end());
}

void MutableString::insert(uinteger pos, const char *format, ...)
{
    parameterAssert(!isTaggedPointer());
    va_list ap;
    va_start(ap, format);
    insert(pos, format, ap);
    va_end(ap);
}

void MutableString::insert(uinteger pos, const char *format, va_list argList)
{
    parameterAssert(!isTaggedPointer());
    char *buffer = nullptr;
    uint32_t capacity = 0;
    uinteger length = tprintf_c(buffer, &capacity, format, argList, OUTPUT_FLAG_DESCRIPTION);
    D_D(MutableString);
    d.buf.insert(pos, buffer, length);
    free(buffer);
}

void MutableString::clear()
{
    parameterAssert(!isTaggedPointer());
    D_D(MutableString);
    d.buf.clear();
}

char& MutableString::operator[](const size_t pos)
{
    parameterAssert(!isTaggedPointer());
    D_D(String);
    return d.buf[pos];
}

void MutableString::replaceOccurrencesOfStringWithString(const String &target, const String &replacement)
{
    parameterAssert(!isTaggedPointer());
    vector<Range> result;
    D_D(MutableString);
    searchAllOfOccurrencesOfString(d.buf.data(),
                                   (uint32_t)d.buf.length(),
                                   target.begin().base(),
                                   (uint32_t)target.length(),
                                   result);
    auto rep = replacement.begin().base();
    auto repLength = replacement.length();
    for (auto &range : result) {
        d.buf.replace(range.location, range.length, rep, repLength);
    }
}

void MutableString::replaceCharactersInRangeWithString(Range range, const String &aString)
{
    parameterAssert(!isTaggedPointer());
    D_D(MutableString);
    if (range.maxRange() > d.buf.length()) {
        // TODO: 抛出异常
    }
    d.buf.replace(range.location, range.length, aString.begin().base(), aString.length());
}

CC_END
