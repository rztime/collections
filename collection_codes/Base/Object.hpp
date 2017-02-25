//
//  Object.hpp
//  TagBufCPP
//
//  Created by hejunqiu on 16/9/16.
//  Copyright © 2016年 CHE. All rights reserved.
//

#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <stdio.h>
#include "defines.h"

CC_BEGIN

class String;
struct ObjectPrivate;

CLASS_TAGGEDPOINTER_AVAILABLE class Object
{
public:
    explicit Object(ObjectPrivate *data = 0);
    friend void release_outer(Object *obj);
    virtual ~Object();

    bool isTaggedPointer() const;
    const char *objectType() const;

    // protocol
    virtual bool equalTo(const Object &anObject) const;
    virtual String *description() const;
    virtual uint64_t hash_code() const;
protected:
    struct ObjectPrivate *_d = 0;
};

struct ObjectPrivate
{
    virtual ~ObjectPrivate() {}
};

#define D_D(cls) auto &d = *(cls##Private *)_d
#define D_O(cls, o) (*(cls##Private *)(o)._d)

CC_END

#endif /* OBJECT_HPP */
