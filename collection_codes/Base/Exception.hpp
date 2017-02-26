//
//  Exception.hpp
//  TagBufCPP
//
//  Created by hejunqiu on 2016/9/26.
//  Copyright © 2016年 CHE. All rights reserved.
//

#ifndef EXCEPTION_HPP
#define EXCEPTION_HPP

#include <stdarg.h>
#include "Object.hpp"
#include <memory>
#include <vector>

using std::shared_ptr;
using std::vector;

CC_BEGIN

class String;
class Number;

class Exception : public Object
{
public:
    Exception(const shared_ptr<String> &exceptionName,
              const shared_ptr<String> &reason,
              const shared_ptr<Object> &userInfo);
    Exception();

    shared_ptr<String> exceptionName() const;
    shared_ptr<String> reason() const;
    shared_ptr<Object> userInfo() const;

    shared_ptr<vector<shared_ptr<Number>>> callStackReturnAddresses() const;
    shared_ptr<vector<shared_ptr<String>>> callStackSymbols() const;

    // creation
    static shared_ptr<Exception> exceptionWithExceptionName(const shared_ptr<String> &exceptionName,
                                                            const shared_ptr<String> &reason,
                                                            const shared_ptr<Object> &userInfo);
    static shared_ptr<Exception> exceptionWithExceptionName(const shared_ptr<String> &exceptionName,
                                                            const char *fmt,
                                                            ...) __printflike(2, 3);
    static void raise(const  shared_ptr<String> &exceptionName,
                      const char *format,
                      ...) throw() __printflike(2, 3);
    static void raise(const  shared_ptr<String> &exceptionName,
                      const char *format,
                      va_list argList) throw();
};

CC_EXTERN shared_ptr<String> const RangeException;
CC_EXTERN shared_ptr<String> const InvalidArgumentException;
CC_EXTERN shared_ptr<String> const OutOfRangeException;

#define throwException(name, format, ...) throw CHException::exceptionWithExceptionName(name, format, ##__VA_ARGS__)

CC_END

#endif /* EXCEPTION_HPP */
