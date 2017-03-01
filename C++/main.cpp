//
//  main.cpp
//  C++
//
//  Created by hejunqiu on 2017/2/27.
//  Copyright © 2017年 CHE. All rights reserved.
//

#include <iostream>
#include "String.hpp"

using namespace _cc;
using namespace std;

int main(int argc, const char * argv[]) {
    auto str = "abc53535353cb"_s;
    auto val = str->componentsSeparatedByString(*"53"_s);
    for (auto &s : *val) {
        cout << s << endl;
    }
    return 0;
}
