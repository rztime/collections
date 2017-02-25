//
//  String.cpp
//  TagBufCPP
//
//  Created by hejunqiu on 16/8/31.
//  Copyright © 2016年 CHE. All rights reserved.
//

#include "String.hpp"
#include "TaggedPointer.h"
#include <stdlib.h>
#include <string.h>
#include "tprintf.hpp"
#include "Algorithm.hpp"
#include "Data.hpp"
#include "CHException.hpp"
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
	:buf((char *)data.data(), (char *)data.data() + data.length)
	{}

	StringPrivate() {}

	string buf;
};

String::String(const char * s)
	: Object(new StringPrivate(s))
{
}

String::String(const string & s)
	: Object(new StringPrivate(s))
{
}

String::String(string && s)
	: Object(new StringPrivate(std::move(s)))
{
}

String::String(const Data & data)
	: Object(new StringPrivate(data))
{
}

String::String() 
: Object(new StringPrivate)
{}

String::~String()
{
}

shared_ptr<String> String::duplicate() const
{
	auto copy = make_shared<String>();
	D_D(String);
	D_O(String, *copy).buf = d.buf;
	return copy;
}

uinteger String::length() const
{
	D_D(String);
	return d.buf.size();
}

uinteger String::capacity() const
{
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
	D_D(String);
	string substr = d.buf.substr(range.location, range.length);
	return make_shared<String>(std::move(substr));
}

bool String::compare(const String & aString) const
{
	D_D(String);
	return d.buf == D_O(String, aString).buf;
}

bool String::caseInsensitiveCompare(const String & aString) const
{
	D_D(String);
	string m1 = d.buf;
	string m2 = D_O(String, aString).buf;
	std::transform(m1.begin(), m1.end(), m1.begin(), toupper);
	std::transform(m2.begin(), m2.end(), m2.begin(), toupper);
	return m1 == m2;
}

bool String::isEqualToString(const String & aString) const
{
	return compare(aString);
}

bool String::hasPrefix(const String & str) const
{
	D_D(String);
	auto &m1 = d.buf;
	auto &m2 = D_O(String, str).buf;
	integer length = m2.length();
	if (m1.length() < length) {
		return false;
	}
	auto b1 = m1.data();
	auto b2 = m2.data();
	while (*b1++ == *b2++ && --length > 0) {
		continue;
	}
	return length == 0;
}

bool String::hasSuffix(const String & str) const
{
	D_D(String);
	auto &m1 = d.buf;
	auto &m2 = D_O(String, str).buf;
	integer length = m2.length();
	if (m1.length() < length) {
		return false;
	}
	auto b1 = m1.data() + m1.length();
	auto b2 = m2.data() + length;
	while (*--b1 == *--b2 && --length > 0) {
		continue;
	}
	return length == 0;
}

#define TAGGED_STRING_POINTER_RETURN(obj) reinterpret_cast<const char *>(((uintptr_t)obj ^ TAGGED_POINTER_STRING_FLAG) >> 1)

CC_END
