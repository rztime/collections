//
//  Date.cpp
//  FMDB-CPP
//
//  Created by hejunqiu on 2017/2/10.
//  Copyright © 2017年 CHE. All rights reserved.
//

#include "Date.hpp"

CC_BEGIN

const TimeInterval TimeIntervalSince1970 = TimeInterval(978307200.0);
const char *const FMDB_CPP_DATE_FORMAT = "%a %b %d %Y %H:%M:%S %z";

Date::Date() _NOEXCEPT_
:d(system_clock::now())
{
}

Date::Date(time_point tp) _NOEXCEPT_
:d(tp)
{
}

Date::Date(TimeInterval SinceReferenceDate) _NOEXCEPT_
:d(duration_cast<system_clock::duration>(TimeIntervalSince1970 + SinceReferenceDate))
{
}

Date Date::clone() _NOEXCEPT_
{
    time_point t;
    Date date(t);
    date.d = d;
    return date;
}

Date::~Date() _NOEXCEPT_ { }

TimeInterval Date::timeIntervalSinceDate(Date anotherData) const _NOEXCEPT_
{
    return d.time_since_epoch() - anotherData.d.time_since_epoch();
}

TimeInterval Date::timeIntervalSinceNow() const _NOEXCEPT_
{
    return d - system_clock::now();
}

TimeInterval Date::timeIntervalSince1970() const _NOEXCEPT_
{
    return d.time_since_epoch();
}

TimeInterval Date::timeIntervalSinceReferenceDate() const _NOEXCEPT_
{
    return d.time_since_epoch() - TimeIntervalSince1970;
}

Date Date::dateByAddingTimeInterval(TimeInterval seconds) const _NOEXCEPT_
{
    Date date;
    date.d += duration_cast<system_clock::duration>(seconds);
    return date;
}

Date Date::dateWithTimeIntervalSinceNow(TimeInterval secs) _NOEXCEPT_
{
    Date date;
    date.d += duration_cast<system_clock::duration>(secs);
    return date;
}

Date Date::dateWithTimeIntervalSince1970(TimeInterval secs) _NOEXCEPT_
{
    time_point t;
    Date date(t);
    date.d += duration_cast<system_clock::duration>(secs);
    return date;
}

Date Date::dateWithTimeIntervalSinceReferenceDate(TimeInterval ti) _NOEXCEPT_
{
    time_point t;
    Date date(t);
    date.d += duration_cast<system_clock::duration>(TimeIntervalSince1970 + ti);
    return date;
}

Date Date::dateWithTimeInterval(TimeInterval secsToAdded, Date sinceDate) _NOEXCEPT_
{
    sinceDate.d += duration_cast<system_clock::duration>(secsToAdded);
    return sinceDate;
}

Date Date::dateEpoch() _NOEXCEPT_
{
    return dateWithTimeIntervalSince1970(TimeInterval(0));
}

#if defined(_MSC_VER)
void strptime(const char *buf, const char *format, struct tm *tm) _NOEXCEPT_
{
	assert(format == FMDB_CPP_DATE_FORMAT, "TODO:");
	// TODO:
}
#endif // _MSC_VER

Date Date::dateFromString(const string &dateString) _NOEXCEPT_
{
    struct tm tm = {0};
    strptime(dateString.c_str(), FMDB_CPP_DATE_FORMAT, &tm);

    struct tm validate_tm{0};
    if (memcmp(&tm, &validate_tm, sizeof(struct tm)) == 0) {
        double count = std::stof(dateString.c_str());
        Date date = dateEpoch();
        date += TimeInterval(count);
        return date;
    }

    time_point t;
    Date date(t);
    date.d = system_clock::from_time_t(mktime(&tm));
    return date;
}

string Date::stringFromDate(const Date &date) _NOEXCEPT_
{
    time_t time = system_clock::to_time_t(date.d);
    char str[64];
    struct tm tm;
#if defined(_MSC_VER)
    localtime_s(&tm, &time);
#else
    localtime_r(&time, &tm);
#endif // _MSC_VER

    size_t length = strftime(str, sizeof(str), FMDB_CPP_DATE_FORMAT, &tm);
    return string(str, str + length);
}

string Date::description() const _NOEXCEPT_
{
    return Date::stringFromDate(*this);
}

CC_END
