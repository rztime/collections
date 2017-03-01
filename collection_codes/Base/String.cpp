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
#include "Data.hpp"
#include "tprintf.hpp"
#include "Algorithm.hpp"
#include "Exception.hpp"
#include "TaggedPointer.h"
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

static inline char *get_taggepointer_str(uintptr_t ptr)
{
    if (TAGGED_POINTER_STRING_HAS_A_LEGNTH(ptr)) {
        ptr ^= (TAGGED_POINTER_STRING_FLAG | TAGGED_POINTER_STRING_LENGTH_FLAG);
        ptr &= (~TAGGED_POINTER_STRING_LENGTH_MASK);
    }
    auto str = reinterpret_cast<char *>(ptr >> 1);
    return str;
}

String::String(const char * s)  _NOEXCEPT
:Object(new StringPrivate(s))
{}

String::String(const void *bytes, uinteger length)  _NOEXCEPT
:Object(new StringPrivate(bytes, length))
{}

String::String(const string & s)  _NOEXCEPT
:Object(new StringPrivate(s))
{}

String::String(string && s) _NOEXCEPT
:Object(new StringPrivate(std::move(s)))
{}

String::String(const Data & data) _NOEXCEPT
:Object(new StringPrivate(data))
{}

String::String(const String &other) _NOEXCEPT
:Object(new StringPrivate)
{
    D_D(String);
    if (other.isTaggedPointer()) {
        d.buf.append(begin().base(), other.length());
    } else {
        d.buf = D_O(String, other).buf;
    }
}

String::String(String &&other) _NOEXCEPT
:Object(new StringPrivate)
{
    D_D(String);
    if (other.isTaggedPointer()) {
        d.buf.append(begin().base(), other.length());
    } else {
        d.buf = std::move(D_O(String, other).buf);
    }
}

String::String() _NOEXCEPT
:Object(new StringPrivate)
{}

String::~String() _NOEXCEPT
{
}

shared_ptr<String> String::description() const _NOEXCEPT
{
    if (isTaggedPointer()) {
        return shared_ptr<String>((String *)this, __deleter__());
    }
	return shared_ptr<String>(copy<String>());
}

uinteger String::length() const _NOEXCEPT
{
    if (isTaggedPointer()) {
        uintptr_t ptr = (uintptr_t)this;
        uinteger len = 0;
        if (TAGGED_POINTER_STRING_HAS_A_LEGNTH(ptr)) {
            len = (ptr & TAGGED_POINTER_STRING_LENGTH_MASK) >> TAGGED_POINTER_STRING_LENGTH_OFFSET;
            parameterAssert(len <= TAGGED_POINTER_STRING_MAX_LENGTH);
        } else {
            const char *str = get_taggepointer_str((uintptr_t)this);
            len = strlen(str);
        }
        return len;
    }
	D_D(String);
	return d.buf.size();
}

uinteger String::capacity() const _NOEXCEPT
{
    if (isTaggedPointer()) {
        return length();
    }
	D_D(String);
	return d.buf.capacity();
}

shared_ptr<String> String::substringFromIndex(uinteger index) const _NOEXCEPT
{
	return substring(Range(index, length() - index));
}

shared_ptr<String> String::substringToIndex(uinteger index) const _NOEXCEPT
{
	return substring(Range(0, index));
}

shared_ptr<String> String::substring(Range range) const _NOEXCEPT
{
    if (isTaggedPointer()) {
        const char *str = get_taggepointer_str((uintptr_t)this);
        return String::stringWithBytes(str + range.location, range.length);
    }
	D_D(String);
	string substr = d.buf.substr(range.location, range.length);
	return shared_ptr<String>(new String(std::move(substr)));
}

