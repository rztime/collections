//
//  CHException.hpp
//  TagBufCPP
//
//  Created by hejunqiu on 2016/9/26.
//  Copyright © 2016年 CHE. All rights reserved.
//

#ifndef _exception_hpp
#define _exception_hpp

#include <stdarg.h>

#include "Object.hpp"

class String;
class Array;
class CHException : public Object
{
    __SUPPORTRUNTIME__(CHException);
    CHException(String *exceptionName, String *reason, id userInfo);
    CHException();
public:
    String *exceptionName() const;
    String *reason() const;
    id userInfo() const;

    ARRAY_CONTAINS_TYPE(Array *, CHNumber *) callStackReturnAddresses() const;
    ARRAY_CONTAINS_TYPE(Array *, String *) callStackSymbols() const;

    // creation
    static CHException *exceptionWithExceptionName(String *exceptionName, String *reason, id userInfo);
    static CHException *exceptionWithExceptionName(String *exceptionName, const char *fmt, ...) __printflike(2, 3);
    static void raise(String *exceptionName, const char *format, ...) throw() __printflike(2, 3);
    static void raise(String *exceptionName, const char *format, va_list argList) throw();

    // runtime
    Class getClass() const override;
    static Class getClass(std::nullptr_t);

    // protocol
    bool equalTo(id anObject) const override;
    String *description() const;
private:
    static id allocateInstance();
};

extern String *const RangeException;
extern String *const CHInvalidArgumentException;
extern String *const CHOutOfRangeException;

#define throwException(name, format, ...) throw CHException::exceptionWithExceptionName(name, format, ##__VA_ARGS__)
#endif /* _exception_hpp */
