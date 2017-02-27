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

std::shared_ptr<String> showRange(Range range)
{
    auto str = String::stringWithFormat("{.location=%llu, .length=%llu}",
                                        range.location, range.length);
    return str;
}

CC_END
