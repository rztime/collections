//
//  Data.hpp
//  TagBufCPP
//
//  Created by hejunqiu on 16/9/2.
//  Copyright © 2016年 CHE. All rights reserved.
//

#ifndef DATA_HPP
#define DATA_HPP

#include "Object.hpp"
#include "Range.hpp"
#include <functional>
#include <iosfwd>


CC_BEGIN

class Data : public Object
{
public:
    explicit Data(uinteger capacity = 0) _NOEXCEPT_;
    explicit Data(const void *bytes, uinteger length) _NOEXCEPT_;
    explicit Data(const vector<byte> &bytes) _NOEXCEPT_;
    explicit Data(vector<byte> &&bytes) _NOEXCEPT_;
    explicit Data(const string &aString) _NOEXCEPT_;
    explicit Data(const Data &data) _NOEXCEPT_;
    explicit Data(Data &&data) _NOEXCEPT_;

    ~Data() _NOEXCEPT_ override;

    uinteger length() const _NOEXCEPT_;
    uinteger capacity() const _NOEXCEPT_;
    void clear() _NOEXCEPT_;

    void deleteBytesInRange(Range range) _NOEXCEPT_(false);

    Data& operator=(const Data &rhs) _NOEXCEPT_;
    Data& operator=(const std::vector<byte> &rhs) _NOEXCEPT_;
    Data& operator=(const char *utf8String) _NOEXCEPT_(false);
    Data& operator=(const std::string &rhs) _NOEXCEPT_;

    Data& operator+=(const Data &rhs) _NOEXCEPT_;
    Data& operator+=(const byte rhs) _NOEXCEPT_;

    Data& append(const void *bytes, uinteger length) _NOEXCEPT_(false);
    Data& append(const Data &data) _NOEXCEPT_;
    Data& append(const vector<byte> &data) _NOEXCEPT_;
    Data& append(const string &aString) _NOEXCEPT_;

    void insert(byte b, uinteger pos) _NOEXCEPT_(false);
    void insert(const void *b, uinteger length, uinteger pos) _NOEXCEPT_(false);
    void insert(const Data &data, uinteger pos) _NOEXCEPT_(false);
    void insert(const vector<byte> &data, uinteger pos) _NOEXCEPT_(false);
    void insert(const string &aString, uinteger pos) _NOEXCEPT_(false);

    void replace(Range range, const Data &data) _NOEXCEPT_(false);
    void replace(Range range, const void *data, uinteger length) _NOEXCEPT_(false);
    void resetInRange(Range range) _NOEXCEPT_(false);

    byte operator[](const uinteger idx) const _NOEXCEPT_(false);
    byte& operator[](const uinteger idx) _NOEXCEPT_(false);

	byte *data() _NOEXCEPT_;
	const byte *data() const _NOEXCEPT_;

    friend std::ostream& operator<<(std::ostream& os, const Data &data) _NOEXCEPT_;
    friend std::ostream& operator<<(std::ostream& os, const shared_ptr<Data> &data) _NOEXCEPT_;
    friend std::ostream& operator<<(std::ostream& os, const Data *data) _NOEXCEPT_;
};

CC_END
#endif /* DATA_HPP */
