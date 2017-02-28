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
    auto str = "123534fefer"_s;
    cout << str << endl;
    cout << str->length() << endl;
    return 0;
}
