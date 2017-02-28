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
#include "Iterator.inl"
#include <stdarg.h>
#include <iosfwd>

CC_BEGIN

class Array;
class Data;
class MutableString;

CLASS_TAGGEDPOINTER_AVAILABLE class String : public Object
{
public:
    using iterator = _CC::iterator<char *>;
    using const_iterator = _CC::iterator<const char *>;
    using reverse_iterator = _CC::reverse_iterator<iterator>;
    using const_reverse_iterator = _CC::reverse_iterator<const_iterator>;
    using pointer = std::string::pointer;
    using const_pointer = std::string::const_pointer;
public:
    String(const char *s);
    explicit String(const void *bytes, uinteger length);
    explicit String(const string &s);
    explicit String(string &&s);
    explicit String(const Data &data);
    explicit String(const String &other);
    explicit String(String &&other);
    template<class InputIterator>
    explicit String(InputIterator first, InputIterator last);
	String();
	~String() override;

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

    const char& operator[](const size_t pos) const;

    // creation
    static shared_ptr<String> stringWithString(const String &other);
    static shared_ptr<String> stringWithBytes(const void *bytes, uinteger length);
    static shared_ptr<String> stringWithUTF8String(const char *nullTerminatedCString);
    static shared_ptr<String> stringWithFormat(const char *format, ...) __printflike__(1, 2);
    static shared_ptr<String> stringWithFormat(const char *format, va_list argList);

    // os
    friend std::ostream& operator<<(std::ostream& os, const String &str);
    friend std::ostream& operator<<(std::ostream& os, const shared_ptr<String> &str);
    friend std::ostream& operator<<(std::ostream& os, const String *str);

    // C++ STL style iterator
    iterator begin() _NOEXCEPT
    { return iterator(__get_pointer()); }

    const_iterator begin() const _NOEXCEPT
    { return const_iterator(__get_pointer()); }
    const_iterator cbegin() const _NOEXCEPT
    { return begin(); }

    iterator end() _NOEXCEPT
    { return iterator(__get_pointer() + length()); }
    const_iterator end() const _NOEXCEPT
    { return const_iterator(__get_pointer() + length()); }
    const_iterator cend() const _NOEXCEPT
    { return end(); }

    reverse_iterator rbegin() _NOEXCEPT
    { return reverse_iterator(end()); }
    const_reverse_iterator rbegin() const _NOEXCEPT
    { return const_reverse_iterator(end()); }
    const_reverse_iterator crbegin() const _NOEXCEPT
    { return rbegin(); }

    reverse_iterator rend() _NOEXCEPT
    { return reverse_iterator(begin()); }
    const_reverse_iterator rend() const _NOEXCEPT
    { return const_reverse_iterator(begin()); }
    const_reverse_iterator crend() const _NOEXCEPT
    { return rend(); }
protected:
    Object *duplicate() const override;
private:
    char * __get_pointer() _NOEXCEPT;
    const char * __get_pointer() const _NOEXCEPT;
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

    void deleteCharactersInRange(Range range) _NOEXCEPT_(false);

    MutableString& append(const String &other);
    MutableString& append(const char *format, ...) __printflike__(2,3);
    MutableString& append(const char *format, va_list argList);
    MutableString& append(const char *nullTerminatedCString);
    MutableString& append(const Data &data);
    MutableString& append(char c);

    void insert(char c, uinteger pos);
    void insert(const char *nullTerminatedCString, uinteger pos);
    void insert(const byte *b, uinteger length, uinteger pos);
    void insert(const Data &data, uinteger pos);
    void insert(const String &aString, uinteger pos);
    void insert(uinteger pos, const char *format, ...) __printflike__(3,4);
    void insert(uinteger pos, const char *format, va_list argList);

    void clear();
    char& operator[](const size_t pos);

    void replaceOccurrencesOfStringWithString(const String &target, const String &replacement);
    void replaceCharactersInRangeWithString(Range range, const String &aString);
};

template<class InputIterator>
String::String(InputIterator first, InputIterator last)
:String(first, last - first)
{}

#ifndef s
#define s(cstr) String::stringWithUTF8String(cstr)
#endif

__unused static shared_ptr<String> operator""_s(const char *cstr, size_t length)
{
	return String::stringWithBytes(cstr, length);
}


CC_END

#endif /* STRING_HPP */
