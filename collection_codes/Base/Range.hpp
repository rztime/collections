//
//  Range.hpp
//  TagBufCPP
//
//  Created by hejunqiu on 2016/9/23.
//  Copyright © 2016年 CHE. All rights reserved.
//

#ifndef RANGE_HPP
#define RANGE_HPP

#include "defines.h"
#include <memory>

CC_BEGIN

class String;

typedef struct Range {
    uinteger location;
    uinteger length;

    Range(uinteger loc = 0, uinteger len = 0)
    :location(loc),
    length(len)
    {}

    bool operator==(const Range &range)
    { return location == range.location && length == range.length; }

    uinteger maxRange()
    { return (location + length); }

    bool locationInRange(uinteger loc)
    { return (!(loc < location) && (loc - location) < length) ? true : false; }

} Range;

//CC_EXTERN std::shared_ptr<String> showRange(Range range);

CC_END

#endif /* RANGE_HPP */
