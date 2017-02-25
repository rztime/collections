//
//  Range.cpp
//  TagBufCPP
//
//  Created by hejunqiu on 2016/9/23.
//  Copyright © 2016年 CHE. All rights reserved.
//

#include "Range.hpp"
#include "String.hpp"

CC_BEGIN

extern String *showRange(Range range)
{
    String *str = String::stringWithFormat("{.location=%u, .length=%u}",
                                               range.location, range.length);
    return str;
}

CC_END
