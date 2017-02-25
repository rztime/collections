//
//  Range.hpp
//  TagBufCPP
//
//  Created by hejunqiu on 2016/9/23.
//  Copyright © 2016年 CHE. All rights reserved.
//

#ifndef Range_hpp
#define Range_hpp

#include "defines.h"

CC_BEGIN

class String;

typedef struct Range {
    uint32_t location;
    uint32_t length;

    Range(uint32_t loc = 0, uint32_t len = 0)
    :location(loc),
    length(len)
    {}

    bool operator==(const Range &range)
    { return location == range.location && length == range.length; }

} Range;

static inline uint32_t maxRange(Range range) {
    return (range.location + range.length);
}

static inline bool locationInRange(uint32_t loc, Range range) {
    return (!(loc < range.location) && (loc - range.location) < range.length) ? true : false;
}

extern String *showRange(Range range);

CC_END

#endif /* Range_hpp */