ComparisonResult String::compare(const String & aString) const _NOEXCEPT
{
    auto iter_s_this = this->begin();
    auto iter_e_this = this->end();
    auto iter_s_other = aString.begin();
    while (*iter_s_this++ == *iter_s_other++ && iter_s_other != iter_e_this) {
        continue;
    }
    if (iter_s_this == iter_e_this) {
        return ComparisonResult::Same;
    }
    if (iter_s_other == aString.end()) {
        return ComparisonResult::Descending;
    }

    if (*iter_s_this > *iter_s_other) {
        return ComparisonResult::Descending;
    } else {
        return ComparisonResult::Ascending;
    }
}

ComparisonResult String::caseInsensitiveCompare(const String & aString) const _NOEXCEPT
{
    string s1(begin(), end());
    string s2(aString.begin(), aString.end());
    std::transform(s1.begin(), s1.end(), s1.begin(), ::toupper);
    std::transform(s2.begin(), s2.end(), s2.begin(), ::toupper);
    String str1(std::move(s1));
    String str2(std::move(s2));
	return str1.compare(str2);
}

bool String::isEqualToString(const String & aString) const _NOEXCEPT
{
    return compare(aString) == ComparisonResult::Same;
}

bool String::hasPrefix(const String & str) const _NOEXCEPT
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

bool String::hasSuffix(const String & str) const _NOEXCEPT
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

bool String::containsString(const String &aString) const _NOEXCEPT
{
    return rangeOfString(aString).location != NotFound;
}

Range String::rangeOfString(const String &aString) const _NOEXCEPT
{
    auto s1 = this->begin().base();
    auto s2 = aString.begin().base();
    return BMContainsString(s1, (uint32_t)length(), s2, (uint32_t)aString.length());
}

shared_ptr<String> String::stringByAppendingString(const String &aString) const _NOEXCEPT
{
    auto copy = this->copy<String>();
    D_O(String, *copy).buf += D_O(String, aString).buf;
    return shared_ptr<String>(copy);
}

shared_ptr<String> String::stringByAppendingFormat(const char *format, ...) const _NOEXCEPT
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

shared_ptr<String> String::stringByReplacingOccurrencesOfStringWithString(const String &target, const String &replacement) const _NOEXCEPT
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

shared_ptr<String> String::stringByReplacingCharactersInRange(Range range, const String &replacement) const _NOEXCEPT
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

shared_ptr<vector<shared_ptr<String>>> String::componentsSeparatedByString(const String &separaotr) const _NOEXCEPT
{
    auto result = make_shared<vector<shared_ptr<String>>>(0);
    vector<Range> ranges;
    searchAllOfOccurrencesOfString(begin().base(),
                                   (uint32_t)length(),
                                   separaotr.begin().base(),
                                   (uint32_t)separaotr.length(),
                                   ranges);
    if (!ranges.empty()) {
        uinteger offset = 0;
        Range spiltRange;
        for (auto &range : ranges) {
            if (offset == range.location) {
                offset = range.maxRange();
                continue;
            }
            spiltRange.location = offset;
            spiltRange.length = range.location - spiltRange.location;
            result->push_back(this->substring(spiltRange));
            offset = range.maxRange();
        }
        uinteger len = length();
        if (offset < len) {
            spiltRange.location = offset;
            spiltRange.length = len - offset;
            result->push_back(this->substring(spiltRange));
        }
    }
    return result;
}

double String::doubleValue() const _NOEXCEPT
{
    return std::stod(begin().base());
}

float String::floatValue() const _NOEXCEPT
{
    return std::stof(begin().base());
}

int String::intValue() const _NOEXCEPT
{
    return std::stoi(begin().base());
}

long String::longValue() const _NOEXCEPT
{
    return std::stol(begin().base());
}

long long String::longLongValue() const _NOEXCEPT
{
    return std::stoll(begin().base());
}

bool String::boolValue() const _NOEXCEPT
{
    return std::stod(begin().base());
}

integer String::integerValue() const _NOEXCEPT
{
    return
#if __PL64__
    std::stoll(begin().base());
#else
    std::stoi(begin().base());
#endif
}

uinteger String::unsignedIntegerValue() const _NOEXCEPT
{
    return
#if __PL64__
    std::stoull(begin().base());
#else
    (uint32_t)std::stoi(begin().base());
#endif
}

