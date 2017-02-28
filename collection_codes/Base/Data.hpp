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
    explicit Data(uinteger capacity = 0);
    explicit Data(const void *bytes, uinteger length);
    explicit Data(const vector<byte> &bytes);
    explicit Data(vector<byte> &&bytes);
    explicit Data(const string &aString);
    explicit Data(const Data &data);
    explicit Data(Data &&data);

    ~Data() override;

    uinteger length() const;
    uinteger capacity() const;
    void clear();

    void deleteBytesInRange(Range range);

    Data& operator=(const Data &rhs);
    Data& operator=(const std::vector<byte> &rhs);
    Data& operator=(const char *utf8String);
    Data& operator=(const std::string &rhs);

    Data& operator+=(const Data &rhs);
    Data& operator+=(const byte rhs);

    Data& append(const void *bytes, uinteger length);
    Data& append(const Data &data);
    Data& append(const vector<byte> &data);
    Data& append(const string &aString);

    void insert(byte b, uinteger pos);
    void insert(const void *b, uinteger length, uinteger pos);
    void insert(const Data &data, uinteger pos);
    void insert(const vector<byte> &data, uinteger pos);
    void insert(const string &aString, uinteger pos);

    void replace(Range range, const Data &data);
    void replace(Range range, const void *data, uinteger length);
    void resetInRange(Range range);

    byte operator[](const uinteger idx) const;
    byte& operator[](const uinteger idx);

	byte *data();
	const byte *data() const;

    friend std::ostream& operator<<(std::ostream& os, const Data &data);
    friend std::ostream& operator<<(std::ostream& os, const shared_ptr<Data> &data);
    friend std::ostream& operator<<(std::ostream& os, const Data *data);
};

CC_END
#endif /* DATA_HPP */
