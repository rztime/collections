//
//  Exception.cpp
//  TagBufCPP
//
//  Created by hejunqiu on 2016/9/26.
//  Copyright © 2016年 CHE. All rights reserved.
//

#include "Exception.hpp"
#include "String.hpp"
#include <execinfo.h>
#include "log.hpp"
#include "Number.hpp"
#include <exception>
#include <cstdlib>
#include "Object.inl"

using namespace std;

CC_BEGIN

shared_ptr<String> const RangeException = s("RangeException");
shared_ptr<String> const InvalidArgumentException = s("CHInvalidArgumentException");
shared_ptr<String> const OutOfRangeException = s("CHOutOfRangeException");

static inline void exceptionHandler()
{
    exception_ptr ptr = current_exception();
    try {
        rethrow_exception(ptr);
    } catch (Exception *e) {
        CHLog("%p@", e);
        e->callStackSymbols();
        rethrow_exception(ptr);
    } catch (exception *e) {
        CHLog("%s", e->what());
        rethrow_exception(ptr);
    } catch (void *p) {
        printf("%p", p);
        rethrow_exception(ptr);
    }
}

struct ExceptionPrivate : public ObjectPrivate
{
    String *name;
    String *reason;
    Object *userInfo;
    void *reserved = 0;

    ExceptionPrivate(const shared_ptr<String> &exceptionName, const shared_ptr<String> &reason, const shared_ptr<Object> &userInfo)
    :name(exceptionName ? exceptionName->copy<String>() : nullptr)
    ,reason(reason ? reason->copy<String>() : nullptr)
    ,userInfo(userInfo ? userInfo->copy<Object>() : nullptr)
    {}

    ExceptionPrivate()
    :name(nullptr)
    ,reason(nullptr)
    ,userInfo(nullptr)
    {}

    ExceptionPrivate* duplicate () const override
    {
        auto copy = new ExceptionPrivate();
        if (name) {
            copy->name = name->copy<String>();
        }
        if (reason) {
            copy->reason = reason->copy<String>();
        }
        if (userInfo) {
            copy->userInfo = userInfo->copy<Object>();
        }
        return copy;
    }
};

Exception::Exception() : Object(new ExceptionPrivate()) {}

Exception::Exception(const shared_ptr<String> &exceptionName,
                     const shared_ptr<String> &reason,
                     const shared_ptr<Object> &userInfo)
:Object(new ExceptionPrivate(exceptionName, reason, userInfo))
{
}

// creation
shared_ptr<Exception> Exception::exceptionWithExceptionName(const shared_ptr<String> &exceptionName,
                                                 const shared_ptr<String> &reason,
                                                 const shared_ptr<Object> &userInfo)
{
    return make_shared<Exception>(exceptionName, reason, userInfo);
}

shared_ptr<Exception> Exception::exceptionWithExceptionName(const shared_ptr<String> &exceptionName, const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    shared_ptr<String> reason = String::stringWithFormat(fmt, ap);
    va_end(ap);
    return make_shared<Exception>(exceptionName, reason, nullptr);
}

void Exception::raise(const shared_ptr<String> &exceptionName,
                      const char *format,
                      ...) throw()
{
    va_list ap;
    va_start(ap, format);
    raise(exceptionName, format, ap);
}

void Exception::raise(const shared_ptr<String> &exceptionName,
                      const char *format,
                      va_list argList) throw()
{
    shared_ptr<String> reason = String::stringWithFormat(format, argList);
    va_end(argList);
    auto e = Exception::exceptionWithExceptionName(exceptionName, reason, nullptr);
    throw e;
}

// method
shared_ptr<String> Exception::exceptionName() const
{
    D_D(Exception);
    return shared_ptr<String>(d.name->copy<String>());
}

shared_ptr<String> Exception::reason() const
{
    D_D(Exception);
    return shared_ptr<String>(d.reason->copy<String>());
}

shared_ptr<Object> Exception::userInfo() const
{
    D_D(Exception);
    return shared_ptr<Object>(d.userInfo->copy<Object>());
}

shared_ptr<vector<shared_ptr<Number>>> Exception::callStackReturnAddresses() const
{
    void *d[10]{0};
    int count = backtrace(d, 10);
    vector<shared_ptr<Number>> addrr(10);
    for (int i=0; i<count; ++i) {
        addrr.push_back(Number::numberWithValue((uintptr_t)d[i]));
    }
    return make_shared<vector<shared_ptr<Number>>>(std::move(addrr));
}

shared_ptr<vector<shared_ptr<String>>> Exception::callStackSymbols() const
{
    void *array[10] = {0};
    int size = 0;
    char **strings = nullptr;
    size = backtrace (array, 10);
    strings = backtrace_symbols (array, size);
    vector<shared_ptr<String>> symbols(10);
    for (int i = 0; i < size; i++) {
        symbols.emplace_back(make_shared<String>(strings[i]));
    }
    free(strings);
    return make_shared<vector<shared_ptr<String>>>(std::move(symbols));
}

CC_END
