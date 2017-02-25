//
//  Algorithm.hpp
//  TagBufCPP
//
//  Created by hejunqiu on 2016/9/24.
//  Copyright © 2016年 CHE. All rights reserved.
//

#ifndef Algorithm_hpp
#define Algorithm_hpp

#include "Range.hpp"
#include <vector>
using std::vector;

extern Range BMContainsString(const char *src, uint32_t srcLen, const char *pattern, uint32_t patternLength);

extern Range containsStringUsually(const char *src, uint32_t srcLen, const char *pattern, uint32_t patternLength);

extern void searchAllOfOccurrencesOfString(const char *src, uint32_t srcLen, const char *pattern, uint32_t patternLength, vector<Range> &result);

#endif /* Algorithm_hpp */