shared_ptr<String> String::uppercaseString() const _NOEXCEPT
{
    auto copy = this->copy<String>();
    auto &s = D_O(String, *copy).buf;
    std::transform(s.begin(), s.end(), s.begin(), ::toupper);
    return shared_ptr<String>(copy);
}

shared_ptr<String> String::lowercaseString() const _NOEXCEPT
{
    auto copy = this->copy<String>();
    auto &s = D_O(String, *copy).buf;
    std::transform(s.begin(), s.end(), s.begin(), ::tolower);
    return shared_ptr<String>(copy);
}

shared_ptr<Data> String::dataUsingEncoding() const _NOEXCEPT
{
    auto data = make_shared<Data>(begin().base(), length());
    return data;
}

uinteger String::getBytes(void *buffer, uinteger bufferLength) const _NOEXCEPT
{
    parameterAssert(buffer != nullptr);
    uinteger length = std::min(bufferLength, this->length());
    memcpy(buffer, begin().base(), length);
    return length;
}

const char& String::operator[](const uinteger idx) const _NOEXCEPT(false)
{
    if (idx >= length()) {
        throwException(RangeException, "Parameter:idx(%llu) should not greater than string's length(%llu)", idx, this->length());
    }
    return begin()[idx];
}

// creation
shared_ptr<String> String::stringWithString(const String &other) _NOEXCEPT
{
    return shared_ptr<String>(other.copy<String>());
}

shared_ptr<String> String::stringWithBytes(const void *bytes, uinteger length) _NOEXCEPT
{
    parameterAssert(bytes != nullptr);
    if (is_steady_pointer(bytes)) { // make a tagged pointer
        auto str_int = (uintptr_t)bytes << 1 | TAGGED_POINTER_STRING_FLAG;
        if (length <= TAGGED_POINTER_STRING_MAX_LENGTH) {
            length <<= TAGGED_POINTER_STRING_LENGTH_OFFSET;
            str_int |= (length | TAGGED_POINTER_STRING_LENGTH_FLAG);
        }
        return shared_ptr<String>((String *)str_int, __deleter__());
    }
    return make_shared<String>(bytes, length);
}

shared_ptr<String> String::stringWithUTF8String(const char *nullTerminatedCString) _NOEXCEPT
{
    if (!nullTerminatedCString) {
        return nullptr;
    }
    size_t length = strlen(nullTerminatedCString);
    return stringWithBytes(nullTerminatedCString, length);
}

shared_ptr<String> String::stringWithFormat(const char *format, ...) _NOEXCEPT
{
    va_list ap;
    va_start(ap, format);
    auto str = stringWithFormat(format, ap);
    va_end(ap);
    return str;
}

shared_ptr<String> String::stringWithFormat(const char *format, va_list argList) _NOEXCEPT
{
    char *buffer = nullptr;
    uint32_t capacity = 0;
    uinteger length = tprintf_c(buffer, &capacity, format, argList, OUTPUT_FLAG_DESCRIPTION);
    auto str = make_shared<String>(buffer, length);
    free(buffer);
    return str;
}

std::ostream& operator<<(std::ostream& os, const String &str) _NOEXCEPT
{
    return os << &str;
}

std::ostream& operator<<(std::ostream& os, const shared_ptr<String> &str) _NOEXCEPT
{
    return os << str.get();
}

std::ostream& operator<<(std::ostream& os, const String *str) _NOEXCEPT
{
    if (!str) {
        os << "(null)";
    } else {
        os.write(str->begin().base(), str->length());
    }
    return os;
}

Object *String::duplicate() const _NOEXCEPT
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
    if (isTaggedPointer()) {
        return get_taggepointer_str((uintptr_t)this);
    }
    D_D(String);
    return (char *)d.buf.data();
}

const char *String::__get_pointer() const _NOEXCEPT
{
    if (isTaggedPointer()) {
        return get_taggepointer_str((uintptr_t)this);
    }
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

void MutableString::deleteCharactersInRange(Range range) NOEXCEPT(false)
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
