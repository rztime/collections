//
//  String.hpp
//  TagBufCPP
//
//  Created by hejunqiu on 16/8/31.
//  Copyright © 2016年 CHE. All rights reserved.
//

#ifndef STRING_HPP
#define STRING_HPP

#include "Object.hpp"
#include "Range.hpp"
#include <stdarg.h>
#include <vector>
#include <memory>
#include <string>

using std::vector;
using std::shared_ptr;
using std::string;

CC_BEGIN

class Array;
class Data;
class MutableString;

CLASS_TAGGEDPOINTER_AVAILABLE class String : public Object
{
public:
    explicit String(const char *s);
    explicit String(const void *bytes, uinteger length);
    explicit String(const string &s);
    explicit String(string &&s);
    explicit String(const Data &data);
    explicit String(const String &other);
    explicit String(String &&other);
	String();
	~String() override;

    shared_ptr<String> duplicate() const;
    
    uinteger length() const;
    uinteger  capacity() const;

    // StringExtensionMethods
    shared_ptr<String> substringFromIndex(uinteger  index) const;
    shared_ptr<String> substringToIndex(uinteger  index) const;
    shared_ptr<String> substring(Range range) const;

    bool compare(const String &aString) const;
    bool caseInsensitiveCompare(const String &aString) const;
    bool isEqualToString(const String &aString) const;

    bool hasPrefix(const String &str) const;
    bool hasSuffix(const String &str) const;

    bool containsString(const String &aString) const;
    Range rangeOfString(const String &aString) const;

    shared_ptr<String> stringByAppendingString(const String &aString) const;
    shared_ptr<String> stringByAppendingFormat(const char *format, ...) const __printflike__(2, 3);
    shared_ptr<String> stringByReplacingOccurrencesOfStringWithString(const String &target, const String &replacement) const;
    shared_ptr<String> stringByReplacingCharactersInRange(Range range, const String &replacement) const;
    shared_ptr<vector<shared_ptr<String>>> componentsSeparatedByString(const String &separaotr) const;

    double doubleValue() const;
    float floatValue() const;
    int intValue() const;
    long longValue() const;
    long long longLongValue() const;
    bool boolValue() const;
    integer integerValue() const;
    uinteger unsignedIntegerValue() const;

    operator MutableString*() = delete;
    operator MutableString&() = delete;
    operator const MutableString*() const = delete;
    operator const MutableString&() const = delete;

    shared_ptr<String> uppercaseString() const;
    shared_ptr<String> lowercaseString() const;

    shared_ptr<Data> dataUsingEncoding() const;
    uinteger getBytes(void *buffer, uinteger bufferLength) const;

    // creation
    static shared_ptr<String> stringWithString(const String &other);
    static shared_ptr<String> stringWithBytes(const void *bytes, uinteger length);
    static shared_ptr<String> stringWithUTF8String(const char *nullTerminatedCString);
    static shared_ptr<String> stringWithFormat(const char *format, ...) __printflike__(1, 2);
    static shared_ptr<String> stringWithFormat(const char *format, va_list argList);
};

class MutableString : public String
{
public:
    explicit MutableString();
    explicit MutableString(const char *s);
    explicit MutableString(const void *bytes, uinteger length);
    explicit MutableString(const string &s);
    explicit MutableString(string &&s);
    explicit MutableString(const Data &data);
    explicit MutableString(const String &other);
    explicit MutableString(String &&other);
    explicit MutableString(const MutableString &other);
    explicit MutableString(MutableString &&other);

    void deleteCharactersInRange(Range range) throw();

    void append(const String &other);
    void append(const char *format, ...) __printflike__(2,3);
    void append(const char *format, va_list argList);
    void append(const char *nullTerminatedCString);
    void append(const Data &data);
    void append(char c);

    void insert(char c, uinteger pos);
    void insert(const char *nullTerminatedCString, uinteger pos);
    void insert(const byte *b, uinteger length, uinteger pos);
    void insert(const Data &data, uinteger pos);
    void insert(const String &aString, uinteger pos);
    void insert(uinteger pos, const char *format, ...) __printflike__(3,4);
    void insert(uinteger pos, const char *format, va_list argList);

    void clear();

    void replaceOccurrencesOfStringWithString(const String &target, const String &replacement);
    void replaceCharactersInRangeWithString(Range range, const String &aString);
};

CC_END
#endif /* STRING_HPP */
