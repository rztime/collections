//
//  CHBuffer.hpp
//  TagBufCPP
//
//  Created by hejunqiu on 16/9/2.
//  Copyright © 2016年 CHE. All rights reserved.
//

#ifndef CHBuffer_hpp
#define CHBuffer_hpp

#include <stdio.h>
#include "Object.hpp"
#include <functional>
#include <vector>
#include <string>

using std::vector;
using std::string;

CC_BEGIN

class Data : public Object
{
public:
    explicit Data(uinteger capacity = 0);
    explicit Data(const byte *bytes, uinteger length);
    explicit Data(const vector<byte> &bytes);
    explicit Data(vector<byte> &&bytes);
    explicit Data(const string &aString);
    explicit Data(const Data &data);
    explicit Data(Data &&data);

    Data *duplicate() const;
    ~Data() override;

    uinteger length() const;
    uinteger capacity() const;
    void clear();

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
    void insert(byte *b, uinteger length, uinteger pos);
    void insert(const Data &data, uinteger pos);
    void insert(const vector<byte> &data, uinteger pos);
    void insert(const string &aString, uinteger pos);

    byte operator[](const uinteger idx) const;
    byte& operator[](const uinteger idx);
};

CC_END
#endif /* CHBuffer_hpp */
